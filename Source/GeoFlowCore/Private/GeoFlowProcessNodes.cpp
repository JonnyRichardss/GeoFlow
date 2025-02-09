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
	UGFN_R_Smin* runtimeNode = NewObject<UGFN_R_Smin>(runtimeGraph);
	runtimeNode->Position = FVector2D(NodePosX, NodePosY);

	//InputA
	UGeoFlowRuntimePin* RuntimePinA = NewObject<UGeoFlowRuntimePin>(runtimeNode);
	RuntimePinA->PinName = InputA->PinName;
	RuntimePinA->PinId = InputA->PinId;
	if (InputA->HasAnyConnections()) {
		std::pair<FGuid, FGuid> connection = std::make_pair(InputA->PinId, InputA->LinkedTo[0]->PinId);
		connections.Add(connection);
	}
	else {
		//only updating default value when we have no connection *should* mean that it persists underneath
		//not entirely sure if this is true
		runtimeNode->a = GetDoubleDefaultValue(InputA);
	}
	idToPinMap.Add(InputA->PinId, RuntimePinA);
	runtimeNode->InputA = RuntimePinA;
	RuntimePinA->OwningNode = runtimeNode;

	//InputB
	UGeoFlowRuntimePin* RuntimePinB = NewObject<UGeoFlowRuntimePin>(runtimeNode);
	RuntimePinB->PinName = InputB->PinName;
	RuntimePinB->PinId = InputB->PinId;
	if (InputB->HasAnyConnections()) {
		std::pair<FGuid, FGuid> connection = std::make_pair(InputB->PinId, InputB->LinkedTo[0]->PinId);
		connections.Add(connection);
	}
	else {
		runtimeNode->b = GetDoubleDefaultValue(InputB);
	}
	idToPinMap.Add(InputB->PinId, RuntimePinB);
	runtimeNode->InputB = RuntimePinB;
	RuntimePinB->OwningNode = runtimeNode;

	//InputSmoothing
	UGeoFlowRuntimePin* RuntimePinS = NewObject<UGeoFlowRuntimePin>(runtimeNode);
	RuntimePinS->PinName = InputSmoothing->PinName;
	RuntimePinS->PinId = InputSmoothing->PinId;
	if (InputSmoothing->HasAnyConnections()) {
		std::pair<FGuid, FGuid> connection = std::make_pair(InputSmoothing->PinId, InputSmoothing->LinkedTo[0]->PinId);
		connections.Add(connection);
	}
	else {
		runtimeNode->smoothing = GetDoubleDefaultValue(InputSmoothing);
	}
	idToPinMap.Add(InputSmoothing->PinId, RuntimePinS);
	runtimeNode->InputSmoothing = RuntimePinS;
	RuntimePinS->OwningNode = runtimeNode;


	//Output
	UGeoFlowRuntimePin* OutputRuntimePin = NewObject<UGeoFlowRuntimePin>(runtimeNode);
	OutputRuntimePin->PinName = Output->PinName;
	OutputRuntimePin->PinId = Output->PinId;
	//connections check only on inputs
	idToPinMap.Add(Output->PinId, OutputRuntimePin);
	runtimeNode->Output = OutputRuntimePin;
	OutputRuntimePin->OwningNode = runtimeNode;

	return runtimeNode;
}


UGFN_E_Base* UGFN_R_Smin::CreateEditorNode(UEdGraph* _workingGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap<FGuid, UEdGraphPin*>& idToPinMap)
{
	auto newNode = InitUiNode<UGFN_E_Smin>(_workingGraph);

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
	UGFN_R_Smax* runtimeNode = NewObject<UGFN_R_Smax>(runtimeGraph);
	runtimeNode->Position = FVector2D(NodePosX, NodePosY);

	//InputA
	UGeoFlowRuntimePin* RuntimePinA = NewObject<UGeoFlowRuntimePin>(runtimeNode);
	RuntimePinA->PinName = InputA->PinName;
	RuntimePinA->PinId = InputA->PinId;
	if (InputA->HasAnyConnections()) {
		std::pair<FGuid, FGuid> connection = std::make_pair(InputA->PinId, InputA->LinkedTo[0]->PinId);
		connections.Add(connection);
	}
	else {
		//only updating default value when we have no connection *should* mean that it persists underneath
		//not entirely sure if this is true
		runtimeNode->a = GetDoubleDefaultValue(InputA);
	}
	idToPinMap.Add(InputA->PinId, RuntimePinA);
	runtimeNode->InputA = RuntimePinA;
	RuntimePinA->OwningNode = runtimeNode;

	//InputB
	UGeoFlowRuntimePin* RuntimePinB = NewObject<UGeoFlowRuntimePin>(runtimeNode);
	RuntimePinB->PinName = InputB->PinName;
	RuntimePinB->PinId = InputB->PinId;
	if (InputB->HasAnyConnections()) {
		std::pair<FGuid, FGuid> connection = std::make_pair(InputB->PinId, InputB->LinkedTo[0]->PinId);
		connections.Add(connection);
	}
	else {
		runtimeNode->b = GetDoubleDefaultValue(InputB);
	}
	idToPinMap.Add(InputB->PinId, RuntimePinB);
	runtimeNode->InputB = RuntimePinB;
	RuntimePinB->OwningNode = runtimeNode;

	//InputSmoothing
	UGeoFlowRuntimePin* RuntimePinS = NewObject<UGeoFlowRuntimePin>(runtimeNode);
	RuntimePinS->PinName = InputSmoothing->PinName;
	RuntimePinS->PinId = InputSmoothing->PinId;
	if (InputSmoothing->HasAnyConnections()) {
		std::pair<FGuid, FGuid> connection = std::make_pair(InputSmoothing->PinId, InputSmoothing->LinkedTo[0]->PinId);
		connections.Add(connection);
	}
	else {
		runtimeNode->smoothing = GetDoubleDefaultValue(InputSmoothing);
	}
	idToPinMap.Add(InputSmoothing->PinId, RuntimePinS);
	runtimeNode->InputSmoothing = RuntimePinS;
	RuntimePinS->OwningNode = runtimeNode;


	//Output
	UGeoFlowRuntimePin* OutputRuntimePin = NewObject<UGeoFlowRuntimePin>(runtimeNode);
	OutputRuntimePin->PinName = Output->PinName;
	OutputRuntimePin->PinId = Output->PinId;
	//connections check only on inputs
	idToPinMap.Add(Output->PinId, OutputRuntimePin);
	runtimeNode->Output = OutputRuntimePin;
	OutputRuntimePin->OwningNode = runtimeNode;

	return runtimeNode;
}


UGFN_E_Base* UGFN_R_Smax::CreateEditorNode(UEdGraph* _workingGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap<FGuid, UEdGraphPin*>& idToPinMap)
{
	auto newNode = InitUiNode<UGFN_E_Smax>(_workingGraph);

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
	//https://iquilezles.org/articles/Smax/
	double h = FMath::Clamp(0.5 + 0.5 * (b + a) / smoothing, 0.0, 1.0);
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
	UGFN_R_Min* runtimeNode = NewObject<UGFN_R_Min>(runtimeGraph);
	runtimeNode->Position = FVector2D(NodePosX, NodePosY);

	//InputA
	UGeoFlowRuntimePin* RuntimePinA = NewObject<UGeoFlowRuntimePin>(runtimeNode);
	RuntimePinA->PinName = InputA->PinName;
	RuntimePinA->PinId = InputA->PinId;
	if (InputA->HasAnyConnections()) {
		std::pair<FGuid, FGuid> connection = std::make_pair(InputA->PinId, InputA->LinkedTo[0]->PinId);
		connections.Add(connection);
	}
	else {
		//only updating default value when we have no connection *should* mean that it persists underneath
		//not entirely sure if this is true
		runtimeNode->a = GetDoubleDefaultValue(InputA);
	}
	idToPinMap.Add(InputA->PinId, RuntimePinA);
	runtimeNode->InputA = RuntimePinA;
	RuntimePinA->OwningNode = runtimeNode;

	//InputB
	UGeoFlowRuntimePin* RuntimePinB = NewObject<UGeoFlowRuntimePin>(runtimeNode);
	RuntimePinB->PinName = InputB->PinName;
	RuntimePinB->PinId = InputB->PinId;
	if (InputB->HasAnyConnections()) {
		std::pair<FGuid, FGuid> connection = std::make_pair(InputB->PinId, InputB->LinkedTo[0]->PinId);
		connections.Add(connection);
	}
	else {
		runtimeNode->b = GetDoubleDefaultValue(InputB);
	}
	idToPinMap.Add(InputB->PinId, RuntimePinB);
	runtimeNode->InputB = RuntimePinB;
	RuntimePinB->OwningNode = runtimeNode;

	//Output
	UGeoFlowRuntimePin* OutputRuntimePin = NewObject<UGeoFlowRuntimePin>(runtimeNode);
	OutputRuntimePin->PinName = Output->PinName;
	OutputRuntimePin->PinId = Output->PinId;
	//connections check only on inputs
	idToPinMap.Add(Output->PinId, OutputRuntimePin);
	runtimeNode->Output = OutputRuntimePin;
	OutputRuntimePin->OwningNode = runtimeNode;

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
	UGFN_R_Max* runtimeNode = NewObject<UGFN_R_Max>(runtimeGraph);
	runtimeNode->Position = FVector2D(NodePosX, NodePosY);

	//InputA
	UGeoFlowRuntimePin* RuntimePinA = NewObject<UGeoFlowRuntimePin>(runtimeNode);
	RuntimePinA->PinName = InputA->PinName;
	RuntimePinA->PinId = InputA->PinId;
	if (InputA->HasAnyConnections()) {
		std::pair<FGuid, FGuid> connection = std::make_pair(InputA->PinId, InputA->LinkedTo[0]->PinId);
		connections.Add(connection);
	}
	else {
		//only updating default value when we have no connection *should* mean that it persists underneath
		//not entirely sure if this is true
		runtimeNode->a = GetDoubleDefaultValue(InputA);
	}
	idToPinMap.Add(InputA->PinId, RuntimePinA);
	runtimeNode->InputA = RuntimePinA;
	RuntimePinA->OwningNode = runtimeNode;

	//InputB
	UGeoFlowRuntimePin* RuntimePinB = NewObject<UGeoFlowRuntimePin>(runtimeNode);
	RuntimePinB->PinName = InputB->PinName;
	RuntimePinB->PinId = InputB->PinId;
	if (InputB->HasAnyConnections()) {
		std::pair<FGuid, FGuid> connection = std::make_pair(InputB->PinId, InputB->LinkedTo[0]->PinId);
		connections.Add(connection);
	}
	else {
		runtimeNode->b = GetDoubleDefaultValue(InputB);
	}
	idToPinMap.Add(InputB->PinId, RuntimePinB);
	runtimeNode->InputB = RuntimePinB;
	RuntimePinB->OwningNode = runtimeNode;



	//Output
	UGeoFlowRuntimePin* OutputRuntimePin = NewObject<UGeoFlowRuntimePin>(runtimeNode);
	OutputRuntimePin->PinName = Output->PinName;
	OutputRuntimePin->PinId = Output->PinId;
	//connections check only on inputs
	idToPinMap.Add(Output->PinId, OutputRuntimePin);
	runtimeNode->Output = OutputRuntimePin;
	OutputRuntimePin->OwningNode = runtimeNode;

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
