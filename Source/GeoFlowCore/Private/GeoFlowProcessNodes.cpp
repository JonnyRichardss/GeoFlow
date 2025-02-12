#include "GeoFlowProcessNodes.h"
#include "GeoFlowRuntimeGraph.h"
#include "GeoFlowPinDefaultValueOps.h"
TArray<UEdGraphPin*> UGFN_E_Smin::CreateInputPins(UEdGraphPin* fromPin)
{
	TArray<UEdGraphPin*> InputPins;
	InputA = CreateCustomPin(EGPD_Input, "A", EGeoFlowReturnType::Double);
	InputB = CreateCustomPin(EGPD_Input, "B", EGeoFlowReturnType::Double);
	InputSmoothing = CreateCustomPin(EGPD_Input, "Smoothing", EGeoFlowReturnType::Double);
	InputPins.Add(InputA);
	InputPins.Add(InputB);
	InputPins.Add(InputSmoothing);
	return InputPins;
}
UGFN_R_Base* UGFN_E_Smin::CreateRuntimeNode(UGeoFlowRuntimeGraph* runtimeGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap<FGuid, UGeoFlowRuntimePin*>& idToPinMap)
{

	UGFN_R_Smin* runtimeNode = InitRuntimeNode<UGFN_R_Smin>(runtimeGraph);
	//inputA
	UGeoFlowRuntimePin* InputARuntimePin = InitRuntimePin(runtimeNode, InputA, connections, idToPinMap, EGeoFlowReturnType::Double);
	runtimeNode->a = GetDoubleDefaultValue(InputA);
	runtimeNode->InputA = InputARuntimePin;
	//inputB
	UGeoFlowRuntimePin* InputBRuntimePin = InitRuntimePin(runtimeNode, InputB, connections, idToPinMap, EGeoFlowReturnType::Double);
	runtimeNode->b = GetDoubleDefaultValue(InputB);
	runtimeNode->InputB = InputBRuntimePin;
	//inputSmoothing
	UGeoFlowRuntimePin* InputSmoothingRuntimePin = InitRuntimePin(runtimeNode, InputSmoothing, connections, idToPinMap, EGeoFlowReturnType::Double);
	runtimeNode->smoothing = GetDoubleDefaultValue(InputSmoothing);
	runtimeNode->InputSmoothing = InputSmoothingRuntimePin;
	//Output
	UGeoFlowRuntimePin* OutputRuntimePin = InitRuntimePin(runtimeNode, Output, connections, idToPinMap, EGeoFlowReturnType::Double);
	runtimeNode->Output = OutputRuntimePin;

	return runtimeNode;
}


UGFN_E_Base* UGFN_R_Smin::CreateEditorNode(UEdGraph* _workingGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap<FGuid, UEdGraphPin*>& idToPinMap)
{
	UGFN_E_Smin* newNode = InitUiNode<UGFN_E_Smin>(_workingGraph);

	//inputA
	UEdGraphPin* UiPinA = InitUiPin(newNode, InputA, connections, idToPinMap);
	newNode->InputA = UiPinA;
	SetDoubleDefaultValue(UiPinA, a);
	//inputB
	UEdGraphPin* UiPinB = InitUiPin(newNode, InputB, connections, idToPinMap);
	newNode->InputB = UiPinB;
	SetDoubleDefaultValue(UiPinB, b);
	//inputSmoothing
	UEdGraphPin* UiPinS = InitUiPin(newNode, InputSmoothing, connections, idToPinMap);
	newNode->InputSmoothing = UiPinS;
	SetDoubleDefaultValue(UiPinS, smoothing);
	//output
	UEdGraphPin* OutputUiPin = InitUiPin(newNode, Output, connections, idToPinMap);
	newNode->Output = OutputUiPin;
	return newNode;
}

double UGFN_R_Smin::Evaluate(const FVector3d& pos)
{
	if (InputA->Connection != nullptr) {
		UGFN_R_BaseDouble* node = Cast<UGFN_R_BaseDouble>(InputA->Connection->OwningNode);
		a = node->Evaluate(pos);
	}
	if (InputA->Connection != nullptr) {
		UGFN_R_BaseDouble* node = Cast<UGFN_R_BaseDouble>(InputB->Connection->OwningNode);
		b = node->Evaluate(pos);
	}
	if (InputSmoothing->Connection != nullptr) {
		UGFN_R_BaseDouble* node = Cast<UGFN_R_BaseDouble>(InputSmoothing->Connection->OwningNode);
		smoothing = node->Evaluate(pos);
	}
	//https://iquilezles.org/articles/smin/
	double h = FMath::Clamp(0.5 + 0.5 * (b - a) / smoothing, 0.0, 1.0);
	return FMath::Lerp(b, a, h) - smoothing * h * (1.0 - h);
}

TArray<UEdGraphPin*> UGFN_E_Smax::CreateInputPins(UEdGraphPin* fromPin)
{
	TArray<UEdGraphPin*> InputPins;
	InputA = CreateCustomPin(EGPD_Input, "A", EGeoFlowReturnType::Double);
	InputB = CreateCustomPin(EGPD_Input, "B", EGeoFlowReturnType::Double);
	InputSmoothing = CreateCustomPin(EGPD_Input, "Smoothing", EGeoFlowReturnType::Double);
	InputPins.Add(InputA);
	InputPins.Add(InputB);
	InputPins.Add(InputSmoothing);
	return InputPins;
}
UGFN_R_Base* UGFN_E_Smax::CreateRuntimeNode(UGeoFlowRuntimeGraph* runtimeGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap<FGuid, UGeoFlowRuntimePin*>& idToPinMap)
{

	UGFN_R_Smax* runtimeNode = InitRuntimeNode<UGFN_R_Smax>(runtimeGraph);
	//inputA
	UGeoFlowRuntimePin* InputARuntimePin = InitRuntimePin(runtimeNode, InputA, connections, idToPinMap, EGeoFlowReturnType::Double);
	runtimeNode->a = GetDoubleDefaultValue(InputA);
	runtimeNode->InputA = InputARuntimePin;
	//inputB
	UGeoFlowRuntimePin* InputBRuntimePin = InitRuntimePin(runtimeNode, InputB, connections, idToPinMap, EGeoFlowReturnType::Double);
	runtimeNode->b = GetDoubleDefaultValue(InputB);
	runtimeNode->InputB = InputBRuntimePin;
	//inputSmoothing
	UGeoFlowRuntimePin* InputSmoothingRuntimePin = InitRuntimePin(runtimeNode, InputSmoothing, connections, idToPinMap, EGeoFlowReturnType::Double);
	runtimeNode->smoothing = GetDoubleDefaultValue(InputSmoothing);
	runtimeNode->InputSmoothing = InputSmoothingRuntimePin;
	//Output
	UGeoFlowRuntimePin* OutputRuntimePin = InitRuntimePin(runtimeNode, Output, connections, idToPinMap, EGeoFlowReturnType::Double);
	runtimeNode->Output = OutputRuntimePin;

	return runtimeNode;
}


UGFN_E_Base* UGFN_R_Smax::CreateEditorNode(UEdGraph* _workingGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap<FGuid, UEdGraphPin*>& idToPinMap)
{
	UGFN_E_Smax* newNode = InitUiNode<UGFN_E_Smax>(_workingGraph);

	//inputA
	UEdGraphPin* UiPinA = InitUiPin(newNode, InputA, connections, idToPinMap);
	newNode->InputA = UiPinA;
	SetDoubleDefaultValue(UiPinA, a);
	//inputB
	UEdGraphPin* UiPinB = InitUiPin(newNode, InputB, connections, idToPinMap);
	newNode->InputB = UiPinB;
	SetDoubleDefaultValue(UiPinB, b);
	//inputSmoothing
	UEdGraphPin* UiPinS = InitUiPin(newNode, InputSmoothing, connections, idToPinMap);
	newNode->InputSmoothing = UiPinS;
	SetDoubleDefaultValue(UiPinS, smoothing);
	//output
	UEdGraphPin* OutputUiPin = InitUiPin(newNode, Output, connections, idToPinMap);
	newNode->Output = OutputUiPin;
	return newNode;
}

double UGFN_R_Smax::Evaluate(const FVector3d& pos)
{
	if (InputA->Connection != nullptr) {
		UGFN_R_BaseDouble* node = Cast<UGFN_R_BaseDouble>(InputA->Connection->OwningNode);
		a = node->Evaluate(pos);
	}
	if (InputA->Connection != nullptr) {
		UGFN_R_BaseDouble* node = Cast<UGFN_R_BaseDouble>(InputB->Connection->OwningNode);
		b = node->Evaluate(pos);
	}
	if (InputSmoothing->Connection != nullptr) {
		UGFN_R_BaseDouble* node = Cast<UGFN_R_BaseDouble>(InputSmoothing->Connection->OwningNode);
		smoothing = node->Evaluate(pos);
	}
	//https://iquilezles.org/articles/distfunctions/
	double h = FMath::Clamp(0.5 - 0.5 * (b + a) / smoothing, 0.0, 1.0);
	return FMath::Lerp(b, 0.0-a, h) + smoothing * h * (1.0 - h);
}



TArray<UEdGraphPin*> UGFN_E_Min::CreateInputPins(UEdGraphPin* fromPin)
{
	TArray<UEdGraphPin*> InputPins;
	InputA = CreateCustomPin(EGPD_Input, "A", EGeoFlowReturnType::Double);
	InputB = CreateCustomPin(EGPD_Input, "B", EGeoFlowReturnType::Double);
	InputPins.Add(InputA);
	InputPins.Add(InputB);
	return InputPins;
}
UGFN_R_Base* UGFN_E_Min::CreateRuntimeNode(UGeoFlowRuntimeGraph* runtimeGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap<FGuid, UGeoFlowRuntimePin*>& idToPinMap)
{
	UGFN_R_Min* runtimeNode = InitRuntimeNode<UGFN_R_Min>(runtimeGraph);
	//inputA
	UGeoFlowRuntimePin* InputARuntimePin = InitRuntimePin(runtimeNode, InputA, connections, idToPinMap, EGeoFlowReturnType::Double);
	runtimeNode->a = GetDoubleDefaultValue(InputA);
	runtimeNode->InputA = InputARuntimePin;
	//inputB
	UGeoFlowRuntimePin* InputBRuntimePin = InitRuntimePin(runtimeNode, InputB, connections, idToPinMap, EGeoFlowReturnType::Double);
	runtimeNode->b = GetDoubleDefaultValue(InputB);
	runtimeNode->InputB = InputBRuntimePin;
	//Output
	UGeoFlowRuntimePin* OutputRuntimePin = InitRuntimePin(runtimeNode, Output, connections, idToPinMap, EGeoFlowReturnType::Double);
	runtimeNode->Output = OutputRuntimePin;

	return runtimeNode;

}


UGFN_E_Base* UGFN_R_Min::CreateEditorNode(UEdGraph* _workingGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap<FGuid, UEdGraphPin*>& idToPinMap)
{
	auto newNode = InitUiNode<UGFN_E_Min>(_workingGraph);

	//inputA
	UEdGraphPin* UiPinA = InitUiPin(newNode, InputA, connections, idToPinMap);
	newNode->InputA = UiPinA;
	SetDoubleDefaultValue(UiPinA, a);
	//inputB
	UEdGraphPin* UiPinB = InitUiPin(newNode, InputB, connections, idToPinMap);
	newNode->InputB = UiPinB;
	SetDoubleDefaultValue(UiPinB, b);
	//output
	UEdGraphPin* OutputUiPin = InitUiPin(newNode, Output, connections, idToPinMap);
	newNode->Output = OutputUiPin;
	return newNode;
}

double UGFN_R_Min::Evaluate(const FVector3d& pos)
{
	if (InputA->Connection != nullptr) {
		UGFN_R_BaseDouble* node = Cast<UGFN_R_BaseDouble>(InputA->Connection->OwningNode);
		a = node->Evaluate(pos);
	}
	if (InputA->Connection != nullptr) {
		UGFN_R_BaseDouble* node = Cast<UGFN_R_BaseDouble>(InputB->Connection->OwningNode);
		b = node->Evaluate(pos);
	}
	//https://iquilezles.org/articles/smin/
	
	return FMath::Min(a, b);
}

TArray<UEdGraphPin*> UGFN_E_Max::CreateInputPins(UEdGraphPin* fromPin)
{
	TArray<UEdGraphPin*> InputPins;
	InputA = CreateCustomPin(EGPD_Input, "A", EGeoFlowReturnType::Double);
	InputB = CreateCustomPin(EGPD_Input, "B", EGeoFlowReturnType::Double);
	InputPins.Add(InputA);
	InputPins.Add(InputB);
	return InputPins;
}
UGFN_R_Base* UGFN_E_Max::CreateRuntimeNode(UGeoFlowRuntimeGraph* runtimeGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap<FGuid, UGeoFlowRuntimePin*>& idToPinMap)
{
	UGFN_R_Max* runtimeNode = InitRuntimeNode<UGFN_R_Max>(runtimeGraph);
	//inputA
	UGeoFlowRuntimePin* InputARuntimePin = InitRuntimePin(runtimeNode, InputA, connections, idToPinMap, EGeoFlowReturnType::Double);
	runtimeNode->a = GetDoubleDefaultValue(InputA);
	runtimeNode->InputA = InputARuntimePin;
	//inputB
	UGeoFlowRuntimePin* InputBRuntimePin = InitRuntimePin(runtimeNode, InputB, connections, idToPinMap, EGeoFlowReturnType::Double);
	runtimeNode->b = GetDoubleDefaultValue(InputB);
	runtimeNode->InputB = InputBRuntimePin;
	//Output
	UGeoFlowRuntimePin* OutputRuntimePin = InitRuntimePin(runtimeNode, Output, connections, idToPinMap, EGeoFlowReturnType::Double);
	runtimeNode->Output = OutputRuntimePin;

	return runtimeNode;
}


UGFN_E_Base* UGFN_R_Max::CreateEditorNode(UEdGraph* _workingGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap<FGuid, UEdGraphPin*>& idToPinMap)
{
	auto newNode = InitUiNode<UGFN_E_Max>(_workingGraph);

	//inputA
	UEdGraphPin* UiPinA = InitUiPin(newNode, InputA, connections, idToPinMap);
	newNode->InputA = UiPinA;
	SetDoubleDefaultValue(UiPinA, a);
	//inputB
	UEdGraphPin* UiPinB = InitUiPin(newNode, InputB, connections, idToPinMap);
	newNode->InputB = UiPinB;
	SetDoubleDefaultValue(UiPinB, b);
	//output
	UEdGraphPin* OutputUiPin = InitUiPin(newNode, Output, connections, idToPinMap);
	newNode->Output = OutputUiPin;
	return newNode;
}

double UGFN_R_Max::Evaluate(const FVector3d& pos)
{
	if (InputA->Connection != nullptr) {
		UGFN_R_BaseDouble* node = Cast<UGFN_R_BaseDouble>(InputA->Connection->OwningNode);
		a = node->Evaluate(pos);
	}
	if (InputA->Connection != nullptr) {
		UGFN_R_BaseDouble* node = Cast<UGFN_R_BaseDouble>(InputB->Connection->OwningNode);
		b = node->Evaluate(pos);
	}
	return FMath::Max(-a, b);
}
