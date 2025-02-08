#pragma once
#include "CoreMinimal.h"
#include "AssetTypeActions_Base.h"

#include "GeoFlowAssetActions.generated.h"

UCLASS()
class UGeoFlowAssetFactory : public UFactory
{
	GENERATED_BODY()
public:
	UGeoFlowAssetFactory(const FObjectInitializer& objectInitializer);
public: //UFactory interface
	virtual UObject* FactoryCreateNew(UClass* uclass, UObject* inParent, FName name, EObjectFlags flags, UObject* context, FFeedbackContext* warn) override;
	virtual bool CanCreateNew() const override;
};




class GeoFlowAssetAction : public FAssetTypeActions_Base {
public:
	GeoFlowAssetAction(EAssetTypeCategories::Type category);
public: //FAssetTypeActions_Base interface
	virtual FText GetName() const override;
	virtual FColor GetTypeColor() const override;
	virtual UClass* GetSupportedClass() const override;
	virtual void OpenAssetEditor(const TArray<UObject*>& inObjects, TSharedPtr<class IToolkitHost> editWithinLevelEditor = TSharedPtr<IToolkitHost>()) override;
	virtual uint32 GetCategories() override;
private:
	EAssetTypeCategories::Type _assetCategory;
};
