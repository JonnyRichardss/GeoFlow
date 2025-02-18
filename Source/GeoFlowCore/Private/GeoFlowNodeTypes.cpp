#include "GeoFlowNodeTypes.h"
#include "GeoFlowRuntimeGraph.h"
//include all other node types for base GetClassFromNodeType
#include "GeoFlowConstantNodes.h"
#include "GeoFlowPrimitiveNodes.h"

/*BASE*/
void UGFN_E_Base::GetNodeContextMenuActions(UToolMenu* menu, UGraphNodeContextMenuContext* context) const
{
	FToolMenuSection& section = menu->AddSection(TEXT("Section"), FText::FromString(TEXT("Custom Node Actions")));
	//TODO find a real way to do this (Type punning)
	UGFN_E_Base* node = (UGFN_E_Base*)this;//dont do this in actual code LMAO -- abusing the fact the function is const and getting side effects anyway
	if (CanUserDeleteNode()) {
		section.AddMenuEntry(
			TEXT("DeleteEntry"),
			FText::FromString(TEXT("Delete Node")),
			FText::FromString(TEXT("Deletes the node.")),
			FSlateIcon(TEXT("GeoFlowEditorStyle"), TEXT("GeoFlowEditor.NodeDeleteNodeIcon")),
			FUIAction(FExecuteAction::CreateLambda(
				[node]() {
					node->GetGraph()->RemoveNode(node);
				}
			))
		);
	}
}
UGeoFlowRuntimePin* UGFN_E_Base::InitRuntimePin(UGFN_R_Base* runtimeNode, UEdGraphPin* UiPin, TArray<std::pair<FGuid, FGuid>>& connections, TMap<FGuid, UGeoFlowRuntimePin*>& idToPinMap, EGeoFlowReturnType returnType)
{
	UGeoFlowRuntimePin* runtimePin = NewObject<UGeoFlowRuntimePin>(runtimeNode);
	runtimePin->Direction = UiPin->Direction;
	runtimePin->ReturnType = returnType;
	runtimePin->PinName = UiPin->PinName;
	runtimePin->PinId = UiPin->PinId;
	if (UiPin->HasAnyConnections() && UiPin->Direction == EEdGraphPinDirection::EGPD_Input) {
		std::pair<FGuid, FGuid> connection = std::make_pair(UiPin->PinId, UiPin->LinkedTo[0]->PinId);
		connections.Add(connection);
	}
	idToPinMap.Add(UiPin->PinId, runtimePin);
	runtimePin->OwningNode = runtimeNode;
	return runtimePin;
}
UEdGraphPin* UGFN_E_Base::CreateCustomPin(EEdGraphPinDirection direction, FName name, EGeoFlowReturnType ConnectionType)
{
	FName category = (direction == EEdGraphPinDirection::EGPD_Input) ? TEXT("Inputs") : TEXT("Outputs");
	FName subcategory = PinNameFromEnum(ConnectionType);

	UEdGraphPin* pin = CreatePin(
		direction,
		category,
		name
	);
	pin->PinType.PinSubCategory = subcategory;
	return pin;
}


/*OUTPUT*/
FName UGFN_E_Output::GetNextInputName() const
{
	int numInputs = 1;
	for (UEdGraphPin* pin : Pins) {
		if (pin->Direction == EGPD_Input) numInputs++;
	}
	return FName(FString::Printf(TEXT("%i"), numInputs));
}

TArray<UEdGraphPin*> UGFN_E_Output::CreateInputPins(UEdGraphPin* fromPin)
{
	TArray<UEdGraphPin*> InputPins;
	InputPins.Add(CreateCustomPin(EEdGraphPinDirection::EGPD_Input, GetNextInputName(), EGeoFlowReturnType::Float));
	InputPins.Add(CreateCustomPin(EEdGraphPinDirection::EGPD_Input, GetNextInputName(), EGeoFlowReturnType::Float));
	InputPins.Add(CreateCustomPin(EEdGraphPinDirection::EGPD_Input, GetNextInputName(), EGeoFlowReturnType::Float));

	return InputPins;
}

void UGFN_E_Output::GetNodeContextMenuActions(UToolMenu* menu, UGraphNodeContextMenuContext* context) const
{
	FToolMenuSection& section = menu->AddSection(TEXT("Section"), FText::FromString(TEXT("Custom Node Actions")));
	//TODO find a real way to do this
	UGFN_E_Base* node = (UGFN_E_Base*)this;//dont do this in actual code LMAO -- abusing the fact the function is const and getting side effects anyway
	
	section.AddMenuEntry(
		TEXT("AddPinEntry"),
		FText::FromString(TEXT("Add Input")),
		FText::FromString(TEXT("Creates a new input")),
		FSlateIcon(TEXT("CustomAssetEditorStyle"), TEXT("CustomAssetEditor.NodeAddPinIcon")),
		FUIAction(FExecuteAction::CreateLambda(
			[node,this]() {
				node->CreateCustomPin(EEdGraphPinDirection::EGPD_Input, GetNextInputName(), EGeoFlowReturnType::Float);
				node->GetGraph()->NotifyGraphChanged();
				node->GetGraph()->Modify();
			}
		))
	);
}

UGFN_R_Base* UGFN_E_Output::CreateRuntimeNode(UGeoFlowRuntimeGraph* runtimeGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap<FGuid, UGeoFlowRuntimePin*>& idToPinMap)
{
	UGFN_R_Output* runtimeNode = InitRuntimeNode<UGFN_R_Output>(runtimeGraph);
	for (UEdGraphPin* uiPin : Pins) {
		UGeoFlowRuntimePin* runtimePin = InitRuntimePin(runtimeNode,uiPin,connections,idToPinMap,EGeoFlowReturnType::Float);
		if (uiPin->Direction == EEdGraphPinDirection::EGPD_Input) {
			runtimeNode->InputPins.Add(runtimePin);
		}
	}
	return runtimeNode;
}
UGFN_E_Base* UGFN_R_Output::CreateEditorNode(UEdGraph* _workingGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap<FGuid, UEdGraphPin*>& idToPinMap)
{
	UGFN_E_Output* newNode = InitUiNode<UGFN_E_Output>(_workingGraph);
	for (UGeoFlowRuntimePin* runtimePin : InputPins) {
		InitUiPin(newNode, runtimePin, connections, idToPinMap);
	}
	return newNode;
}
//TODO investigate whether its better to just have 1 input with multiple connections
//im pretty sure there can be a special case in the schema for it
float UGFN_R_Output::Evaluate(const FVector3f& pos)
{
	TArray<float> inputs;
	inputs.Reserve(InputPins.Num());
	for (UGeoFlowRuntimePin* pin : InputPins) {
		//assuming 1-1 connection on these pins
		if (pin->Connection != nullptr) {
			UGFN_R_BaseFloat* node = Cast<UGFN_R_BaseFloat>(pin->Connection->OwningNode);
			//if cast succeeded -- i think this check is technically redundant due to the schema but better safe than crashing
			if (node != nullptr) inputs.Add(node->Evaluate(pos));
		}
	}
	//i think this is the only way to do min for an arbitrary number of values
	float output = FLT_MAX;
	for (float num : inputs) {
		if (num < output) {
			output = num;
		}
	}
	return output;
}

FString UGFN_R_Output::CreateShaderEvalCall(TArray<FString>& PinDeclarations)
{
	FString output;
	
	TArray<FString> PinVars;
	for (UGeoFlowRuntimePin* pin : InputPins) {
		//assuming 1-1 connection on these pins
		if (pin->Connection != nullptr) {
			UGFN_R_BaseFloat* node = Cast<UGFN_R_BaseFloat>(pin->Connection->OwningNode);
			//if cast succeeded -- i think this check is technically redundant due to the schema but better safe than crashing
			if (node != nullptr) {
				//add node as a variable
				//add code to choose min of vars
				FString pinVar;
				pinVar.Appendf(TEXT("PIN_%s"), *(pin->PinId.ToString(EGuidFormats::DigitsLower)));
				PinVars.Add(pinVar);
				PinDeclarations.Add(FString::Printf(TEXT("float %s = %s;\n"),*pinVar, *(node->CreateShaderEvalCall(PinDeclarations))));
			}
		}
	}
	//special cases for 0, 1 connections
	//after that just max the numbers LOL
	int intNumber = 0;
	do {
		if (PinVars.Num() == 0) {
			output.Appendf(TEXT("return FLT_MAX;"));
		}
		else if (PinVars.Num() == 1) {
			FString OurVar = PinVars.Pop(EAllowShrinking::No);
			output.Appendf(TEXT("return %s;\n"), *OurVar);
		}
		else{
			FString NewVar;
			FString OurVarA = PinVars.Pop(EAllowShrinking::No);
			FString OurVarB = PinVars.Pop(EAllowShrinking::No);
			NewVar.Appendf(TEXT("Intermediate_%i"), intNumber++);
			output.Appendf(TEXT("float %s = min(%s, %s);\n"), *NewVar, *OurVarA, *OurVarB);
			PinVars.Insert(NewVar, 0);
		}
	} while (PinVars.Num() > 0);

	return output;
}

UEdGraphPin* UGFN_R_Base::InitUiPin(UGFN_E_Base* newNode,UGeoFlowRuntimePin* runtimePin, TArray<std::pair<FGuid, FGuid>>& connections, TMap < FGuid, UEdGraphPin*>& idToPinMap)
{
	UEdGraphPin* UiPin = newNode->CreateCustomPin(runtimePin->Direction, runtimePin->PinName, runtimePin->ReturnType);
	UiPin->PinId = runtimePin->PinId;
	if (runtimePin->Connection != nullptr) {
		connections.Add(std::make_pair(runtimePin->PinId, runtimePin->Connection->PinId));
	}
	idToPinMap.Add(runtimePin->PinId, UiPin);
	return UiPin;
}
