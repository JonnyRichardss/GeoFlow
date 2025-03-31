#pragma once
#include "CoreMinimal.h"
#include "Nodes/GeoFlowNodeTypes.h"
#include "Nodes/GeoFlowConstantNodes.h"
#include "GeoFlowComponent.h"
#include "ComponentVisualizer.h"

//https://unrealcommunity.wiki/component-visualizers-xaa1qsng


class GEOFLOWCORE_API FGeoFlowPreviewVisualizer : public FComponentVisualizer {
public:
	//FGeoFlowPreviewVisualizer();
	//virtual ~FGeoFlowPreviewVisualizer();
	// Begin FComponentVisualizer interface
	//virtual void OnRegister() override;
	virtual void DrawVisualization(const UActorComponent* Component, const FSceneView* View, FPrimitiveDrawInterface* PDI) override;

	//virtual bool VisProxyHandleClick(FEditorViewportClient* InViewportClient, HComponentVisProxy* VisProxy, const FViewportClick& Click);
	//virtual void EndEditing() override;
	//virtual bool GetWidgetLocation(const FEditorViewportClient* ViewportClient, FVector& OutLocation) const override;
	//virtual bool GetCustomInputCoordinateSystem(const FEditorViewportClient* ViewportClient, FMatrix& OutMatrix) const override;
	//virtual bool HandleInputDelta(FEditorViewportClient* ViewportClient, FViewport* Viewport, FVector& DeltaTranslate, FRotator& DeltaRotate, FVector& DeltaScale) override;
	//virtual bool HandleInputKey(FEditorViewportClient* ViewportClient, FViewport* Viewport, FKey Key, EInputEvent Event) override;
	//virtual void DrawVisualizationHUD(const UActorComponent* Component, const FViewport* Viewport, const FSceneView* View, FCanvas* Canvas) override;
	//virtual TSharedPtr<SWidget> GenerateContextMenu() const override;
	// End FComponentVisualizer interface

	/** Get the component we are currently editing */
	//UGeoFlowPreviewComponent* GetEditedComponent() const;
private:
	/**Selected node*/
	UGFN_E_Base* TargetNode;

	/**Output log commands*/
	TSharedPtr<FUICommandList> TargetingComponentVisualizerActions;
};
/**Base class for clickable targeting editing proxies*/
struct GEOFLOWCORE_API HGeoFlowVisProxy : public HComponentVisProxy
{
	DECLARE_HIT_PROXY();

	HGeoFlowVisProxy(const UActorComponent* InComponent)
		: HComponentVisProxy(InComponent, HPP_Wireframe)
	{
	}
};
struct GEOFLOWCORE_API HGeoFlowTargetProxy : public HGeoFlowVisProxy {
	DECLARE_HIT_PROXY()

	HGeoFlowTargetProxy(const UActorComponent* InComponent, UGFN_E_Base* InTargetNode)
		:HGeoFlowVisProxy(InComponent), TargetNode(InTargetNode) {}
	UGFN_E_Base* TargetNode;
};