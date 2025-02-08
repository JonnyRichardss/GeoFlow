#include "GeoFlowConstantNodes.h"
#include "GeoFlowRuntimeGraph.h"
/*
	Editor
*/
static void DoCommonEditor(UGFN_E_Base* UiNode,UGFN_R_Base* runtimeNode, TArray<std::pair<FGuid, FGuid>>& connections, TMap<FGuid, UGeoFlowRuntimePin*>& idToPinMap) {
	runtimeNode->Position = FVector2D(UiNode->NodePosX, UiNode->NodePosY);
	UGeoFlowRuntimePin* OutputRuntimePin = NewObject<UGeoFlowRuntimePin>(runtimeNode);
	OutputRuntimePin->PinName = UiNode->Output->PinName;
	OutputRuntimePin->PinId = UiNode->Output->PinId;
	//connections check only on inputs
	idToPinMap.Add(UiNode->Output->PinId, OutputRuntimePin);
	runtimeNode->Output = OutputRuntimePin;
	OutputRuntimePin->OwningNode = runtimeNode;
}
UGFN_R_Base* UGFN_E_ConstantBool::CreateRuntimeNode(UGeoFlowRuntimeGraph* runtimeGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap<FGuid, UGeoFlowRuntimePin*>& idToPinMap)
{
	UGFN_R_ConstantBool* runtimeNode = NewObject<UGFN_R_ConstantBool>(runtimeGraph);
	DoCommonEditor(this, runtimeNode, connections, idToPinMap);
	runtimeNode->Value = Value;
	return runtimeNode;
}
UGFN_R_Base* UGFN_E_ConstantInt::CreateRuntimeNode(UGeoFlowRuntimeGraph* runtimeGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap<FGuid, UGeoFlowRuntimePin*>& idToPinMap)
{
	UGFN_R_ConstantInt* runtimeNode = NewObject<UGFN_R_ConstantInt>(runtimeGraph);
	DoCommonEditor(this, runtimeNode, connections, idToPinMap);
	runtimeNode->Value = Value;
	return runtimeNode;
}
UGFN_R_Base* UGFN_E_ConstantDouble::CreateRuntimeNode(UGeoFlowRuntimeGraph* runtimeGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap<FGuid, UGeoFlowRuntimePin*>& idToPinMap)
{
	UGFN_R_ConstantDouble* runtimeNode = NewObject<UGFN_R_ConstantDouble>(runtimeGraph);
	DoCommonEditor(this, runtimeNode, connections, idToPinMap);
	runtimeNode->Value = Value;
	return runtimeNode;
}
UGFN_R_Base* UGFN_E_ConstantVector::CreateRuntimeNode(UGeoFlowRuntimeGraph* runtimeGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap<FGuid, UGeoFlowRuntimePin*>& idToPinMap)
{
	UGFN_R_ConstantVector* runtimeNode = NewObject<UGFN_R_ConstantVector>(runtimeGraph);
	DoCommonEditor(this, runtimeNode, connections, idToPinMap);
	runtimeNode->Value = Value;
	return runtimeNode;
}
/*
	Runtime
*/
static void DoCommonRuntime(UGFN_R_Base* runtimeNode, UGFN_E_Base* newNode, TArray<std::pair<FGuid, FGuid>>& connections, TMap<FGuid, UEdGraphPin*>& idToPinMap) {
	newNode->CreateNewGuid();
	newNode->NodePosX = runtimeNode->Position.X;
	newNode->NodePosY = runtimeNode->Position.Y;
	UEdGraphPin* uiPin = newNode->CreateCustomPin(EEdGraphPinDirection::EGPD_Output, runtimeNode->Output->PinName, runtimeNode->NodeReturnType());
	uiPin->PinId = runtimeNode->Output->PinId;
	newNode->Output = uiPin;
	if (runtimeNode->Output->Connection != nullptr) {
		connections.Add(std::make_pair(runtimeNode->Output->PinId, runtimeNode->Output->Connection->PinId));
	}
	idToPinMap.Add(runtimeNode->Output->PinId, uiPin);
}
UGFN_E_Base* UGFN_R_ConstantBool::CreateEditorNode(UEdGraph* _workingGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap<FGuid, UEdGraphPin*>& idToPinMap)
{
	UGFN_E_ConstantBool* newNode = NewObject<UGFN_E_ConstantBool>(_workingGraph);
	DoCommonRuntime(this, newNode, connections, idToPinMap);
	newNode->Value = Value;
	return newNode;
}
UGFN_E_Base* UGFN_R_ConstantInt::CreateEditorNode(UEdGraph* _workingGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap<FGuid, UEdGraphPin*>& idToPinMap)
{
	UGFN_E_ConstantInt* newNode = NewObject<UGFN_E_ConstantInt>(_workingGraph);
	DoCommonRuntime(this, newNode, connections, idToPinMap);
	newNode->Value = Value;
	return newNode;
}
UGFN_E_Base* UGFN_R_ConstantDouble::CreateEditorNode(UEdGraph* _workingGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap<FGuid, UEdGraphPin*>& idToPinMap)
{
	UGFN_E_ConstantDouble* newNode = NewObject<UGFN_E_ConstantDouble>(_workingGraph);
	DoCommonRuntime(this, newNode, connections, idToPinMap);
	newNode->Value = Value;
	return newNode;
}
UGFN_E_Base* UGFN_R_ConstantVector::CreateEditorNode(UEdGraph* _workingGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap<FGuid, UEdGraphPin*>& idToPinMap)
{
	UGFN_E_ConstantVector* newNode = NewObject<UGFN_E_ConstantVector>(_workingGraph);
	DoCommonRuntime(this, newNode, connections, idToPinMap);
	newNode->Value = Value;
	return newNode;
}