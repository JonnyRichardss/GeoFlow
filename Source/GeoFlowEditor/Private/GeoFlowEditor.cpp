// Copyright Epic Games, Inc. All Rights Reserved.

#include "GeoFlowEditor.h"
#include "GeoFlowAssetActions.h"
#include "IAssetTools.h"
#include "AssetToolsModule.h"
#include "Styling/SlateStyleRegistry.h"
#include "Interfaces/IPluginManager.h"

#include "GeoFlowNode.h"
#include "GeoFlowPin.h"

#define LOCTEXT_NAMESPACE "FGeoFlowEditorModule"

//todo move this

void FGeoFlowEditorModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	IAssetTools& assetToolsModule = IAssetTools::Get();
	EAssetTypeCategories::Type assetType = assetToolsModule.RegisterAdvancedAssetCategory(FName(TEXT("GeoFlow")), FText::FromString("Geo Flow"));
	TSharedPtr<GeoFlowAssetAction> assetAction = MakeShareable(new GeoFlowAssetAction(assetType));
	assetToolsModule.RegisterAssetTypeActions(assetAction.ToSharedRef());

	_styleSet = MakeShareable(new FSlateStyleSet(TEXT("GeoFlowEditorStyle")));
	TSharedPtr<IPlugin> plugin = IPluginManager::Get().FindPlugin("GeoFlow");
	FString contentDir = plugin->GetContentDir();
	_styleSet->SetContentRoot(contentDir);
	//TODO replace icons with my own
	FSlateImageBrush* thumbnailBrush = new FSlateImageBrush(_styleSet->RootToContentDir(TEXT("CustomAssetThumbnail"), TEXT(".png")), FVector2D(128.0, 128.0));
	FSlateImageBrush* iconBrush = new FSlateImageBrush(_styleSet->RootToContentDir(TEXT("CustomAssetIcon"), TEXT(".png")), FVector2D(128.0, 128.0));
	FSlateImageBrush* nodeAddPinIcon = new FSlateImageBrush(_styleSet->RootToContentDir(TEXT("NodeAddPinIcon"), TEXT(".png")), FVector2D(128.0, 128.0));
	FSlateImageBrush* nodeDeletePinIcon = new FSlateImageBrush(_styleSet->RootToContentDir(TEXT("NodeDeletePinIcon"), TEXT(".png")), FVector2D(128.0, 128.0));
	FSlateImageBrush* nodeDeleteNodeIcon = new FSlateImageBrush(_styleSet->RootToContentDir(TEXT("NodeDeleteNodeIcon"), TEXT(".png")), FVector2D(128.0, 128.0));
	FSlateImageBrush* HiddenPinIcon = new FSlateImageBrush(_styleSet->RootToContentDir(TEXT("transparentpixel"), TEXT(".png")), FVector2D(1.0, 1.0));
	_styleSet->Set(TEXT("ClassThumbnail.GeoFlowAsset"), thumbnailBrush);
	_styleSet->Set(TEXT("ClassIcon.GeoFlowAsset"), iconBrush);
	_styleSet->Set(TEXT("GeoFlowEditor.NodeAddPinIcon"), nodeAddPinIcon);
	_styleSet->Set(TEXT("GeoFlowEditor.NodeDeletePinIcon"), nodeDeletePinIcon);
	_styleSet->Set(TEXT("GeoFlowEditor.NodeDeleteNodeIcon"), nodeDeleteNodeIcon);
	_styleSet->Set(TEXT("GeoFlowEditor.HiddenPinIcon"), HiddenPinIcon);
	FSlateStyleRegistry::RegisterSlateStyle(*_styleSet);

	_pinFactory = MakeShareable(new FGeoFlowPinFactory());
	_nodeFactory = MakeShareable(new FGeoFlowNodeFactory());
	FEdGraphUtilities::RegisterVisualPinFactory(_pinFactory);
	FEdGraphUtilities::RegisterVisualNodeFactory(_nodeFactory);

}

void FGeoFlowEditorModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	FSlateStyleRegistry::UnRegisterSlateStyle(*_styleSet);
	FEdGraphUtilities::UnregisterVisualPinFactory(_pinFactory);
	FEdGraphUtilities::UnregisterVisualNodeFactory(_nodeFactory);
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FGeoFlowEditorModule, GeoFlowEditor)