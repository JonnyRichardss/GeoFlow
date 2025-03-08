#include "Nodes/GeoFlowPlacementNodes.h"
#include "GeoFlowRuntimeGraph.h"
#include "GeoFlowPinDefaultValueOps.h"
UGFN_R_Base* UGFN_E_Mirror::CreateRuntimeNode(UGeoFlowRuntimeGraph* runtimeGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap<FGuid, UGeoFlowRuntimePin*>& idToPinMap)
{
	UGFN_R_Mirror* runtimeNode = InitRuntimeNode<UGFN_R_Mirror>(runtimeGraph);

	UGeoFlowRuntimePin* PositionRuntimePin = InitRuntimePin(runtimeNode, PositionInput, connections, idToPinMap, EGeoFlowReturnType::Vector);
	runtimeNode->position = GetVectorDefaultValue(PositionInput);
	runtimeNode->PositionInput = PositionRuntimePin;
	UGeoFlowRuntimePin* RotationRuntimePin = InitRuntimePin(runtimeNode, RotationInput, connections, idToPinMap, EGeoFlowReturnType::Vector);
	runtimeNode->rotation = GetVectorDefaultValue(RotationInput);
	runtimeNode->RotationInput = RotationRuntimePin;
	runtimeNode->mirrorX = mirrorX;
	runtimeNode->mirrorY = mirrorY;
	runtimeNode->mirrorZ = mirrorZ;
	//Output
	UGeoFlowRuntimePin* OutputRuntimePin = InitRuntimePin(runtimeNode, Output, connections, idToPinMap, EGeoFlowReturnType::Array);
	runtimeNode->Output = OutputRuntimePin;

	return runtimeNode;
}

TArray<UEdGraphPin*> UGFN_E_Mirror::CreateInputPins(UEdGraphPin* fromPin)
{
	TArray<UEdGraphPin*> output;
	PositionInput = CreateCustomPin(EGPD_Input, "Position", EGeoFlowReturnType::Vector);
	RotationInput = CreateCustomPin(EGPD_Input, "Rotation", EGeoFlowReturnType::Vector);
	output.Add(PositionInput);
	output.Add(RotationInput);
	XPin = CreateCustomPin(EGPD_Input, "X", EGeoFlowReturnType::Bool);
	YPin = CreateCustomPin(EGPD_Input, "Y", EGeoFlowReturnType::Bool);
	ZPin = CreateCustomPin(EGPD_Input, "Z", EGeoFlowReturnType::Bool);
	XPin->bNotConnectable = true;
	YPin->bNotConnectable = true;
	ZPin->bNotConnectable = true;
	output.Add(XPin);
	output.Add(YPin);
	output.Add(ZPin);
	return output;
}

UGFN_E_Base* UGFN_R_Mirror::CreateEditorNode(UEdGraph* _workingGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap<FGuid, UEdGraphPin*>& idToPinMap)
{
	UGFN_E_Mirror* newNode = InitUiNode<UGFN_E_Mirror>(_workingGraph);


	UEdGraphPin* UiPinPosition = InitUiPin(newNode, PositionInput, connections, idToPinMap);
	newNode->PositionInput = UiPinPosition;
	SetVectorDefaultValue(UiPinPosition, position);
	UEdGraphPin* UiPinRotation = InitUiPin(newNode, RotationInput, connections, idToPinMap);
	newNode->RotationInput = UiPinRotation;
	SetVectorDefaultValue(UiPinRotation, rotation);

	newNode->XPin = newNode->CreateCustomPin(EGPD_Input, "X", EGeoFlowReturnType::Bool);
	newNode->YPin = newNode->CreateCustomPin(EGPD_Input, "Y", EGeoFlowReturnType::Bool);
	newNode->ZPin = newNode->CreateCustomPin(EGPD_Input, "Z", EGeoFlowReturnType::Bool);
	newNode->XPin->bNotConnectable = true;
	newNode->YPin->bNotConnectable = true;
	newNode->ZPin->bNotConnectable = true;

	newNode->mirrorX = mirrorX;
	newNode->mirrorY = mirrorY;
	newNode->mirrorZ = mirrorZ;
	SetBoolDefaultValue(newNode->XPin, mirrorX);
	SetBoolDefaultValue(newNode->YPin, mirrorY);
	SetBoolDefaultValue(newNode->ZPin, mirrorZ);

	//output
	UEdGraphPin* OutputUiPin = InitUiPin(newNode, Output, connections, idToPinMap);
	newNode->Output = OutputUiPin;
	return newNode;
}

TArray<FGeoFlowTransform> UGFN_R_Mirror::Evaluate(const FVector3f& pos)
{
	TArray<FGeoFlowTransform> output;
	if (PositionInput->Connection != nullptr) {
		UGFN_R_BaseVector* node = Cast<UGFN_R_BaseVector>(PositionInput->Connection->OwningNode);
		position = node->Evaluate(pos);
	}
	if (RotationInput->Connection != nullptr) {
		UGFN_R_BaseVector* node = Cast<UGFN_R_BaseVector>(RotationInput->Connection->OwningNode);
		rotation = node->Evaluate(pos);
	}
	output.Emplace(position,rotation);
	float posx = mirrorX ? 0.0f - position.X : position.X;
	float posy = mirrorY ? 0.0f - position.Y : position.Y;
	float posz = mirrorZ ? 0.0f - position.Z : position.Z;

	float rotx = mirrorX ? 0.0f - rotation.X : rotation.X;
	float roty = mirrorY ? 0.0f - rotation.Y : rotation.Y;
	float rotz = mirrorZ ? 0.0f - rotation.Z : rotation.Z;
	output.Emplace(FVector3f(posx,posy,posz),FVector3f(rotx,roty,rotz));
	return output;
}

UGFN_R_Base* UGFN_E_Grid::CreateRuntimeNode(UGeoFlowRuntimeGraph* runtimeGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap<FGuid, UGeoFlowRuntimePin*>& idToPinMap)
{
	UGFN_R_Grid* runtimeNode = InitRuntimeNode<UGFN_R_Grid>(runtimeGraph);

	UGeoFlowRuntimePin* PositionRuntimePin = InitRuntimePin(runtimeNode, PositionInput, connections, idToPinMap, EGeoFlowReturnType::Vector);
	runtimeNode->position = GetVectorDefaultValue(PositionInput);
	runtimeNode->PositionInput = PositionRuntimePin;
	UGeoFlowRuntimePin* RotationRuntimePin = InitRuntimePin(runtimeNode, RotationInput, connections, idToPinMap, EGeoFlowReturnType::Vector);
	runtimeNode->rotation = GetVectorDefaultValue(RotationInput);
	runtimeNode->RotationInput = RotationRuntimePin;
	runtimeNode->copies = copies;
	runtimeNode->spacing = spacing;
	//Output
	UGeoFlowRuntimePin* OutputRuntimePin = InitRuntimePin(runtimeNode, Output, connections, idToPinMap, EGeoFlowReturnType::Array);
	runtimeNode->Output = OutputRuntimePin;

	return runtimeNode;
}

TArray<UEdGraphPin*> UGFN_E_Grid::CreateInputPins(UEdGraphPin* fromPin)
{
	TArray<UEdGraphPin*> output;
	PositionInput = CreateCustomPin(EGPD_Input, "Position", EGeoFlowReturnType::Vector);
	RotationInput = CreateCustomPin(EGPD_Input, "Rotation", EGeoFlowReturnType::Vector);
	output.Add(PositionInput);
	output.Add(RotationInput);
	CopiesPin = CreateCustomPin(EGPD_Input, "Copies", EGeoFlowReturnType::Vector);
	SpacingPin = CreateCustomPin(EGPD_Input, "Spacing", EGeoFlowReturnType::Vector);
	CopiesPin->bNotConnectable = true;
	SpacingPin->bNotConnectable = true;
	output.Add(CopiesPin);
	output.Add(SpacingPin);
	return output;
}

UGFN_E_Base* UGFN_R_Grid::CreateEditorNode(UEdGraph* _workingGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap<FGuid, UEdGraphPin*>& idToPinMap)
{
	UGFN_E_Grid* newNode = InitUiNode<UGFN_E_Grid>(_workingGraph);


	UEdGraphPin* UiPinPosition = InitUiPin(newNode, PositionInput, connections, idToPinMap);
	newNode->PositionInput = UiPinPosition;
	SetVectorDefaultValue(UiPinPosition, position);
	UEdGraphPin* UiPinRotation = InitUiPin(newNode, RotationInput, connections, idToPinMap);
	newNode->RotationInput = UiPinRotation;
	SetVectorDefaultValue(UiPinRotation, rotation);

	newNode->CopiesPin = newNode->CreateCustomPin(EGPD_Input, "Copies", EGeoFlowReturnType::Vector);
	newNode->SpacingPin = newNode->CreateCustomPin(EGPD_Input, "Spacing", EGeoFlowReturnType::Vector);
	newNode->CopiesPin->bNotConnectable = true;
	newNode->SpacingPin->bNotConnectable = true;

	newNode->copies = copies;
	newNode->spacing = spacing;
	SetIntVectorDefaultValue(newNode->CopiesPin, copies);
	SetVectorDefaultValue(newNode->SpacingPin, spacing);
	//output
	UEdGraphPin* OutputUiPin = InitUiPin(newNode, Output, connections, idToPinMap);
	newNode->Output = OutputUiPin;
	return newNode;
}

TArray<FGeoFlowTransform> UGFN_R_Grid::Evaluate(const FVector3f& pos)
{
	TArray<FGeoFlowTransform> output;
	if (PositionInput->Connection != nullptr) {
		UGFN_R_BaseVector* node = Cast<UGFN_R_BaseVector>(PositionInput->Connection->OwningNode);
		position = node->Evaluate(pos);
	}
	if (RotationInput->Connection != nullptr) {
		UGFN_R_BaseVector* node = Cast<UGFN_R_BaseVector>(RotationInput->Connection->OwningNode);
		rotation = node->Evaluate(pos);
	}
	for (int i = 0; i < copies.X; i++) {
		for (int j = 0; j < copies.Y; j++) {
			for (int k = 0; k < copies.Z; k++) {
				FVector3f newPos = FVector3f(position.X + (i * spacing.X), position.Y + (j * spacing.Y), position.Z + (k * spacing.Z));
				output.Emplace(newPos, rotation);
			}
		}
	}
	return output;
}
