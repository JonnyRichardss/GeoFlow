// Fill out your copyright notice in the Description page of Project Settings.

#include "GeoFlowAsset.h"
#include "GeoFlowComponent.h"
#include "DynamicMesh/MeshAttributeUtil.h"
#include "DynamicMesh/MeshNormals.h"
#include "GeoFlowNodeTypes.h"
#include "GeoFlowPrimitiveNodes.h"
#include "TriangulationTable.h"
#include "Misc/Paths.h"
#include "Misc/CoreMisc.h"
#include "Interfaces/IPluginManager.h"
DECLARE_STATS_GROUP(TEXT("JR_MarchingProfiling"),STATGROUP_JR,STATCAT_JR)
DECLARE_DWORD_ACCUMULATOR_STAT(TEXT("JR_LastEvaluateCalls"),JR_EVAL_CALLS,STATGROUP_JR)
DECLARE_DWORD_ACCUMULATOR_STAT(TEXT("JR_LastSamplesInsideObject"),JR_EVAL_SUCCESSES,STATGROUP_JR)
DECLARE_DWORD_ACCUMULATOR_STAT(TEXT("JR_LastCubesMarched"),JR_TOTAL_CUBES,STATGROUP_JR)
//#include "IcosphereGenerator.h"
;
void UGeoFlowAsset::Generate(UDynamicMeshComponent* parent)
{
	//TODO - pass Marching cubes settings thru from the component
	if (parent == nullptr) {
		return;
	}

	if (lastWorkingGraph != nullptr) {
		SaveFromEditor(lastWorkingGraph);
	}
	FDynamicMesh3* EditMesh = parent->GetMesh();

	if (!IsValid(Graph)) return;

	TArray<FVector3f> PrimitivePositions;
	for (UGFN_R_Base* node : Graph->Nodes) {
		if (node->NodeType() == EGeoFlowNodeType::Output) {
			_outputNode = Cast<UGFN_R_Output>(node);
			break;
		}
	}
	TArray<UGFN_R_Base*> frontier;
	TArray<UGFN_R_Base*> visited;
	for (UGeoFlowRuntimePin* pin : _outputNode->InputPins) {
		if (pin->Connection != nullptr) {
			frontier.Add(pin->Connection->OwningNode);
		}
	}
	while (!frontier.IsEmpty()) {
		UGFN_R_Base* baseNode = frontier.Pop();
		visited.Add(baseNode);
		for (UGeoFlowRuntimePin* pin : baseNode->InputPins) {
			if (pin->Connection != nullptr) {
				frontier.Add(pin->Connection->OwningNode);
			}
		}
		UGFN_R_BasePrimitive* primitiveNode = Cast<UGFN_R_BasePrimitive>(baseNode);
		if (primitiveNode != nullptr) {
			PrimitivePositions.Add(primitiveNode->GetPosition());
		}
	}
	TArray<FIntVector3> primitiveOffsets;

	for (FVector3f position : PrimitivePositions) {
		FVector3f offset = position;
		//must snap to double step width otherwise different (connected) prims can generate at 1 step width offset from each other causing geometry to be doubled
		offset /= Settings.stepSize * 2;
		FIntVector3 intOffset(FMath::RoundToInt(offset.X), FMath::RoundToInt(offset.Y), FMath::RoundToInt(offset.Z));
		intOffset *= 2;
		primitiveOffsets.Add(intOffset);

	}
	DoMarchingCubes(EditMesh, parent,primitiveOffsets);

	//compute normals -- the builtin calculation from faces seems better than my not-quite-a-derivative method
	if (!EditMesh->HasAttributes()) EditMesh->EnableAttributes();
	UE::Geometry::FMeshNormals normals(EditMesh);
	if (EditMesh->Attributes()->PrimaryNormals()->ElementCount() == 0)
	{
		EditMesh->Attributes()->PrimaryNormals()->CreateFromPredicate([](int, int, int)->bool {return true; }, 0.0f);
	}
	EditMesh->EnableVertexNormals(FVector3f::ZeroVector);
	normals.RecomputeOverlayNormals(EditMesh->Attributes()->PrimaryNormals(), true, true);
	normals.CopyToOverlay(EditMesh->Attributes()->PrimaryNormals(), false);

	parent->NotifyMeshUpdated();
	LastGenerate = FDateTime::UtcNow();
}

void UGeoFlowAsset::SaveFromEditor(UEdGraph* _workingGraph)
{
	
	lastWorkingGraph = _workingGraph;
	UGeoFlowRuntimeGraph* runtimeGraph = NewObject<UGeoFlowRuntimeGraph>(this);
	TArray<std::pair<FGuid, FGuid>> connections;
	TMap < FGuid, UGeoFlowRuntimePin*> idToPinMap;
	for (UEdGraphNode* uiNode : _workingGraph->Nodes) {
		UGFN_E_Base* node = Cast<UGFN_E_Base>(uiNode);
		node->needsResave = false;
		UGFN_R_Base* runtimeNode = node->CreateRuntimeNode(runtimeGraph, connections, idToPinMap);
		runtimeGraph->Nodes.Add(runtimeNode);
	}

	for (std::pair<FGuid, FGuid> connection : connections) {
		UGeoFlowRuntimePin* p1 = idToPinMap[connection.first];
		UGeoFlowRuntimePin* p2 = idToPinMap[connection.second];
		p1->Connection = p2;
	}
	Graph = runtimeGraph;
	LastGraphChange = FDateTime::UtcNow();
	
	
}
void UGeoFlowAsset::LoadToEditor(UEdGraph* _workingGraph)
{
	lastWorkingGraph = _workingGraph;
	TArray<std::pair<FGuid, FGuid>> connections;
	TMap < FGuid, UEdGraphPin*> idToPinMap;

	for (UGFN_R_Base* runtimeNode : Graph->Nodes) {
		UGFN_E_Base* newNode = runtimeNode->CreateEditorNode(_workingGraph, connections, idToPinMap);

		_workingGraph->AddNode(newNode, true, true);
	}
	for (std::pair<FGuid, FGuid> connection : connections) {
		UEdGraphPin* p1 = idToPinMap[connection.first];
		UEdGraphPin* p2 = idToPinMap[connection.second];
		p1->LinkedTo.Add(p2);
		p2->LinkedTo.Add(p1);
	}
}
bool UGeoFlowAsset::NeedsResave()
{
	//we couldnt re-save ourselves anyway so its false
	if (!IsValid(lastWorkingGraph)) {
		return false;
	}
	bool output = false;
	for (UEdGraphNode* uiNode : lastWorkingGraph->Nodes) {
		UGFN_E_Base* node = Cast<UGFN_E_Base>(uiNode);
		if (node->needsResave) {
			output = true;
		}
		
	}
	return output;
}
static FIntVector3 cornerOffsets[8] = { {-1,-1,-1},{-1,1,-1},{1,1,-1},{1,-1,-1},{-1,-1,1},{-1,1,1},{1,1,1},{1,-1,1} };
static FIntVector3 edgeOffsets[12] = { {-1,0,-1},{0,1,-1},{1,0,-1},{0,-1,-1},{-1,0,1},{0,1,1},{1,0,1},{0,-1,1},{-1,-1,0},{-1,1,0},{1,1,0},{1,-1,0} };
static std::pair<int, int> edgeVertices[12] = { {0,1},{1,2},{2,3},{3,0},{4,5},{5,6},{6,7},{7,4},{0,4},{1,5},{2,6},{3,7} };
static FIntVector3 OrthoOffsets[6] = { {2,0,0},{-2,0,0},{0,2,0},{0,-2,0},{0,0,2},{0,0,-2} };
void UGeoFlowAsset::DoMarchingCubes(UE::Geometry::FDynamicMesh3* EditMesh, UDynamicMeshComponent* parent,TArray<FIntVector3> primitiveOffsets)
{

	DECLARE_SCOPE_CYCLE_COUNTER(TEXT("MarchingCubes"), JR_MARCHINGCUBES_FULL, STATGROUP_JR)
	SET_DWORD_STAT(JR_EVAL_CALLS, 0)
	SET_DWORD_STAT(JR_EVAL_SUCCESSES, 0)
	SET_DWORD_STAT(JR_TOTAL_CUBES, 0)

	auto stepSize =Settings.stepSize;

	double cubeDiagonalSize = FMath::Sqrt(3 * (2 * stepSize) * (2 * stepSize));
	TArray<FIntVector3> frontier;

	//TArray<FIntVector3> visited;
	TSet<FIntVector> visited;

	TMap<FIntVector3, int> existingEdges;

	frontier.Append(primitiveOffsets); //start at centre of all primitives
	//vaguely
	//https://paulbourke.net/geometry/polygonise/
	while (!frontier.IsEmpty()){
		INC_DWORD_STAT(JR_TOTAL_CUBES)

		FIntVector3 pos = frontier.Pop();

		visited.Add(pos);

		double x = (pos.X *(stepSize));
		double y = (pos.Y *(stepSize));
		double z = (pos.Z *(stepSize));

		FVector3f points[8];
		float values[8];
		int cubeCase = 0;
		FIntVector3 edges[12];
		bool vertEnabled[12] = { false };
		int vertIDs[12];

		for (int idx = 0; idx < 8; idx++) {
			DECLARE_SCOPE_CYCLE_COUNTER(TEXT("Construct Points"), JR_MARCHINGCUBES_POINTSCONSTRUCT, STATGROUP_JR)
			int j = cornerOffsets[idx].X;
			int k = cornerOffsets[idx].Y;
			int l = cornerOffsets[idx].Z;
			points[idx] = FVector3f(x + (j * stepSize), y + (k * stepSize), z + (l * stepSize));
		}
		for (int idx = 0; idx < 12; idx++) {
		}
		//get all points
		for (int idx = 0; idx < 8; idx++) {
			DECLARE_SCOPE_CYCLE_COUNTER(TEXT("Sample Loop"), JR_MARCHINGCUBES_SAMPLE, STATGROUP_JR)
			float sample = SampleSDF(points[idx]);
			if (FMath::IsNaN(sample)) goto cubeDiscard; //if we hit NaN something has gone very wrong
			//nested-loop means goto is needed to continue twice
			if (sample > cubeDiagonalSize) {
				if (Settings.debug) {
					DECLARE_SCOPE_CYCLE_COUNTER(TEXT("Marching Cubes draw debug"), JR_MARCHINGCUBES_DBG, STATGROUP_JR)
					DrawDebugBox(parent->GetWorld(), FVector(x, y, z), FVector(stepSize),FColor::Red, false, 5.0f);
					DrawDebugSphere(parent->GetWorld(), FVector(x, y, z),.1,6,FColor::Yellow, false, 5.0f);
				}
				goto cubeDiscard; //cubes OUTSIDE the primitive don't propagate
			}
			else if (sample < (0.0 - cubeDiagonalSize)) {
				goto cubeSkip; //cubes INSIDE the primitve are skipped but still propagate
			}
			values[idx] = sample;
		}

		//use point samples to determine cube case
		for (int i = 0; i < 8; i++) {
			if (values[i] <= 0.0) {
				cubeCase |= 1 << i;
			}
		}

		if (edgeTable[cubeCase] == 0)
		{
			goto cubeSkip;
		}

		for (int i = 0; i < 12; i++) {
			if (edgeTable[cubeCase] & (1 << i)) {
				edges[i] = pos + edgeOffsets[i];
				vertEnabled[i] = true;
			}
		}	
		//append vertices to mesh
		for (int i0 = 0; i0 < 12; i0++) {
			DECLARE_SCOPE_CYCLE_COUNTER(TEXT("Add Vertices"), JR_MARCHINGCUBES_VERTS, STATGROUP_JR)
			vertIDs[i0] = -1;
			if (!vertEnabled[i0]) continue;

			if (Settings.ReuseVertices) {
				//new method of checking if vertex exists for a given edge
				int* existingID = existingEdges.Find(edges[i0]);
				if (existingID != nullptr) {
					vertIDs[i0] = *existingID;
					continue;
				}
			}

			std::pair<int, int> vertexIndices = edgeVertices[i0];
			int newID = EditMesh->AppendVertex(FVector3d(VertexInterp(points[vertexIndices.first],points[vertexIndices.second], values[vertexIndices.first], values[vertexIndices.second])));
			vertIDs[i0] = newID;
			existingEdges.Add(edges[i0], newID);
				
			
		}
		//append triangles to mesh
		for (int i = 0; triTable[cubeCase][i] != -1; i += 3) {
			DECLARE_SCOPE_CYCLE_COUNTER(TEXT("Add Triangles"), JR_MARCHINGCUBES_Tris, STATGROUP_JR)
			EditMesh->AppendTriangle(vertIDs[triTable[cubeCase][i]], vertIDs[triTable[cubeCase][i + 2]], vertIDs[triTable[cubeCase][i + 1]]);
		}

	cubeSkip: //label for nested loop that needs to propagate to neighbours
		{
			DECLARE_SCOPE_CYCLE_COUNTER(TEXT("Add new positions"), JR_MARCHINGCUBES_ADDPOSITIONS, STATGROUP_JR)
			//search all neighbouring positions that havent been visited yet
			for (FIntVector3 offset : OrthoOffsets) {
				FIntVector3 newPos = pos + offset;
				bool add = true;
				{
					DECLARE_SCOPE_CYCLE_COUNTER(TEXT("Check position visited"), JR_MARCHINGCUBES_CHECKVISITED, STATGROUP_JR)
					add = !visited.Contains(newPos);
				}
				if (add) {
					frontier.Add(newPos);
				}
			}

		}
	cubeDiscard:;//label for nested loop that needs to NOT propagate to neighbours
	}
}


float UGeoFlowAsset::SampleSDF(FVector3f pos)
{ 
	DECLARE_SCOPE_CYCLE_COUNTER(TEXT("SDF Evaluate"),JR_MARCHINGCUBES_EVAL,STATGROUP_JR)
	INC_DWORD_STAT(JR_EVAL_CALLS)
	double res = _outputNode->Evaluate(pos);
	if (res <= 0.0) {
		INC_DWORD_STAT(JR_EVAL_SUCCESSES)
	}
	return res;
}

FVector3f UGeoFlowAsset::VertexInterp(FVector3f aPos, FVector3f bPos, float aVal, float bVal)
{
	float frac = aVal / (bVal - aVal);
	frac = FMath::Abs(frac);
	return FMath::Lerp(aPos, bPos, frac);
}



