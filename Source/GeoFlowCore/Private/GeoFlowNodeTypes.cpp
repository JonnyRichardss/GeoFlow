#include "GeoFlowNodeTypes.h"
#include "GeoFlowRuntimeGraph.h"
//include all other node types for base GetClassFromNodeType
#include "GeoFlowConstantNodes.h"
#include "GeoFlowPrimitiveNodes.h"
/*BASE*/
void UGFN_E_Base::GetNodeContextMenuActions(UToolMenu* menu, UGraphNodeContextMenuContext* context) const
{
	FToolMenuSection& section = menu->AddSection(TEXT("Section"), FText::FromString(TEXT("Custom Node Actions")));
	//TODO find a real way to do this
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
	InputPins.Add(CreateCustomPin(EEdGraphPinDirection::EGPD_Input, GetNextInputName(), EGeoFlowReturnType::Double));
	InputPins.Add(CreateCustomPin(EEdGraphPinDirection::EGPD_Input, GetNextInputName(), EGeoFlowReturnType::Double));
	InputPins.Add(CreateCustomPin(EEdGraphPinDirection::EGPD_Input, GetNextInputName(), EGeoFlowReturnType::Double));

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
				node->CreateCustomPin(EEdGraphPinDirection::EGPD_Input, GetNextInputName(), EGeoFlowReturnType::Double);
				node->GetGraph()->NotifyGraphChanged();
				node->GetGraph()->Modify();
			}
		))
	);
}

UGFN_R_Base* UGFN_E_Output::CreateRuntimeNode(UGeoFlowRuntimeGraph* runtimeGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap<FGuid, UGeoFlowRuntimePin*>& idToPinMap)
{
	UGFN_R_Output* runtimeNode = NewObject<UGFN_R_Output>(runtimeGraph);
	runtimeNode->Position = FVector2D(NodePosX, NodePosY);

	for (UEdGraphPin* uiPin : Pins) {
		UGeoFlowRuntimePin* runtimePin = NewObject<UGeoFlowRuntimePin>(runtimeNode);
		runtimePin->PinName = uiPin->PinName;
		runtimePin->PinId = uiPin->PinId;

		// Only record the the input side of the connection since this is a directed graph
		//assuming one connection
		if (uiPin->HasAnyConnections() && uiPin->Direction == EEdGraphPinDirection::EGPD_Input) {
			std::pair<FGuid, FGuid> connection = std::make_pair(uiPin->PinId, uiPin->LinkedTo[0]->PinId);
			connections.Add(connection);
		}

		idToPinMap.Add(uiPin->PinId, runtimePin);
		if (uiPin->Direction == EEdGraphPinDirection::EGPD_Input) {
			runtimeNode->InputPins.Add(runtimePin);
		}
		runtimePin->OwningNode = runtimeNode;
	}
	return runtimeNode;
}
UGFN_E_Base* UGFN_R_Output::CreateEditorNode(UEdGraph* _workingGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap<FGuid, UEdGraphPin*>& idToPinMap)
{
	UGFN_E_Output* newNode = NewObject<UGFN_E_Output>(_workingGraph);
	newNode->CreateNewGuid();
	newNode->NodePosX = Position.X;
	newNode->NodePosY = Position.Y;

	for (UGeoFlowRuntimePin* runtimePin : InputPins) {
		UEdGraphPin* uiPin = newNode->CreateCustomPin(EEdGraphPinDirection::EGPD_Input, runtimePin->PinName, EGeoFlowReturnType::Double);
		uiPin->PinId = runtimePin->PinId;
		if (runtimePin->Connection != nullptr) {
			connections.Add(std::make_pair(runtimePin->PinId, runtimePin->Connection->PinId));
		}
		idToPinMap.Add(runtimePin->PinId, uiPin);
	}

	return newNode;
}
//TODO investigate whether its better to just have 1 input with multiple connections
//im pretty sure there can be a special case in the schema for it
double UGFN_R_Output::Evaluate(const FVector3d& pos)
{
	TArray<double> inputs;
	inputs.Reserve(InputPins.Num());
	for (UGeoFlowRuntimePin* pin : InputPins) {
		//assuming 1-1 connection on these pins
		if (pin->Connection != nullptr) {
			UGFN_R_BaseDouble* node = Cast<UGFN_R_BaseDouble>(pin->Connection->OwningNode);
			inputs.Add(node->Evaluate(pos));
		}
	}
	//i think this is the only way to do min for an arbitrary number of values
	double output = DBL_MAX;
	for (double num : inputs) {
		if (num < output) {
			output = num;
		}
	}
	return output;
}
