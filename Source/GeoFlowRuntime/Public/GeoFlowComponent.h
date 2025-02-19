// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/DynamicMeshComponent.h"
#include "GeoFlowAsset.h"
#include "GeoFlowComponent.generated.h"


/**
 *
 */
UCLASS(hidecategories = (LOD), meta = (BlueprintSpawnableComponent), ClassGroup = Rendering)
class GEOFLOWRUNTIME_API UGeoFlowComponent : public UDynamicMeshComponent
{
public:
	GENERATED_BODY()
	UPROPERTY(EditAnywhere,Category = "GeoFlow",DisplayName="Generation Settings", meta = (FullyExpand = true))
	FGeoFlowGenerationSettings GenSettings;

	UPROPERTY(EditAnywhere,Category = "GeoFlow",DisplayName="GeoFlow Asset")
	UGeoFlowAsset* geo;
	UGeoFlowComponent();
	virtual ~UGeoFlowComponent();
	void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent);

	//regenerate the mesh from the graph asset
	UFUNCTION(BlueprintCallable) 
	void Regen();

	//forces regeneration whether graph has been updated or not
	UFUNCTION(BlueprintCallable)
	void ForceRegen();
};
