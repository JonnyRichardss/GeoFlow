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
void UGeoFlowAsset::Generate(UGeoFlowComponent* parent)
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
		}
		//cant check node type as its overwritten on derived
		UGFN_R_BasePrimitive* primitive = Cast<UGFN_R_BasePrimitive>(node);
		if (IsValid(primitive)) {
			PrimitivePositions.Add(primitive->GetPosition());
		}
	}
	TArray<FIntVector3> primitiveOffsets;
	FVector3f averagePos;
	for (FVector3f position : PrimitivePositions) {
		averagePos += position;
	}
	averagePos /= (float)PrimitivePositions.Num();

	for (FVector3f position : PrimitivePositions) {
		FVector3f offset = position - averagePos;
		offset /= parent->GenSettings.stepSize;
		primitiveOffsets.Emplace(FIntVector3(FMath::RoundToInt(offset.X), FMath::RoundToInt(offset.Y), FMath::RoundToInt(offset.Z)));
	}
	DoMarchingCubes(EditMesh, parent->GenSettings, averagePos,primitiveOffsets);

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
}static FIntVector3 cornerOffsets[8] = { {-1,-1,-1},{-1,1,-1},{1,1,-1},{1,-1,-1},{-1,-1,1},{-1,1,1},{1,1,1},{1,-1,1} };
void UGeoFlowAsset::DoMarchingCubes(UE::Geometry::FDynamicMesh3* EditMesh, const FGeoFlowGenerationSettings& settings, FVector3f objectCentre,TArray<FIntVector3> primitiveOffsets)
{

	DECLARE_SCOPE_CYCLE_COUNTER(TEXT("MarchingCubes"), JR_MARCHINGCUBES_FULL, STATGROUP_JR)
	SET_DWORD_STAT(JR_EVAL_CALLS, 0)
	SET_DWORD_STAT(JR_EVAL_SUCCESSES, 0)
	SET_DWORD_STAT(JR_TOTAL_CUBES, 0)
	auto stepSize = settings.stepSize;

	double cubeDiagonalSize = FMath::Sqrt(2 * (2 * stepSize)) + stepSize;
	TArray<FIntVector3> frontier;
	TArray<FIntVector3> visited;

	frontier.Append(primitiveOffsets); //start at centre of all primitives
	//vaguely
	//https://paulbourke.net/geometry/polygonise/
	while (!frontier.IsEmpty()){
		INC_DWORD_STAT(JR_TOTAL_CUBES)

		FIntVector3 pos = frontier.Pop();
		visited.Add(pos);

		double x = (objectCentre.X) + (pos.X * stepSize);
		double y = (objectCentre.Y) + (pos.Y * stepSize);
		double z = (objectCentre.Z) + (pos.Z * stepSize);
		FVector3f points[8];
		float values[8];
		int cubeCase = 0;
		FVector3f vertices[12];
		bool vertEnabled[12] = { false };
		int vertIDs[12];

		for (int idx = 0; idx < 8; idx++) {
			int j = cornerOffsets[idx].X;
			int k = cornerOffsets[idx].Y;
			int l = cornerOffsets[idx].Z;
			points[idx] = FVector3f(x + (j * (stepSize / 2.0)), y + (k * (stepSize / 2.0)), z + (l * (stepSize / 2.0)));
		}

		//get all points
		for (int idx = 0; idx < 8; idx++) {
			float sample = SampleSDF(points[idx]);
			//nested-loop means goto is needed to continue twice
			if (sample > cubeDiagonalSize) {
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
		
			
		//this stinks but the points are specific to each case so its harder to for loop
		//sure its fixable (TODO)
		//determine vertex positions for cube case
		if (edgeTable[cubeCase] & 1)
		{
			vertices[0] = VertexInterp(points[0], points[1], values[0], values[1]);
			vertEnabled[0] = true;
		}
		if (edgeTable[cubeCase] & 2)
		{
			vertices[1] = VertexInterp(points[1], points[2], values[1], values[2]);
			vertEnabled[1] = true;
		}
		if (edgeTable[cubeCase] & 4)
		{
			vertices[2] = VertexInterp(points[2], points[3], values[2], values[3]);
			vertEnabled[2] = true;
		}
		if (edgeTable[cubeCase] & 8)
		{
			vertices[3] = VertexInterp(points[3], points[0], values[3], values[0]);
			vertEnabled[3] = true;
		}
		if (edgeTable[cubeCase] & 16)
		{
			vertices[4] = VertexInterp(points[4], points[5], values[4], values[5]);
			vertEnabled[4] = true;
		}
		if (edgeTable[cubeCase] & 32)
		{
			vertices[5] = VertexInterp(points[5], points[6], values[5], values[6]);
			vertEnabled[5] = true;
		}
		if (edgeTable[cubeCase] & 64)
		{
			vertices[6] = VertexInterp(points[6], points[7], values[6], values[7]);
			vertEnabled[6] = true;
		}
		if (edgeTable[cubeCase] & 128)
		{
			vertices[7] = VertexInterp(points[7], points[4], values[7], values[4]);
			vertEnabled[7] = true;
		}
		if (edgeTable[cubeCase] & 256)
		{
			vertices[8] = VertexInterp(points[0], points[4], values[0], values[4]);
			vertEnabled[8] = true;
		}
		if (edgeTable[cubeCase] & 512)
		{
			vertices[9] = VertexInterp(points[1], points[5], values[1], values[5]);
			vertEnabled[9] = true;
		}
		if (edgeTable[cubeCase] & 1024)
		{
			vertices[10] = VertexInterp(points[2], points[6], values[2], values[6]);
			vertEnabled[10] = true;
		}
		if (edgeTable[cubeCase] & 2048)
		{
			vertices[11] = VertexInterp(points[3], points[7], values[3], values[7]);
			vertEnabled[11] = true;
		}
		
		//append vertices to mesh
		for (int i = 0; i < 12; i++) {
			vertIDs[i] = -1;
			if (vertEnabled[i]) {
				vertIDs[i] = EditMesh->AppendVertex(FVector3d(vertices[i]));

			}
		}

		//append triangles to mesh
		for (int i = 0; triTable[cubeCase][i] != -1; i += 3) {
			EditMesh->AppendTriangle(vertIDs[triTable[cubeCase][i]], vertIDs[triTable[cubeCase][i + 2]], vertIDs[triTable[cubeCase][i + 1]]);
		}

	cubeSkip:
		//search all neighbouring positions that havent been visited yet
		for (int i = pos.X-1; i < pos.X+2; i++) {
			for (int j = pos.Y - 1; j < pos.Y + 2; j++) {
				for (int k = pos.Z - 1; k < pos.Z + 2; k++) {
					FIntVector3 newPos(i, j, k);
					if (!visited.Contains(newPos)) {
						frontier.Add(newPos);
					}
				}
			}
		}
	cubeDiscard:;
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

FString UGeoFlowAsset::MakeShaderSource()
{
	FString Output;
	TArray<FString> PinDeclarations;
	FString endingCode = _outputNode->CreateShaderEvalCall(PinDeclarations);
	for (FString& dec : PinDeclarations) {
		Output.Append(dec);
	}
	Output.Append(endingCode);
	return Output;
}

