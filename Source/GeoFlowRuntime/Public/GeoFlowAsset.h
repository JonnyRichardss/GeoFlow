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
	UPROPERTY(EditAnywhere,DisplayName="Resolution")
	double stepSize = 5;
	UPROPERTY(EditAnywhere,DisplayName="Vertex Merge Distance")
	double mergeDistance = 0.01;
};
UCLASS(BlueprintType)
class GEOFLOWRUNTIME_API UGeoFlowAsset : public UObject
{
	GENERATED_BODY()
public:
	void Generate(UGeoFlowComponent* parent);
	UPROPERTY()
	UGeoFlowRuntimeGraph* Graph = nullptr;
	//when loaded - *guarantee* that last generate was BEFORE last graph change
	FDateTime LastGraphChange = 0;
	FDateTime LastGenerate = 0;
	void SaveFromEditor(UEdGraph* _workingGraph);
	void LoadToEditor(UEdGraph* _workingGraph);
	bool NeedsResave();
	UEdGraph* lastWorkingGraph;
	FString MakeShaderSource();
protected:
	
	void DoMarchingCubes(UE::Geometry::FDynamicMesh3* EditMesh, const FGeoFlowGenerationSettings& settings,FVector3f ObjectCentre, TArray<FIntVector3> primitiveOffsets);

	float SampleSDF(FVector3f pos);

	FVector3f VertexInterp(FVector3f aPos, FVector3f bPos, float aVal, float bVal);
	UGFN_R_Output* _outputNode;


	TArray<FVector3f> AllPoints;
	TArray<float> Vals;
};
