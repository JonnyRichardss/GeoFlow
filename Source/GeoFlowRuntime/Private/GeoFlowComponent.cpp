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
	else if (PropertyChangedEvent.GetMemberPropertyName() == GET_MEMBER_NAME_CHECKED(UGeoFlowComponent, GenSettings)) {
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
	if (geo != nullptr && !EnableFrustumGen) {
		if (!UseGPU)
			geo->Generate(this);
		else
			geo->GenGPU(this);
	}
	/*
	if (GpuRunning) return;
	//This is just test code so it dont matter
	//but when i do this for real make sure to have a flag if GPU is already dispatched
	points.Reset();
	points.Init(FVector3f(2,4,5),128);
	FGeoFlowMarchingCubesComputeDispatchParams Params(2, 1, 1,points,Vals);
	GpuRunning = true;
	FGeoFlowMarchingCubesInterface::Dispatch(Params, [this](int errorVal) {
		// OutputVal == 10
		// Called when the results are back from the GPU
		UE_LOG(LogTemp, Warning, TEXT("GPU IS BACK"));
		int a = 1;
		int b = a * 2;
		GpuRunning = false;
		});
	return;
	*/

}

void UGeoFlowComponent::GenFrustum()
{
	MeshObject->Reset();
	if (geo != nullptr && EnableFrustumGen) {
		geo->GenFrustum(this);
	}
}

