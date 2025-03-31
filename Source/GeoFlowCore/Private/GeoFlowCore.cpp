// Copyright Epic Games, Inc. All Rights Reserved.

#include "GeoFlowCore.h"
#include "UnrealEd.h"
#include "GeoFlowPreviewVisualization.h"
#define LOCTEXT_NAMESPACE "FGeoFlowCoreModule"

void FGeoFlowCoreModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	if (GUnrealEd != NULL)
	{

		TSharedPtr<FGeoFlowPreviewVisualizer> Visualizer = MakeShareable(new FGeoFlowPreviewVisualizer);
		GUnrealEd->RegisterComponentVisualizer(UGeoFlowComponent::StaticClass()->GetFName(), Visualizer);
		Visualizer->OnRegister();
	}
}

void FGeoFlowCoreModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	if (GUnrealEd != NULL)
	{
		GUnrealEd->UnregisterComponentVisualizer(UGeoFlowComponent::StaticClass()->GetFName());
	}
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FGeoFlowCoreModule, GeoFlowCore)