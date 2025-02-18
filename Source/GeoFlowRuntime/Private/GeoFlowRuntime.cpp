// Copyright Epic Games, Inc. All Rights Reserved.

#include "GeoFlowRuntime.h"
#include "Interfaces/IPluginManager.h"
#define LOCTEXT_NAMESPACE "FGeoFlowRuntimeModule"

void FGeoFlowRuntimeModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	FString PluginShaderDir = FPaths::Combine(IPluginManager::Get().FindPlugin(TEXT("GeoFlow"))->GetBaseDir(), TEXT("Shaders/Private"));
	AddShaderSourceDirectoryMapping(TEXT("/GeoFlowShaders"), PluginShaderDir);
}

void FGeoFlowRuntimeModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FGeoFlowRuntimeModule, GeoFlowRuntime)