// Fill out your copyright notice in the Description page of Project Settings.


#include "GeoFlowComponent.h"

UGeoFlowComponent::UGeoFlowComponent()
{

}

UGeoFlowComponent::~UGeoFlowComponent()
{
}

void UGeoFlowComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	if (!AutoUpdate) return;
	if (PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(UGeoFlowComponent, geo)) {
		ForceRegen();
	}
}


void UGeoFlowComponent::Regen()
{
	if (geo != nullptr) {
		//if last generate was earlier than last save OR if one of our nodes is marked as needing to be saved (when pin changes)
		if (geo->LastGenerate < geo->LastGraphChange || geo->NeedsResave() || MeshObject->IsEmpty()) {
			ForceRegen();
		}
	}
}

void UGeoFlowComponent::ForceRegen()
{
	MeshObject->Reset();
	if (geo != nullptr) {
		geo->Generate(this);
	}
}

