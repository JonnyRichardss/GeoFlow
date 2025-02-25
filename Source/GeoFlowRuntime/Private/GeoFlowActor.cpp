#include "GeoFlowActor.h"

AGeoFlowActor::AGeoFlowActor()
{
	PrimaryActorTick.bCanEverTick = true;
	GeoFlow = CreateDefaultSubobject<UGeoFlowComponent>(TEXT("GeoFlow"));
	
}

void AGeoFlowActor::OnConstruction(const FTransform& Transform)
{
	if (GeoFlow->AutoUpdate) {
		GeoFlow->Regen();
	}
}

void AGeoFlowActor::BeginPlay()
{
	Super::BeginPlay();
}

void AGeoFlowActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
