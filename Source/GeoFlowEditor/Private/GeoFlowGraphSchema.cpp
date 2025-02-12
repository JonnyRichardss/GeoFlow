#include "GeoFlowGraphSchema.h"
#include "GeoFlowNodeTypes.h"
#include "GeoFlowPrimitiveNodes.h"
#include "GeoFlowConstantNodes.h"

//move this ty TODO
static void AddNewNodeAction(TArray<TSharedPtr<FNewNodeAction>>& AllActions, const AddNodeArgs& args) {
	AllActions.Emplace(new FNewNodeAction(
		args.TemplateClass,
		FText::FromString(args.Category),
		FText::FromString(args.Name),
		FText::FromString(TEXT("Makes a ")+args.Name+TEXT(" node.")),
		0
	));
}
void UGeoFlowGraphSchema::GetGraphContextActions(FGraphContextMenuBuilder& contextMenuBuilder) const
{

	TArray<TSharedPtr<FNewNodeAction>> AllActions;
	for (int i = 0; i < (int)EGeoFlowNodeType::Output; i++) {
		AddNewNodeAction(AllActions, EnumToAddNodeArgs((EGeoFlowNodeType)i));
	}
	//TODO maybe unmanualify this, it can probably be done by iterating over the enum
	
	for (auto action : AllActions) {
		contextMenuBuilder.AddAction(action);
	}
}

const FPinConnectionResponse UGeoFlowGraphSchema::CanCreateConnection(const UEdGraphPin* a, const UEdGraphPin* b) const
{
	if (a->PinType.PinSubCategory != b->PinType.PinSubCategory) {
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT("Pins do not have same value type"));
	}
	if (a == nullptr || b == nullptr) {
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT("One or both pins are null"));
	}
	if (a->Direction == b->Direction) {
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT("Pins are the same direction"));
	}
	//if connection is allowed we want to save the graph (it will be incorrectly saved but the modify time will change which will trigger correct saving later)
	//TODO

	//always want to ONLY break the inputs connections
	//therefore outputs can go to multiple places but each input only allows one
	if (a->Direction == EEdGraphPinDirection::EGPD_Input) {
		return FPinConnectionResponse(CONNECT_RESPONSE_BREAK_OTHERS_A, TEXT(""));
	}
	else {//(a->Direction == EEdGraphPinDirection::EGPD_Output)
		return FPinConnectionResponse(CONNECT_RESPONSE_BREAK_OTHERS_B, TEXT(""));
	}
}

void UGeoFlowGraphSchema::CreateDefaultNodesForGraph(UEdGraph& graph) const
{
	UGFN_E_Output* endNode = NewObject<UGFN_E_Output>(&graph);
	endNode->CreateNewGuid();
	endNode->NodePosX = 0;
	endNode->NodePosY = 0;
	endNode->CreateInputPins(nullptr);
	endNode->CreateOutputPins();
	graph.Modify();
	graph.AddNode(endNode, true, true);
}

bool UGeoFlowGraphSchema::ShouldHidePinDefaultValue(UEdGraphPin* Pin) const
{
	//temporarily left it like this
	//in future i might want to set up a whole system and let the schema handle a lot of logic for showhiding
	return Pin->GetOwningNode()->IsA<UGFN_E_Output>();
}



UEdGraphNode* FNewNodeAction::PerformAction(UEdGraph* parentGraph, UEdGraphPin* fromPin, const FVector2D location, bool bSelectNewNode)
{
	UGFN_E_Base* output = NewObject<UGFN_E_Base>(parentGraph,_templateClass);
	output->CreateNewGuid();
	output->NodePosX = location.X;
	output->NodePosY = location.Y;

	TArray<UEdGraphPin*> inputPins = output->CreateInputPins(fromPin);
	TArray<UEdGraphPin*> outputPins = output->CreateOutputPins();

	//keep trying to create a valid connection until either one is made or its not possible
	if (fromPin != nullptr) {
		bool connected = false;

		if (fromPin->Direction == EEdGraphPinDirection::EGPD_Output) {
			for (UEdGraphPin* pin : inputPins) {
				if (connected) break;
				connected = output->GetSchema()->TryCreateConnection(fromPin, pin);
			}
		}

		else {//(fromPin->Direction == EEdGraphPinDirection::EGPD_Input)
			for (UEdGraphPin* pin : outputPins) {
				if (connected) break;
				connected = output->GetSchema()->TryCreateConnection(fromPin, pin);
			}
		}
	}

	
	parentGraph->Modify();
	parentGraph->AddNode(output, true, true);
	return output;
}
