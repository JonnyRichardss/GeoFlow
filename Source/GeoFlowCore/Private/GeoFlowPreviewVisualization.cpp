#include "GeoFlowPreviewVisualization.h"

IMPLEMENT_HIT_PROXY(HGeoFlowVisProxy, HComponentVisProxy);
IMPLEMENT_HIT_PROXY(HGeoFlowTargetProxy, HGeoFlowVisProxy);

void FGeoFlowPreviewVisualizer::DrawVisualization(const UActorComponent* Component, const FSceneView* View, FPrimitiveDrawInterface* PDI)
{
	if (const UGeoFlowComponent* GeoFlow = Cast<UGeoFlowComponent>(Component))
	{
		/*
		if (!IsValid(GeoFlow->geo)) return;
		UEdGraph* graph = GeoFlow->geo->lastWorkingGraph;
		if (!IsValid(graph)) return;
		TArray<UGFN_E_Base*> nodes;
		graph->GetNodesOfClass(nodes);
		for (UGFN_E_Base* node : nodes) {
			PDI->SetHitProxy(new HGeoFlowVisProxy(Component));
			node->DrawInViewport(PDI);
			PDI->SetHitProxy(NULL);
		}
		PDI->SetHitProxy(new HGeoFlowVisProxy(Component));
		PDI->DrawLine({ 0,0,-10 }, { 0,0,10 }, FLinearColor::Blue, SDPG_Foreground);
		PDI->SetHitProxy(NULL);
		*/
		
	}
}
