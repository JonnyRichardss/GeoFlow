// Fill out your copyright notice in the Description page of Project Settings.

#include "GeoFlowAsset.h"
#include "GeoFlowComponent.h"
#include "DynamicMesh/MeshAttributeUtil.h"
#include "DynamicMesh/MeshNormals.h"
#include "GeoFlowNodeTypes.h"
#include "TriangulationTable.h"

DECLARE_STATS_GROUP(TEXT("JR_MarchingProfiling"),STATGROUP_JR,STATCAT_JR)
DECLARE_DWORD_ACCUMULATOR_STAT(TEXT("JR_LastEvaluateCalls"),JR_EVAL_CALLS,STATGROUP_JR)
DECLARE_DWORD_ACCUMULATOR_STAT(TEXT("JR_LastSamplesInsideObject"),JR_EVAL_SUCCESSES,STATGROUP_JR)
//#include "IcosphereGenerator.h"
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

	for (UGFN_R_Base* node : Graph->Nodes) {
		if (node->NodeType() == EGeoFlowNodeType::Output) {
			_outputNode = Cast<UGFN_R_Output>(node);
			break;
		}
	}
	DoMarchingCubes(EditMesh,parent->GenSettings);

	parent->NotifyMeshUpdated();
	LastGenerate = FDateTime::UtcNow();
}
void UGeoFlowAsset::GenFrustum(UGeoFlowComponent* parent)
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

	for (UGFN_R_Base* node : Graph->Nodes) {
		if (node->NodeType() == EGeoFlowNodeType::Output) {
			_outputNode = Cast<UGFN_R_Output>(node);
			break;
		}
	}
	MarchThroughFrustum(EditMesh, parent);

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
static constexpr double safetyMargin = 0.0;
void UGeoFlowAsset::DoMarchingCubes(UE::Geometry::FDynamicMesh3* EditMesh,const FGeoFlowGenerationSettings& settings)
{
	
	DECLARE_SCOPE_CYCLE_COUNTER(TEXT("MarchingCubes"),JR_MARCHINGCUBES_FULL,STATGROUP_JR)
	SET_DWORD_STAT(JR_EVAL_CALLS,0)
	SET_DWORD_STAT(JR_EVAL_SUCCESSES,0)
	auto boundingRadius = settings.boundingRadius;
	auto stepSize = settings.stepSize;
	auto objectCentre = settings.objectCentre;
	int numSteps = (boundingRadius * 2) / stepSize;

	double cubeDiagonalSize = FMath::Sqrt(2 * (2 * stepSize)) + stepSize;
	//vaguely
	//https://paulbourke.net/geometry/polygonise/
	for (int xi = 0; xi < numSteps; xi++) {
		for (int yi = 0; yi < numSteps; yi++) {
			for (int zi = 0; zi < numSteps; zi++) {
				double x = (objectCentre.X - boundingRadius) + (xi * stepSize);
				double y = (objectCentre.Y - boundingRadius) + (yi * stepSize);
				double z = (objectCentre.Z - boundingRadius) + (zi * stepSize);
				FVector3d points[8];
				FIntVector3 cornerOffsets[8] = { {-1,-1,-1},{-1,1,-1},{1,1,-1},{1,-1,-1},{-1,-1,1},{-1,1,1},{1,1,1},{1,-1,1} };
				for (int idx = 0; idx < 8; idx++) {
					int j = cornerOffsets[idx].X;
					int k = cornerOffsets[idx].Y;
					int l = cornerOffsets[idx].Z;
					points[idx] = FVector3d(x + (j * (stepSize / 2.0)), y + (k * (stepSize / 2.0)), z + (l * (stepSize / 2.0)));
				}
				SampleCube(points, cubeDiagonalSize, EditMesh);

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
void UGeoFlowAsset::MarchThroughFrustum(UE::Geometry::FDynamicMesh3* EditMesh, UGeoFlowComponent* parent)
{
	UWorld* World = parent->GetWorld();
	if (!IsValid(World)) return;
	APlayerCameraManager* PlayerCamera = World->GetFirstPlayerController()->PlayerCameraManager;
	if (!IsValid(PlayerCamera)) return;
	FMinimalViewInfo view = PlayerCamera->GetCameraCacheView();
	FVector3d startPos = PlayerCamera->GetCameraLocation() - parent->GetComponentLocation();
	FRotator worldToCameraSpace = PlayerCamera->GetCameraRotation();
	FVector3d wDir = worldToCameraSpace.RotateVector(FVector::ForwardVector);
	
	FVector3d pos = startPos;
	double w = view.GetFinalPerspectiveNearClipPlane();
	double lastW = 0;
	for (int stepsW = 0; stepsW < parent->FrustumGen.numStepsW; stepsW++) {
		pos = startPos + (wDir * w);
		
		double viewWidth = FMath::Tan(view.FOV / 2.0) * 2.0 * w;
		double viewHeight = viewWidth / view.AspectRatio;
		double stepWidthX = viewWidth / (double)parent->FrustumGen.Divisions;
		double stepWidthY = stepWidthX / view.AspectRatio;
		FVector3d cornerPos = pos + worldToCameraSpace.RotateVector(FVector3d(0.0,0.0-(viewWidth / 2.0), 0.0-(viewHeight / 2.0)));
		FVector3d otherCorner = pos - worldToCameraSpace.RotateVector(FVector3d(0.0,0.0-(viewWidth / 2.0), 0.0-(viewHeight / 2.0)));
		/*
		if (stepsW > 10 && stepsW < 30) {
			DrawDebugSphere(World, pos, 10.0, 10, FColor::Red);
			DrawDebugSphere(World, cornerPos, 10.0, 10, FColor::Blue);
			DrawDebugSphere(World, otherCorner, 10.0, 10, FColor::Blue);
		}
		*/
		for (int i = 0; i < parent->FrustumGen.Divisions; i++) {
			for (int j = 0; j < parent->FrustumGen.Divisions; j++) {
				FVector3d samplePos = cornerPos + worldToCameraSpace.RotateVector(FVector3d(0.0,stepWidthX * i, stepWidthY * j));

				//probably can un-manual this a bit
				FVector3d points[8];
				points[0] = samplePos + worldToCameraSpace.RotateVector(FVector3d(0.0 - (lastW / 2),0.0 - (stepWidthX / 2.0), 0.0 - (stepWidthY / 2.0)));//{-1,-1,-1}
				points[1] = samplePos + worldToCameraSpace.RotateVector(FVector3d(0.0 - (lastW / 2),0.0 - (stepWidthX / 2.0),(stepWidthY / 2.0)		  ));//{-1,1,-1}
				points[2] = samplePos + worldToCameraSpace.RotateVector(FVector3d(0.0 - (lastW / 2),(stepWidthX / 2.0)	  , (stepWidthY / 2.0)		  ));//{1,1,-1}
				points[3] = samplePos + worldToCameraSpace.RotateVector(FVector3d(0.0 - (lastW / 2),(stepWidthX / 2.0)	  , 0.0 - (stepWidthY / 2.0)  ));//{1,-1,-1}
																								   
																								   
				lastW = FMath::Max(stepWidthX, lastW);											   
				points[4] = samplePos + worldToCameraSpace.RotateVector(FVector3d(		(lastW / 2),0.0 - (stepWidthX / 2.0), 0.0 - (stepWidthY / 2.0)));//{-1,-1,1}
				points[5] = samplePos + worldToCameraSpace.RotateVector(FVector3d(		(lastW / 2),0.0 - (stepWidthX / 2.0),(stepWidthY / 2.0)		  ));//{-1,1,1}
				points[6] = samplePos + worldToCameraSpace.RotateVector(FVector3d(		(lastW / 2),(stepWidthX / 2.0)	  , (stepWidthY / 2.0)		  ));//{1,1,1}
				points[7] = samplePos + worldToCameraSpace.RotateVector(FVector3d(		(lastW / 2),(stepWidthX / 2.0)	  , 0.0 - (stepWidthY / 2.0)  ));//{1,-1,1}
				/*
				if (stepsW ==15 || stepsW == 16) {
					if (i == parent->FrustumGen.Divisions / 2 && j == parent->FrustumGen.Divisions / 2 || i == (parent->FrustumGen.Divisions / 2) +1 && j == parent->FrustumGen.Divisions / 2) {
						DrawDebugSphere(World, samplePos, 1.0, 5, FColor::Yellow);
						for (int pointidx = 0; pointidx < 8; pointidx++) {
							DrawDebugLine(World, samplePos, points[pointidx], FColor::Red, false, -1.0, 0u, 0.5f);
							DrawDebugSphere(World, points[pointidx], 1.0, 5, FColor::Green);
						}
					}
				}
				*/
				SampleCube(points, FMath::Sqrt(2 * (2 * stepWidthX)) + stepWidthX, EditMesh);
			}
		}
		
		w += lastW;
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
void UGeoFlowAsset::SampleCube(FVector3d points[8], double cubeDiagonalSize, UE::Geometry::FDynamicMesh3* EditMesh)
{
	int cubeCase = 0;

	double values[8];
	FVector3d vertices[12];
	bool vertEnabled[12] = { false };
	int vertIDs[12];

	//get all points
	for (int idx = 0; idx < 8; idx++) {
		double sample = SampleSDF(points[idx]);
		if (FMath::Abs(sample) > cubeDiagonalSize) {
			return;
		}
		values[idx] = sample;
	}

	//use point samples to determine cube case

	for (int i = 0; i < 8; i++) {
		if (values[i] <= 0.0) {
			cubeCase |= 1 << i;
		}
	}
	//this stinks but the points are specific to each case so its harder to for loop
	//sure its fixable (TODO)
	if (edgeTable[cubeCase] == 0)
	{
		return;
	};

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
		EditMesh->AppendTriangle(vertIDs[triTable[cubeCase][i]], vertIDs[triTable[cubeCase][i + 2]], vertIDs[triTable[cubeCase][i + 1]]);
	}

}

/*
int UGeoFlowAsset::DualContouringSample(UE::Geometry::FDynamicMesh3* EditMesh, FVector3d pos, double stepSize, double cubeDiagonalSize) {
	FVector3d points[8];
	double values[8];
		
	FIntVector3 cornerOffsets[8] = { {-1,-1,-1},{-1,1,-1},{1,1,-1},{1,-1,-1},{-1,-1,1},{-1,1,1},{1,1,1},{1,-1,1} };

	for (int idx = 0; idx < 8; idx++) {
		int j = cornerOffsets[idx].X;
		int k = cornerOffsets[idx].Y;
		int l = cornerOffsets[idx].Z;
		FVector3d pos = FVector3d(pos.X + (j * (stepSize / 2.0)), pos.Y + (k * (stepSize / 2.0)), pos.Z + (l * (stepSize / 2.0)));
		points[idx] = pos;
		double sample = SampleSDF(pos);
		if (FMath::Abs(sample) > cubeDiagonalSize) {
			return -1;
		}
		values[idx] = sample;
	}
	TArray<FVector3d> changeLocations;

	// Z AXIS CHANGE -- edges 0,4 1,5 2,6 3,7
	for (int i = 0; i < 4; i++) {
		//change of sign on edge
		if ((values[i] > 0)  != (values[i + 4] > 0)) {
			//TODO add interp
			changeLocations.Add((points[i] + points[i + 4]) / 2.0);
		}
	}
	// Y AXIS CHANGE -- edges 0,1 2,3 4,5 6,7
	for (int i = 0; i < 7; i += 2) {
		//change of sign on edge
		if ((values[i] > 0) != (values[i + 1] > 0)) {
			//TODO add interp
			changeLocations.Add((points[i] + points[i + 1]) / 2.0);
		}
	}
	// X AXIS CHANGE -- edges 0,3 1,2 4,7 5,6
	//TODO un-manualify this
	if ((values[0] > 0) != (values[3] > 0)) changeLocations.Add((points[0] + points[3]) / 2.0);
	if ((values[1] > 0) != (values[2] > 0)) changeLocations.Add((points[1] + points[2]) / 2.0);
	if ((values[4] > 0) != (values[7] > 0)) changeLocations.Add((points[4] + points[7]) / 2.0);
	if ((values[5] > 0) != (values[6] > 0)) changeLocations.Add((points[5] + points[6]) / 2.0);
	
	//no changes - no vertex
	if (changeLocations.Num() == 0) return -1;
	
	TArray<FVector3d> normals;
	normals.Reserve(changeLocations.Num());
	for (FVector3d change : changeLocations) {
		normals.Add(GetSDFNormal(change));
	}
	// the correct normal is chosen by solving a quadratic error function QEF
	
	FMath::

	return -1;
}
void UGeoFlowAsset::DoDualContouring(UE::Geometry::FDynamicMesh3* EditMesh, const FGeoFlowGenerationSettings& settings)
{
	//methodology based on
	//https://www.boristhebrave.com/2018/04/15/dual-contouring-tutorial/
	auto boundingRadius = settings.boundingRadius;
	auto stepSize = settings.stepSize;
	auto objectCentre = settings.objectCentre;
	int numSteps = (boundingRadius * 2) / stepSize;
	double cubeDiagonalSize = FMath::Sqrt(2 * (2 * stepSize)) + safetyMargin;
	TMap<FIntVector3, int> PosIdxToVidMap;
	for (int xi = 0; xi < numSteps; xi++) {
		for (int yi = 0; yi < numSteps; yi++) {
			for (int zi = 0; zi < numSteps; zi++) {
				double x = (objectCentre.X - boundingRadius) + (xi * stepSize);
				double y = (objectCentre.Y - boundingRadius) + (yi * stepSize);
				double z = (objectCentre.Z - boundingRadius) + (zi * stepSize);
				




			}
		}
	}


}
*/

double UGeoFlowAsset::SampleSDF(FVector3d pos)
{ 
	DECLARE_SCOPE_CYCLE_COUNTER(TEXT("SDF Evaluate"),JR_MARCHINGCUBES_EVAL,STATGROUP_JR)
	INC_DWORD_STAT(JR_EVAL_CALLS)
	double res = _outputNode->Evaluate(pos);
	if (res <= 0.0) {
		INC_DWORD_STAT(JR_EVAL_SUCCESSES)
	}
	return res;
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

