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
	FGeoFlowMarchingCubesSettings MCSettings;
	UPROPERTY(EditAnywhere,Category = "GeoFlow",DisplayName="GeoFlow Asset")
	UGeoFlowAsset* geo;
	UGeoFlowComponent();
	virtual ~UGeoFlowComponent();
	void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent);

	UFUNCTION(BlueprintCallable) 
	void ForceRegen();
};
