#include "GeoFlowViewport.h"
#include "Nodes/GeoFlowNodeTypes.h"
#include "Selection.h"
void SGeoFlowEditorViewport::Construct(const FArguments& InArgs)
{
	SEditorViewport::Construct(SEditorViewport::FArguments());
}

TSharedRef<FEditorViewportClient> SGeoFlowEditorViewport::MakeEditorViewportClient()
{
	ViewportClient = MakeShareable(new FGeoFlowViewportClient(SharedThis(this), PreviewScene.ToSharedRef()));
	return ViewportClient.ToSharedRef();
}

EVisibility SGeoFlowEditorViewport::GetTransformToolbarVisibility() const
{
	return EVisibility::Visible;
}


SGeoFlowEditorViewport::SGeoFlowEditorViewport() : PreviewScene(MakeShareable(new FAdvancedPreviewScene(FPreviewScene::ConstructionValues())))
{
	GeoFlow = NewObject<UGeoFlowComponent>();
}

SGeoFlowEditorViewport::~SGeoFlowEditorViewport()
{
}

FGeoFlowViewportClient::FGeoFlowViewportClient(const TSharedRef<SGeoFlowEditorViewport>& InEditorViewport,const TSharedRef<FAdvancedPreviewScene>& InPreviewScene)
	: FEditorViewportClient(nullptr,&InPreviewScene.Get(),StaticCastSharedRef<SEditorViewport>(InEditorViewport)), ViewportPtr(InEditorViewport)
{
	//TODO - make a custom preview scene and dont rely on the builtin one
	AdvancedPreviewScene = static_cast<FAdvancedPreviewScene*>(PreviewScene);
	UE_LOG(LogTemp, Error, TEXT("MADE PREVIEW SCENE"));
	SetRealtime(true);
	DrawHelper.bDrawGrid = true;
	DrawHelper.bDrawPivot = false;
	DrawHelper.AxesLineThickness = 5;
	DrawHelper.PivotSize = 5;
	
	SetViewLocation(FVector(0, 1000, 0));
	SetViewRotation(FVector(0, -180, 0).Rotation());

	EngineShowFlags.SetScreenPercentage(true);

	SetViewportType(LVT_Perspective);

	SetViewModes(VMI_Lit, VMI_Lit);

	GeoFlow = ViewportPtr.Pin()->GeoFlow;
	GeoFlow->bSelectable = true;
	AdvancedPreviewScene->SetFloorVisibility(false);
	PreviewScene->AddComponent(GeoFlow,FTransform(),false);
	ActorComponents.Add(GeoFlow);
}
void FGeoFlowViewportClient::Tick(float DeltaSeconds) {
	FEditorViewportClient::Tick(DeltaSeconds);
	if (AdvancedPreviewScene) {
		AdvancedPreviewScene->GetWorld()->Tick(LEVELTICK_All, DeltaSeconds);
	}

	if (!IsValid(GeoFlow->geo)) return;
	UEdGraph* graph = GeoFlow->geo->lastWorkingGraph;
	if (!IsValid(graph)) return;
	TArray<UGFN_E_Base*> nodes;
	graph->GetNodesOfClass(nodes);
	for (UGFN_E_Base* node : nodes) {
		node->DrawInViewport(PreviewScene->GetWorld());
	}
}
void FGeoFlowViewportClient::SetAsset(UGeoFlowAsset* InAsset)
{
	GeoFlow->geo = InAsset;
	//Generate();
}

void FGeoFlowViewportClient::Generate()
{
	GeoFlow->ForceRegen();
}

void FGeoFlowViewportClient::Clear()
{
	GeoFlow->ForceClear();
}

