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
struct GEOFLOWRUNTIME_API FGeoFlowMarchingCubesSettings {
	GENERATED_BODY()
	UPROPERTY(EditAnywhere,DisplayName="Generation Offset")
	FVector3d objectCentre;
	UPROPERTY(EditAnywhere,DisplayName="Generation Size")
	double boundingRadius = 50;
	UPROPERTY(EditAnywhere,DisplayName="Resolution")
	double stepSize = 5;

};
UCLASS(BlueprintType)
class GEOFLOWRUNTIME_API UGeoFlowAsset : public UObject
{
	GENERATED_BODY()
public:
	void Generate(UGeoFlowComponent* parent);
	UPROPERTY()
	UGeoFlowRuntimeGraph* Graph = nullptr;

	void SaveFromGraph(UEdGraph* _workingGraph);
	void LoadToGraph(UEdGraph* _workingGraph);

	UEdGraph* lastWorkingGraph;
protected:
	
	void DoMarchingCubes(UE::Geometry::FDynamicMesh3* EditMesh, const FGeoFlowMarchingCubesSettings& settings);
	double SampleSDF(FVector3d pos);
	FVector3d GetSDFNormal(FVector3d pos);
	FVector3d VertexInterp(FVector3d aPos, FVector3d bPos, double aVal, double bVal);
	UGFN_R_Output* _outputNode;

};
