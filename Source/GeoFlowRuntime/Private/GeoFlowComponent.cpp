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
	if (PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(UGeoFlowComponent, geo)) {
		ForceRegen();
	}
}


void UGeoFlowComponent::ForceRegen()
{
	MeshObject->Reset();
	if (geo != nullptr) {
		geo->Generate(this);
	}
}
