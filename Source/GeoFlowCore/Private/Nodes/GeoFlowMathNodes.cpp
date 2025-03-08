#include "Nodes/GeoFlowMathNodes.h"
#include "GeoFlowRuntimeGraph.h"
#include "GeoFlowPinDefaultValueOps.h"

UGFN_E_Base* UGFN_R_MakeVector::CreateEditorNode(UEdGraph* _workingGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap<FGuid, UEdGraphPin*>& idToPinMap)
{
	UGFN_E_MakeVector* newNode = InitUiNode<UGFN_E_MakeVector>(_workingGraph);
	//input
	UEdGraphPin* UiPin = InitUiPin(newNode, Input, connections, idToPinMap);
	newNode->Input = UiPin;
	SetFloatDefaultValue(UiPin, inputVal);

	//output
	UEdGraphPin* OutputUiPin = InitUiPin(newNode, Output, connections, idToPinMap);
	newNode->Output = OutputUiPin;
	return newNode;
}



UGFN_R_Base* UGFN_E_MakeVector::CreateRuntimeNode(UGeoFlowRuntimeGraph* runtimeGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap<FGuid, UGeoFlowRuntimePin*>& idToPinMap)
{
	UGFN_R_MakeVector* runtimeNode = InitRuntimeNode<UGFN_R_MakeVector>(runtimeGraph);
	//inputA
	UGeoFlowRuntimePin* InputARuntimePin = InitRuntimePin(runtimeNode, Input, connections, idToPinMap, EGeoFlowReturnType::Float);
	runtimeNode->inputVal = GetFloatDefaultValue(Input);
	runtimeNode->Input = InputARuntimePin;
	//Output
	UGeoFlowRuntimePin* OutputRuntimePin = InitRuntimePin(runtimeNode, Output, connections, idToPinMap, EGeoFlowReturnType::Float);
	runtimeNode->Output = OutputRuntimePin;

	return runtimeNode;
}

TArray<UEdGraphPin*> UGFN_E_MakeVector::CreateInputPins(UEdGraphPin* fromPin)
{
	TArray<UEdGraphPin*> InputPins;
	Input = CreateCustomPin(EGPD_Input, "In", EGeoFlowReturnType::Float);
	InputPins.Add(Input);
	return InputPins;
}

FVector3f UGFN_R_MakeVector::Evaluate(const FVector3f& pos)
{
	if (Input->Connection != nullptr) {
		UGFN_R_BaseFloat* node = Cast<UGFN_R_BaseFloat>(Input->Connection->OwningNode);
		inputVal = node->Evaluate(pos);
	}
	return FVector3f(inputVal, inputVal, inputVal);
}
UGFN_R_Base* UGFN_E_FloatMathBase::CreateRuntimeNodeSpecial(UGFN_R_FloatMathBase* runtimeNode,UGeoFlowRuntimeGraph* runtimeGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap<FGuid, UGeoFlowRuntimePin*>& idToPinMap)
{
	//inputA
	UGeoFlowRuntimePin* InputARuntimePin = InitRuntimePin(runtimeNode, InputA, connections, idToPinMap, EGeoFlowReturnType::Float);
	runtimeNode->a = GetFloatDefaultValue(InputA);
	runtimeNode->InputA = InputARuntimePin;
	//inputB
	UGeoFlowRuntimePin* InputBRuntimePin = InitRuntimePin(runtimeNode, InputB, connections, idToPinMap, EGeoFlowReturnType::Float);
	runtimeNode->b = GetFloatDefaultValue(InputB);
	runtimeNode->InputB = InputBRuntimePin;
	//Output
	UGeoFlowRuntimePin* OutputRuntimePin = InitRuntimePin(runtimeNode, Output, connections, idToPinMap, EGeoFlowReturnType::Float);
	runtimeNode->Output = OutputRuntimePin;

	return runtimeNode;
}

TArray<UEdGraphPin*> UGFN_E_FloatMathBase::CreateInputPins(UEdGraphPin* fromPin)
{
	TArray<UEdGraphPin*> InputPins;
	InputA = CreateCustomPin(EGPD_Input, "A", EGeoFlowReturnType::Float);
	InputB = CreateCustomPin(EGPD_Input, "B", EGeoFlowReturnType::Float);
	InputPins.Add(InputA);
	InputPins.Add(InputB);
	return InputPins;
}

UGFN_E_Base* UGFN_R_FloatMathBase::CreateEditorNodeSpecial(UGFN_E_FloatMathBase* newNode,UEdGraph* _workingGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap<FGuid, UEdGraphPin*>& idToPinMap)
{

	//inputA
	UEdGraphPin* UiPinA = InitUiPin(newNode, InputA, connections, idToPinMap);
	newNode->InputA = UiPinA;
	SetFloatDefaultValue(UiPinA, a);
	//inputB
	UEdGraphPin* UiPinB = InitUiPin(newNode, InputB, connections, idToPinMap);
	newNode->InputB = UiPinB;
	SetFloatDefaultValue(UiPinB, b);
	//output
	UEdGraphPin* OutputUiPin = InitUiPin(newNode, Output, connections, idToPinMap);
	newNode->Output = OutputUiPin;
	return newNode;
}

UGFN_R_Base* UGFN_E_FloatAdd::CreateRuntimeNode(UGeoFlowRuntimeGraph* runtimeGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap<FGuid, UGeoFlowRuntimePin*>& idToPinMap)
{
	UGFN_R_FloatAdd* runtimeNode = InitRuntimeNode<UGFN_R_FloatAdd>(runtimeGraph);
	UGFN_E_FloatMathBase::CreateRuntimeNodeSpecial(runtimeNode, runtimeGraph, connections, idToPinMap);
	return runtimeNode;
}

UGFN_E_Base* UGFN_R_FloatAdd::CreateEditorNode(UEdGraph* _workingGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap<FGuid, UEdGraphPin*>& idToPinMap)
{
	UGFN_E_FloatAdd* newNode = InitUiNode<UGFN_E_FloatAdd>(_workingGraph);
	UGFN_R_FloatMathBase::CreateEditorNodeSpecial(newNode, _workingGraph, connections, idToPinMap);
	return newNode;
}

float UGFN_R_FloatAdd::Evaluate(const FVector3f& pos)
{
	if (InputA->Connection != nullptr) {
		UGFN_R_BaseFloat* node = Cast<UGFN_R_BaseFloat>(InputA->Connection->OwningNode);
		a = node->Evaluate(pos);
	}

	if (InputB->Connection != nullptr) {
		UGFN_R_BaseFloat* node = Cast<UGFN_R_BaseFloat>(InputB->Connection->OwningNode);
		b = node->Evaluate(pos);
	}
	return a + b;
}

UGFN_R_Base* UGFN_E_FloatMultiply::CreateRuntimeNode(UGeoFlowRuntimeGraph* runtimeGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap<FGuid, UGeoFlowRuntimePin*>& idToPinMap)
{
	UGFN_R_FloatMultiply* runtimeNode = InitRuntimeNode<UGFN_R_FloatMultiply>(runtimeGraph);
	UGFN_E_FloatMathBase::CreateRuntimeNodeSpecial(runtimeNode, runtimeGraph, connections, idToPinMap);
	return runtimeNode;
}

UGFN_E_Base* UGFN_R_FloatMultiply::CreateEditorNode(UEdGraph* _workingGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap<FGuid, UEdGraphPin*>& idToPinMap)
{
	UGFN_E_FloatMultiply* newNode = InitUiNode<UGFN_E_FloatMultiply>(_workingGraph);
	UGFN_R_FloatMathBase::CreateEditorNodeSpecial(newNode, _workingGraph, connections, idToPinMap);
	return newNode;
}

float UGFN_R_FloatMultiply::Evaluate(const FVector3f& pos)
{
	if (InputA->Connection != nullptr) {
		UGFN_R_BaseFloat* node = Cast<UGFN_R_BaseFloat>(InputA->Connection->OwningNode);
		a = node->Evaluate(pos);
	}

	if (InputB->Connection != nullptr) {
		UGFN_R_BaseFloat* node = Cast<UGFN_R_BaseFloat>(InputB->Connection->OwningNode);
		b = node->Evaluate(pos);
	}
	return a * b;
}
UGFN_R_Base* UGFN_E_FloatSubtract::CreateRuntimeNode(UGeoFlowRuntimeGraph* runtimeGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap<FGuid, UGeoFlowRuntimePin*>& idToPinMap)
{
	UGFN_R_FloatSubtract* runtimeNode = InitRuntimeNode<UGFN_R_FloatSubtract>(runtimeGraph);
	UGFN_E_FloatMathBase::CreateRuntimeNodeSpecial(runtimeNode, runtimeGraph, connections, idToPinMap);
	return runtimeNode;
}

UGFN_E_Base* UGFN_R_FloatSubtract::CreateEditorNode(UEdGraph* _workingGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap<FGuid, UEdGraphPin*>& idToPinMap)
{
	UGFN_E_FloatSubtract* newNode = InitUiNode<UGFN_E_FloatSubtract>(_workingGraph);
	UGFN_R_FloatMathBase::CreateEditorNodeSpecial(newNode, _workingGraph, connections, idToPinMap);
	return newNode;
}

float UGFN_R_FloatSubtract::Evaluate(const FVector3f& pos)
{
	if (InputA->Connection != nullptr) {
		UGFN_R_BaseFloat* node = Cast<UGFN_R_BaseFloat>(InputA->Connection->OwningNode);
		a = node->Evaluate(pos);
	}

	if (InputB->Connection != nullptr) {
		UGFN_R_BaseFloat* node = Cast<UGFN_R_BaseFloat>(InputB->Connection->OwningNode);
		b = node->Evaluate(pos);
	}
	return a - b;
}
UGFN_R_Base* UGFN_E_FloatDivide::CreateRuntimeNode(UGeoFlowRuntimeGraph* runtimeGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap<FGuid, UGeoFlowRuntimePin*>& idToPinMap)
{
	UGFN_R_FloatDivide* runtimeNode = InitRuntimeNode<UGFN_R_FloatDivide>(runtimeGraph);
	UGFN_E_FloatMathBase::CreateRuntimeNodeSpecial(runtimeNode, runtimeGraph, connections, idToPinMap);
	return runtimeNode;
}

UGFN_E_Base* UGFN_R_FloatDivide::CreateEditorNode(UEdGraph* _workingGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap<FGuid, UEdGraphPin*>& idToPinMap)
{
	UGFN_E_FloatDivide* newNode = InitUiNode<UGFN_E_FloatDivide>(_workingGraph);
	UGFN_R_FloatMathBase::CreateEditorNodeSpecial(newNode, _workingGraph, connections, idToPinMap);
	return newNode;
}

float UGFN_R_FloatDivide::Evaluate(const FVector3f& pos)
{
	if (InputA->Connection != nullptr) {
		UGFN_R_BaseFloat* node = Cast<UGFN_R_BaseFloat>(InputA->Connection->OwningNode);
		a = node->Evaluate(pos);
	}

	if (InputB->Connection != nullptr) {
		UGFN_R_BaseFloat* node = Cast<UGFN_R_BaseFloat>(InputB->Connection->OwningNode);
		b = node->Evaluate(pos);
	}
	if (b == 0) return 0; //prevent divide by 0

	return a / b;
}

UGFN_R_Base* UGFN_E_VectorMathBase::CreateRuntimeNodeSpecial(UGFN_R_VectorMathBase* runtimeNode, UGeoFlowRuntimeGraph* runtimeGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap<FGuid, UGeoFlowRuntimePin*>& idToPinMap)
{
	//inputA
	UGeoFlowRuntimePin* InputARuntimePin = InitRuntimePin(runtimeNode, InputA, connections, idToPinMap, EGeoFlowReturnType::Float);
	runtimeNode->a = GetVectorDefaultValue(InputA);
	runtimeNode->InputA = InputARuntimePin;
	//inputB
	UGeoFlowRuntimePin* InputBRuntimePin = InitRuntimePin(runtimeNode, InputB, connections, idToPinMap, EGeoFlowReturnType::Float);
	runtimeNode->b = GetVectorDefaultValue(InputB);
	runtimeNode->InputB = InputBRuntimePin;
	//Output
	UGeoFlowRuntimePin* OutputRuntimePin = InitRuntimePin(runtimeNode, Output, connections, idToPinMap, EGeoFlowReturnType::Float);
	runtimeNode->Output = OutputRuntimePin;

	return runtimeNode;
}

TArray<UEdGraphPin*> UGFN_E_VectorMathBase::CreateInputPins(UEdGraphPin* fromPin)
{
	TArray<UEdGraphPin*> InputPins;
	InputA = CreateCustomPin(EGPD_Input, "A", EGeoFlowReturnType::Vector);
	InputB = CreateCustomPin(EGPD_Input, "B", EGeoFlowReturnType::Vector);
	InputPins.Add(InputA);
	InputPins.Add(InputB);
	return InputPins;
}

UGFN_E_Base* UGFN_R_VectorMathBase::CreateEditorNodeSpecial(UGFN_E_VectorMathBase* newNode, UEdGraph* _workingGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap<FGuid, UEdGraphPin*>& idToPinMap)
{
	//inputA
	UEdGraphPin* UiPinA = InitUiPin(newNode, InputA, connections, idToPinMap);
	newNode->InputA = UiPinA;
	SetVectorDefaultValue(UiPinA, a);
	//inputB
	UEdGraphPin* UiPinB = InitUiPin(newNode, InputB, connections, idToPinMap);
	newNode->InputB = UiPinB;
	SetVectorDefaultValue(UiPinB, b);
	//output
	UEdGraphPin* OutputUiPin = InitUiPin(newNode, Output, connections, idToPinMap);
	newNode->Output = OutputUiPin;
	return newNode;
}



UGFN_R_Base* UGFN_E_VectorAdd::CreateRuntimeNode(UGeoFlowRuntimeGraph* runtimeGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap<FGuid, UGeoFlowRuntimePin*>& idToPinMap)
{
	UGFN_R_VectorAdd* runtimeNode = InitRuntimeNode<UGFN_R_VectorAdd>(runtimeGraph);
	UGFN_E_VectorMathBase::CreateRuntimeNodeSpecial(runtimeNode, runtimeGraph, connections, idToPinMap);
	return runtimeNode;
}

UGFN_E_Base* UGFN_R_VectorAdd::CreateEditorNode(UEdGraph* _workingGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap<FGuid, UEdGraphPin*>& idToPinMap)
{
	UGFN_E_VectorAdd* newNode = InitUiNode<UGFN_E_VectorAdd>(_workingGraph);
	UGFN_R_VectorMathBase::CreateEditorNodeSpecial(newNode, _workingGraph, connections, idToPinMap);
	return newNode;
}

FVector3f UGFN_R_VectorAdd::Evaluate(const FVector3f& pos)
{
	if (InputA->Connection != nullptr) {
		UGFN_R_BaseVector* node = Cast<UGFN_R_BaseVector>(InputA->Connection->OwningNode);
		a = node->Evaluate(pos);
	}

	if (InputB->Connection != nullptr) {
		UGFN_R_BaseVector* node = Cast<UGFN_R_BaseVector>(InputB->Connection->OwningNode);
		b = node->Evaluate(pos);
	}
	return a + b;
}

UGFN_R_Base* UGFN_E_VectorMultiply::CreateRuntimeNode(UGeoFlowRuntimeGraph* runtimeGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap<FGuid, UGeoFlowRuntimePin*>& idToPinMap)
{
	UGFN_R_VectorMultiply* runtimeNode = InitRuntimeNode<UGFN_R_VectorMultiply>(runtimeGraph);
	UGFN_E_VectorMathBase::CreateRuntimeNodeSpecial(runtimeNode, runtimeGraph, connections, idToPinMap);
	return runtimeNode;
}

UGFN_E_Base* UGFN_R_VectorMultiply::CreateEditorNode(UEdGraph* _workingGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap<FGuid, UEdGraphPin*>& idToPinMap)
{
	UGFN_E_VectorMultiply* newNode = InitUiNode<UGFN_E_VectorMultiply>(_workingGraph);
	UGFN_R_VectorMathBase::CreateEditorNodeSpecial(newNode, _workingGraph, connections, idToPinMap);
	return newNode;
}

FVector3f UGFN_R_VectorMultiply::Evaluate(const FVector3f& pos)
{
	if (InputA->Connection != nullptr) {
		UGFN_R_BaseVector* node = Cast<UGFN_R_BaseVector>(InputA->Connection->OwningNode);
		a = node->Evaluate(pos);
	}

	if (InputB->Connection != nullptr) {
		UGFN_R_BaseVector* node = Cast<UGFN_R_BaseVector>(InputB->Connection->OwningNode);
		b = node->Evaluate(pos);
	}
	return a * b;
}
UGFN_R_Base* UGFN_E_VectorSubtract::CreateRuntimeNode(UGeoFlowRuntimeGraph* runtimeGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap<FGuid, UGeoFlowRuntimePin*>& idToPinMap)
{
	UGFN_R_VectorSubtract* runtimeNode = InitRuntimeNode<UGFN_R_VectorSubtract>(runtimeGraph);
	UGFN_E_VectorMathBase::CreateRuntimeNodeSpecial(runtimeNode, runtimeGraph, connections, idToPinMap);
	return runtimeNode;
}

UGFN_E_Base* UGFN_R_VectorSubtract::CreateEditorNode(UEdGraph* _workingGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap<FGuid, UEdGraphPin*>& idToPinMap)
{
	UGFN_E_VectorSubtract* newNode = InitUiNode<UGFN_E_VectorSubtract>(_workingGraph);
	UGFN_R_VectorMathBase::CreateEditorNodeSpecial(newNode, _workingGraph, connections, idToPinMap);
	return newNode;
}

FVector3f UGFN_R_VectorSubtract::Evaluate(const FVector3f& pos)
{
	if (InputA->Connection != nullptr) {
		UGFN_R_BaseVector* node = Cast<UGFN_R_BaseVector>(InputA->Connection->OwningNode);
		a = node->Evaluate(pos);
	}

	if (InputB->Connection != nullptr) {
		UGFN_R_BaseVector* node = Cast<UGFN_R_BaseVector>(InputB->Connection->OwningNode);
		b = node->Evaluate(pos);
	}
	return a - b;
}
UGFN_R_Base* UGFN_E_VectorDivide::CreateRuntimeNode(UGeoFlowRuntimeGraph* runtimeGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap<FGuid, UGeoFlowRuntimePin*>& idToPinMap)
{
	UGFN_R_VectorDivide* runtimeNode = InitRuntimeNode<UGFN_R_VectorDivide>(runtimeGraph);
	UGFN_E_VectorMathBase::CreateRuntimeNodeSpecial(runtimeNode, runtimeGraph, connections, idToPinMap);
	return runtimeNode;
}

UGFN_E_Base* UGFN_R_VectorDivide::CreateEditorNode(UEdGraph* _workingGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap<FGuid, UEdGraphPin*>& idToPinMap)
{
	UGFN_E_VectorDivide* newNode = InitUiNode<UGFN_E_VectorDivide>(_workingGraph);
	UGFN_R_VectorMathBase::CreateEditorNodeSpecial(newNode, _workingGraph, connections, idToPinMap);
	return newNode;
}

FVector3f UGFN_R_VectorDivide::Evaluate(const FVector3f& pos)
{
	if (InputA->Connection != nullptr) {
		UGFN_R_BaseVector* node = Cast<UGFN_R_BaseVector>(InputA->Connection->OwningNode);
		a = node->Evaluate(pos);
	}

	if (InputB->Connection != nullptr) {
		UGFN_R_BaseVector* node = Cast<UGFN_R_BaseVector>(InputB->Connection->OwningNode);
		b = node->Evaluate(pos);
	}
	if (b.X == 0 || b.Y == 0 || b.Z == 0) return FVector3f::ZeroVector; //prevent divide by 0

	return a / b;
}


