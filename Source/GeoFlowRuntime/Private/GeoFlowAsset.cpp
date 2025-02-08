// Fill out your copyright notice in the Description page of Project Settings.

#include "GeoFlowAsset.h"
#include "GeoFlowComponent.h"
#include "DynamicMesh/MeshAttributeUtil.h"
#include "DynamicMesh/MeshNormals.h"
#include "GeoFlowNodeTypes.h"
#include "TriangulationTable.h"

//#include "IcosphereGenerator.h"
void UGeoFlowAsset::Generate(UGeoFlowComponent* parent)
{
	//TODO - pass Marching cubes settings thru from the component
	if (parent == nullptr) {
		return;
	}
	if (lastWorkingGraph != nullptr) {
		SaveFromGraph(lastWorkingGraph);
	}
	FDynamicMesh3* EditMesh = parent->GetMesh();


	for (UGFN_R_Base* node : Graph->Nodes) {
		if (node->NodeType() == EGeoFlowNodeType::Output) {
			_outputNode = Cast<UGFN_R_Output>(node);
			break;
		}
	}
	DoMarchingCubes(EditMesh,parent->MCSettings);

	parent->NotifyMeshUpdated();
}
void UGeoFlowAsset::SaveFromGraph(UEdGraph* _workingGraph)
{
	lastWorkingGraph = _workingGraph;
	UGeoFlowRuntimeGraph* runtimeGraph = NewObject<UGeoFlowRuntimeGraph>(this);
	Graph = runtimeGraph;

	TArray<std::pair<FGuid, FGuid>> connections;
	TMap < FGuid, UGeoFlowRuntimePin*> idToPinMap;

	for (UEdGraphNode* uiNode : _workingGraph->Nodes) {
		UGFN_E_Base* node = Cast<UGFN_E_Base>(uiNode);
		UGFN_R_Base* runtimeNode = node->CreateRuntimeNode(runtimeGraph, connections, idToPinMap);
		runtimeGraph->Nodes.Add(runtimeNode);
	}

	for (std::pair<FGuid, FGuid> connection : connections) {
		UGeoFlowRuntimePin* p1 = idToPinMap[connection.first];
		UGeoFlowRuntimePin* p2 = idToPinMap[connection.second];
		p1->Connection = p2;
	}
}
void UGeoFlowAsset::LoadToGraph(UEdGraph* _workingGraph)
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
void UGeoFlowAsset::DoMarchingCubes(UE::Geometry::FDynamicMesh3* EditMesh,const FGeoFlowMarchingCubesSettings& settings)
{
	auto boundingRadius = settings.boundingRadius;
	auto stepSize = settings.stepSize;
	auto objectCentre = settings.objectCentre;
	//vaguely
	//https://paulbourke.net/geometry/polygonise/
	int numSteps = (boundingRadius * 2) / stepSize;
	for (int xi = 0; xi < numSteps; xi++) {
		for (int yi = 0; yi < numSteps; yi++) {
			for (int zi = 0; zi < numSteps; zi++) {
				double x = (objectCentre.X - boundingRadius) + (xi*stepSize);
				double y = (objectCentre.Y - boundingRadius) + (yi*stepSize);
				double z = (objectCentre.Z - boundingRadius) + (zi*stepSize);
				TArray<FVector3d> points;
				points.Init(FVector3d::Zero(), 8);
				double values[8];

				TArray<FVector3d> vertices;
				vertices.Init(FVector3d::Zero(), 12);
				bool vertEnabled[12] = { false };
				int vertIDs[12];

				//get all points
				FIntVector3 cornerOffsets[8] = { {-1,-1,-1},{-1,1,-1},{1,1,-1},{1,-1,-1},{-1,-1,1},{-1,1,1},{1,1,1},{1,-1,1} };
				for (int idx = 0; idx < 8; idx++) {
					int j = cornerOffsets[idx].X;
					int k = cornerOffsets[idx].Y;
					int l = cornerOffsets[idx].Z;
					FVector3d pos = FVector3d(x + (j * (stepSize / 2.0)), y + (k * (stepSize / 2.0)), z + (l * (stepSize / 2.0)));
					points[idx] = pos;
					values[idx] = SampleSDF(pos);
				}
						
				//use point samples to determine cube case
				int cubeCase = 0;
				for (int i = 0; i < 8; i++) {
					if (values[i] <= 0.0) {
						cubeCase |= 1 << i;
					}
				}
				//this stinks but the points are specific to each case so its harder to for loop
				//sure its fixable (TODO)
				if (edgeTable[cubeCase] == 0)
					continue;

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
						vertIDs[i] = EditMesh->AppendVertex(vertices[i]);
					}
				}

				//append triangles to mesh
				for (int i = 0; triTable[cubeCase][i] != -1; i += 3) {
					EditMesh->AppendTriangle(vertIDs[triTable[cubeCase][i]],  vertIDs[triTable[cubeCase][i + 2]], vertIDs[triTable[cubeCase][i + 1]]);
				}
			}
		}
	}

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
}

static double smin(double a, double b, double k)
{
	double h = FMath::Clamp(0.5 + 0.5 * (b - a) / k, 0.0, 1.0);
	return FMath::Lerp(b, a, h) - k * h * (1.0 - h);
}

double UGeoFlowAsset::SampleSDF(FVector3d pos)
{ 
	return _outputNode->Evaluate(pos);
}
FVector3d UGeoFlowAsset::GetSDFNormal(FVector3d pos)
{
	double d = 0.01;
	FVector3d output = FVector3d(
		SampleSDF(FVector3d(pos.X + d, pos.Y, pos.Z)) - SampleSDF(FVector3d(pos.X - d, pos.Y, pos.Z)) / (2.0 * d),
		SampleSDF(FVector3d(pos.X, pos.Y + d, pos.Z)) - SampleSDF(FVector3d(pos.X, pos.Y - d, pos.Z)) / (2.0 * d),
		SampleSDF(FVector3d(pos.X, pos.Y, pos.Z + d)) - SampleSDF(FVector3d(pos.X, pos.Y, pos.Z - d)) / (2.0 * d)
	);
	output.Normalize();
	return output;
}

 
FVector3d UGeoFlowAsset::VertexInterp(FVector3d aPos, FVector3d bPos, double aVal, double bVal)
{
	double frac = aVal / (bVal - aVal);
	frac = FMath::Abs(frac);
	return FMath::Lerp(aPos, bPos, frac);
}

