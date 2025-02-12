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

};
UCLASS(BlueprintType)
class GEOFLOWRUNTIME_API UGeoFlowAsset : public UObject
{
	GENERATED_BODY()
public:
	void Generate(UGeoFlowComponent* parent);
	void GenFrustum(UGeoFlowComponent* parent);
	UPROPERTY()
	UGeoFlowRuntimeGraph* Graph = nullptr;
	//when loaded - *guarantee* that last generate was BEFORE last graph change
	FDateTime LastGraphChange = INT64_MAX;
	FDateTime LastGenerate = 0;
	void SaveFromEditor(UEdGraph* _workingGraph);
	void LoadToEditor(UEdGraph* _workingGraph);
	bool NeedsResave();
	UEdGraph* lastWorkingGraph;
protected:
	
	void DoMarchingCubes(UE::Geometry::FDynamicMesh3* EditMesh, const FGeoFlowGenerationSettings& settings);
	void MarchThroughFrustum(UE::Geometry::FDynamicMesh3* EditMesh, UGeoFlowComponent* parent);
	void SampleCube(FVector3d points[8], double cubeDiagonalSize, UE::Geometry::FDynamicMesh3* EditMesh);
	double SampleSDF(FVector3d pos);
	FVector3d GetSDFNormal(FVector3d pos);
	FVector3d VertexInterp(FVector3d aPos, FVector3d bPos, double aVal, double bVal);
	UGFN_R_Output* _outputNode;

	//binned DC for now due to complex linear algebra requirements
	//int DualContouringSample(UE::Geometry::FDynamicMesh3* EditMesh, FVector3d pos);
	//void DoDualContouring(UE::Geometry::FDynamicMesh3* EditMesh, const FGeoFlowGenerationSettings& settings);
};
