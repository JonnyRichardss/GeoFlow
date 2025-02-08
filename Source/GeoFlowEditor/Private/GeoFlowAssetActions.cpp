#include "GeoFlowAssetActions.h"
#include "GeoFlowApp.h"
#include "GeoFlowAsset.h"
UGeoFlowAssetFactory::UGeoFlowAssetFactory(const FObjectInitializer& objectInitializer) : Super(objectInitializer)
{
	SupportedClass = UGeoFlowAsset::StaticClass();
}

UObject* UGeoFlowAssetFactory::FactoryCreateNew(UClass* uclass, UObject* inParent, FName name, EObjectFlags flags, UObject* context, FFeedbackContext* warn)
{
	return NewObject<UGeoFlowAsset>(inParent, name, flags);
}

bool UGeoFlowAssetFactory::CanCreateNew() const
{
	return true;
}



GeoFlowAssetAction::GeoFlowAssetAction(EAssetTypeCategories::Type category)
{
	_assetCategory = category;
}

FText GeoFlowAssetAction::GetName() const
{
	return FText::FromString(TEXT("GeoFlow Graph"));
}

FColor GeoFlowAssetAction::GetTypeColor() const
{
	return FColor::Cyan;
}

UClass* GeoFlowAssetAction::GetSupportedClass() const
{
	return UGeoFlowAsset::StaticClass();
}

void GeoFlowAssetAction::OpenAssetEditor(const TArray<UObject*>& inObjects, TSharedPtr<class IToolkitHost> editWithinLevelEditor)
{
	EToolkitMode::Type mode = editWithinLevelEditor.IsValid() ? EToolkitMode::WorldCentric : EToolkitMode::Standalone;
	for (UObject* object : inObjects) {
		UGeoFlowAsset* asset = Cast<UGeoFlowAsset>(object);
		if (asset != nullptr) {
			TSharedRef<GeoFlowEditorApp> editor(new GeoFlowEditorApp());
			editor->InitEditor(mode, editWithinLevelEditor, asset);
		}
	}
}

uint32 GeoFlowAssetAction::GetCategories()
{
	return _assetCategory;
}
