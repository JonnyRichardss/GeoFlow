// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GeoFlowRuntimeGraph.h"
#include "UObject/NoExportTypes.h"
#include "GeoFlowAsset.generated.h"

namespace UE {
	namespace Geometry {
		class FDynamicMesh3;
	}
};
class UGeoFlowComponent;
class UGFN_R_Output;
USTRUCT()
struct GEOFLOWRUNTIME_API FGeoFlowGenerationSettings {
	GENERATED_BODY()
	UPROPERTY(EditAnywhere,DisplayName="Generation Offset")
	FVector3d objectCentre;
	UPROPERTY(EditAnywhere,DisplayName="Generation Size")
	double boundingRadius = 50;
	UPROPERTY(EditAnywhere,DisplayName="Resolution")
	double stepSize = 5;

};
USTRUCT()
struct GEOFLOWRUNTIME_API FGeoFlowFrustumSettings {
	GENERATED_BODY()
	UPROPERTY(EditAnywhere)
	int numStepsW = 100;
	UPROPERTY(EditAnywhere)
	int Divisions = 10;
	UPROPERTY(EditAnywhere)
	double DistanceScaling = 1.0;

};
UCLASS(BlueprintType)
class GEOFLOWRUNTIME_API UGeoFlowAsset : public UObject
{
	GENERATED_BODY()
public:
	void Generate(UGeoFlowComponent* parent);
	void GenFrustum(UGeoFlowComponent* parent);
	void GenGPU(UGeoFlowComponent* parent);
	void GenFrustumGPU(UGeoFlowComponent* parent);
	UPROPERTY()
	UGeoFlowRuntimeGraph* Graph = nullptr;
	//when loaded - *guarantee* that last generate was BEFORE last graph change
	FDateTime LastGraphChange = INT64_MAX;
	FDateTime LastGenerate = 0;
	void SaveFromEditor(UEdGraph* _workingGraph);
	void LoadToEditor(UEdGraph* _workingGraph);
	bool NeedsResave();
	UEdGraph* lastWorkingGraph;
	FString MakeShaderSource();
protected:
	
	void DoMarchingCubes(UE::Geometry::FDynamicMesh3* EditMesh, const FGeoFlowGenerationSettings& settings);
	void MarchThroughFrustum(UE::Geometry::FDynamicMesh3* EditMesh, UGeoFlowComponent* parent);
	void MakePoints(TArray<FVector3f>& points, UGeoFlowComponent* parent);
	void FrustumMakePoints(TArray<FVector3f>& points, UGeoFlowComponent* parent);
	float SampleSDF(FVector3f pos);
	FVector3f GetSDFNormal(FVector3f pos);
	FVector3f VertexInterp(FVector3f aPos, FVector3f bPos, float aVal, float bVal);
	UGFN_R_Output* _outputNode;
	bool GpuRunning = false;

	
	void SampleCube(FVector3f points[8], float cubeDiagonalSize, UE::Geometry::FDynamicMesh3* EditMesh);
	void CubeFromSamples(FVector3f points[8], float values[8], UE::Geometry::FDynamicMesh3* EditMesh);
	TArray<FVector3f> AllPoints;
	TArray<float> Vals;

	//binned DC for now due to complex linear algebra requirements
	//int DualContouringSample(UE::Geometry::FDynamicMesh3* EditMesh, FVector3d pos);
	//void DoDualContouring(UE::Geometry::FDynamicMesh3* EditMesh, const FGeoFlowGenerationSettings& settings);
};
