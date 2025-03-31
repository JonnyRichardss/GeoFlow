// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/DynamicMeshComponent.h"
#include "GeoFlowAsset.h"
#include "GeoFlowComponent.generated.h"


/**
 *
 */
UCLASS(hidecategories = (LOD), meta = (BlueprintSpawnableComponent), ClassGroup = GeoFlow)
class GEOFLOWCORE_API UGeoFlowComponent : public UDynamicMeshComponent
{
public:
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category = "GeoFlow", DisplayName = "Auto-Update");
	bool AutoUpdate = true;

	UPROPERTY(EditAnywhere,Category = "GeoFlow",DisplayName="GeoFlow Asset")
	UGeoFlowAsset* geo;
	UGeoFlowComponent();
	virtual ~UGeoFlowComponent();
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	//regenerate the mesh from the graph asset
	UFUNCTION(BlueprintCallable,CallInEditor, Category = "GeoFlow")
	void Regen();
	//forces regeneration whether graph has been updated or not
	UFUNCTION(BlueprintCallable)
	void ForceRegen();
	//forces geometry to clear
	UFUNCTION(BlueprintCallable)
	void ForceClear();
};
