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
class UDynamicMeshComponent;
class UGFN_R_Output;
USTRUCT()
struct GEOFLOWCORE_API FGeoFlowGenerationSettings {
	GENERATED_BODY()
	UPROPERTY(EditAnywhere,DisplayName="Resolution")
	double stepSize = 5;
	UPROPERTY(EditAnywhere,DisplayName="Draw Boundary Debug")
	bool debug = false;
	UPROPERTY(EditAnywhere,DisplayName="Reuse common vertices")
	bool ReuseVertices = true;
};
UCLASS(BlueprintType)
class GEOFLOWCORE_API UGeoFlowAsset : public UObject
{
	GENERATED_BODY()
public:
	void Generate(UDynamicMeshComponent* parent);
	UPROPERTY()
	UGeoFlowRuntimeGraph* Graph = nullptr;
	//when loaded - *guarantee* that last generate was BEFORE last graph change
	FDateTime LastGraphChange = 0;
	FDateTime LastGenerate = 0;
	void SaveFromEditor(UEdGraph* _workingGraph);
	void LoadToEditor(UEdGraph* _workingGraph);
	UEdGraph* lastWorkingGraph;
	UPROPERTY(EditAnywhere, DisplayName = "Generation Settings", meta = (FullyExpand = true))
	FGeoFlowGenerationSettings Settings;
protected:
	
	void DoMarchingCubes(UE::Geometry::FDynamicMesh3* EditMesh, UDynamicMeshComponent* parent, TArray<FIntVector3> primitiveOffsets);

	float SampleSDF(FVector3f pos);

	FVector3f VertexInterp(FVector3f aPos, FVector3f bPos, float aVal, float bVal);
	UGFN_R_Output* _outputNode;
};
