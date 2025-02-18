#include "GeoFlowConstantNodes.h"
#include "GeoFlowRuntimeGraph.h"
/*
	Editor
*/
UGFN_R_Base* UGFN_E_ConstantBool::CreateRuntimeNode(UGeoFlowRuntimeGraph* runtimeGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap<FGuid, UGeoFlowRuntimePin*>& idToPinMap)
{
	UGFN_R_ConstantBool* runtimeNode = InitRuntimeNode<UGFN_R_ConstantBool>(runtimeGraph);
	runtimeNode->Output = InitRuntimePin(runtimeNode, Output, connections, idToPinMap, EGeoFlowReturnType::Bool);
	runtimeNode->Value = Value;
	return runtimeNode;
}
UGFN_R_Base* UGFN_E_ConstantInt::CreateRuntimeNode(UGeoFlowRuntimeGraph* runtimeGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap<FGuid, UGeoFlowRuntimePin*>& idToPinMap)
{
	UGFN_R_ConstantInt* runtimeNode = InitRuntimeNode<UGFN_R_ConstantInt>(runtimeGraph);
	runtimeNode->Output = InitRuntimePin(runtimeNode, Output, connections, idToPinMap, EGeoFlowReturnType::Int);
	runtimeNode->Value = Value;
	return runtimeNode;
}
UGFN_R_Base* UGFN_E_ConstantDouble::CreateRuntimeNode(UGeoFlowRuntimeGraph* runtimeGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap<FGuid, UGeoFlowRuntimePin*>& idToPinMap)
{
	UGFN_R_ConstantDouble* runtimeNode = InitRuntimeNode<UGFN_R_ConstantDouble>(runtimeGraph);
	runtimeNode->Output = InitRuntimePin(runtimeNode, Output, connections, idToPinMap, EGeoFlowReturnType::Float);
	runtimeNode->Value = Value;
	return runtimeNode;
}
UGFN_R_Base* UGFN_E_ConstantVector::CreateRuntimeNode(UGeoFlowRuntimeGraph* runtimeGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap<FGuid, UGeoFlowRuntimePin*>& idToPinMap)
{
	UGFN_R_ConstantVector* runtimeNode = InitRuntimeNode<UGFN_R_ConstantVector>(runtimeGraph);
	runtimeNode->Output = InitRuntimePin(runtimeNode, Output, connections, idToPinMap, EGeoFlowReturnType::Vector);
	runtimeNode->Value = Value;
	return runtimeNode;
}
/*
	Runtime
*/
UGFN_E_Base* UGFN_R_ConstantBool::CreateEditorNode(UEdGraph* _workingGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap<FGuid, UEdGraphPin*>& idToPinMap)
{
	UGFN_E_ConstantBool* newNode = InitUiNode<UGFN_E_ConstantBool>(_workingGraph);
	newNode->Output = InitUiPin(newNode, Output, connections, idToPinMap);
	newNode->Value = Value;
	return newNode;
}

UGFN_E_Base* UGFN_R_ConstantInt::CreateEditorNode(UEdGraph* _workingGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap<FGuid, UEdGraphPin*>& idToPinMap)
{
	UGFN_E_ConstantInt* newNode = InitUiNode<UGFN_E_ConstantInt>(_workingGraph);
	newNode->Output = InitUiPin(newNode, Output, connections, idToPinMap);
	newNode->Value = Value;
	return newNode;
}

UGFN_E_Base* UGFN_R_ConstantDouble::CreateEditorNode(UEdGraph* _workingGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap<FGuid, UEdGraphPin*>& idToPinMap)
{
	UGFN_E_ConstantDouble* newNode = InitUiNode<UGFN_E_ConstantDouble>(_workingGraph);
	newNode->Output = InitUiPin(newNode, Output, connections, idToPinMap);
	newNode->Value = Value;
	return newNode;
}

UGFN_E_Base* UGFN_R_ConstantVector::CreateEditorNode(UEdGraph* _workingGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap<FGuid, UEdGraphPin*>& idToPinMap)
{
	UGFN_E_ConstantVector* newNode = InitUiNode<UGFN_E_ConstantVector>(_workingGraph);
	newNode->Output = InitUiPin(newNode, Output, connections, idToPinMap);
	newNode->Value = Value;
	return newNode;
}

