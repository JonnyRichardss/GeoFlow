#pragma once
#include "SCommonEditorViewportToolbarBase.h"
#include "SEditorViewport.h"
#include "Editor/AdvancedPreviewScene/Public/AdvancedPreviewScene.h"
#include "GeoFlowComponent.h"

class SGeoFlowEditorViewport : public SEditorViewport, public FGCObject, public ICommonEditorViewportToolbarInfoProvider
{
public:

	SLATE_BEGIN_ARGS(SGeoFlowEditorViewport) {}
	SLATE_END_ARGS()

	UGeoFlowComponent* GeoFlow;
	TSharedPtr<FAdvancedPreviewScene> PreviewScene;

	void AddReferencedObjects(FReferenceCollector& Collector) override { Collector.AddReferencedObject(GeoFlow); }

	//Toolbar
	virtual TSharedRef<class SEditorViewport> GetViewportWidget() override { return SharedThis(this); }
	virtual TSharedPtr<FExtender> GetExtenders() const override { return MakeShareable(new FExtender); }
	virtual void OnFloatingButtonClicked() override{}

	SGeoFlowEditorViewport();
	~SGeoFlowEditorViewport();

	void Construct(const FArguments& InArgs);
	virtual TSharedRef<FEditorViewportClient> MakeEditorViewportClient() override;
	EVisibility GetTransformToolbarVisibility() const;
	void OnFocusViewportToSelection() {}
	TSharedPtr<class FGeoFlowViewportClient> GetViewportClient() { return ViewportClient; }

	TSharedPtr<class FGeoFlowViewportClient> ViewportClient;

protected:
	FText GetTitleText() const { return FText::FromString(TEXT("GeoFlow Preview")); }

	// Inherited via FGCObject
	FString GetReferencerName() const override { return TEXT("GeoFlowViewport"); }
};
class FGeoFlowViewportClient : public FEditorViewportClient {
public:
	TWeakPtr<SGeoFlowEditorViewport> ViewportPtr;
	FGeoFlowViewportClient(const TSharedRef<SGeoFlowEditorViewport>& InEditorViewport, const TSharedRef<FAdvancedPreviewScene>& InPreviewScene);


	FAdvancedPreviewScene* AdvancedPreviewScene;

	UPROPERTY()
	UGeoFlowComponent* GeoFlow;

	UPROPERTY()
	TArray<UPrimitiveComponent*> ActorComponents;
	UPROPERTY()
	virtual void Tick(float DeltaSeconds) override;

	void SetAsset(UGeoFlowAsset* InAsset);

	void Generate();
	void Clear();

	//
	//MakeDebug(type??)
	//DestroyDebug(ref)
};