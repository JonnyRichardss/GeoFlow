#include "Nodes/GeoFlowPrimitiveNodes.h"
#include "GeoFlowRuntimeGraph.h"
#include "GeoFlowVectorMath.h"

//https://iquilezles.org/articles/distfunctions/
//for all evals

FVector3f UGFN_R_BasePrimitive::GetPosition()
{
	if (PositionInput->Connection != nullptr) {
		UGFN_R_BaseVector* node = Cast<UGFN_R_BaseVector>(PositionInput->Connection->OwningNode);
		//this isnt technically very correct BUT i'm fairly certain that changing the positon of a primitive
		//based on where its sampled from can't produce anything except garbage
		position = node->Evaluate(FVector3f(0.0f));
	}
	return position;
}
TArray<UEdGraphPin*> UGFN_E_PrimitiveSphere::CreateInputPins(UEdGraphPin* fromPin)
{
	TArray<UEdGraphPin*> InputPins;
	PositionInput = CreateCustomPin(EGPD_Input, "Position", EGeoFlowReturnType::Vector);
	RotationInput = CreateCustomPin(EGPD_Input, "Rotation", EGeoFlowReturnType::Vector);
	RadiusInput = CreateCustomPin(EGPD_Input, "Radius", EGeoFlowReturnType::Float);
	InputPins.Add(PositionInput);
	InputPins.Add(RotationInput);
	InputPins.Add(RadiusInput);
	return InputPins;
}
UGFN_R_Base* UGFN_E_PrimitiveSphere::CreateRuntimeNode(UGeoFlowRuntimeGraph* runtimeGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap<FGuid, UGeoFlowRuntimePin*>& idToPinMap)
{
	UGFN_R_PrimitiveSphere* runtimeNode = InitRuntimeNode<UGFN_R_PrimitiveSphere>(runtimeGraph);
	//PositionInput
	UGeoFlowRuntimePin* positionRuntimePin = InitRuntimePin(runtimeNode, PositionInput, connections, idToPinMap, EGeoFlowReturnType::Vector);
	runtimeNode->position = GetVectorDefaultValue(PositionInput);
	runtimeNode->PositionInput = positionRuntimePin;
	//RotationInput
	UGeoFlowRuntimePin* rotationRuntimePin = InitRuntimePin(runtimeNode, RotationInput, connections, idToPinMap, EGeoFlowReturnType::Vector);
	runtimeNode->rotation = GetVectorDefaultValue(RotationInput);
	runtimeNode->RotationInput = rotationRuntimePin;
	//RadiusInput
	UGeoFlowRuntimePin* RadiusRuntimePin = InitRuntimePin(runtimeNode, RadiusInput, connections, idToPinMap, EGeoFlowReturnType::Float); 
	runtimeNode->radius = GetFloatDefaultValue(RadiusInput);
	runtimeNode->RadiusInput = RadiusRuntimePin;
	//Output
	UGeoFlowRuntimePin* OutputRuntimePin = InitRuntimePin(runtimeNode, Output, connections, idToPinMap, EGeoFlowReturnType::Float);
	runtimeNode->Output = OutputRuntimePin;

	return runtimeNode;
}

UGFN_E_Base* UGFN_R_PrimitiveSphere::CreateEditorNode(UEdGraph* _workingGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap<FGuid, UEdGraphPin*>& idToPinMap)
{
	UGFN_E_PrimitiveSphere* newNode = InitUiNode<UGFN_E_PrimitiveSphere>(_workingGraph);
	//PositionInput
	UEdGraphPin* positionUiPin = InitUiPin(newNode, PositionInput, connections, idToPinMap);
	newNode->PositionInput = positionUiPin;
	newNode->position = position;
	SetVectorDefaultValue(positionUiPin, position);
	//RotationInput
	UEdGraphPin* rotationUiPin = InitUiPin(newNode, RotationInput, connections, idToPinMap);
	newNode->RotationInput = rotationUiPin;
	newNode->rotation = rotation;
	SetVectorDefaultValue(rotationUiPin, rotation);
	//radiusinput
	UEdGraphPin* RadiusUiPin = InitUiPin(newNode, RadiusInput, connections, idToPinMap);
	newNode->RadiusInput = RadiusUiPin;
	newNode->radius = radius;
	SetFloatDefaultValue(RadiusUiPin, radius);
	//output
	UEdGraphPin* OutputUiPin = InitUiPin(newNode, Output, connections, idToPinMap);
	newNode->Output = OutputUiPin;

	return newNode;
}

float UGFN_R_PrimitiveSphere::Evaluate(const FVector3f& pos)
{
	//TODO NEED A WAY TO FAIL TO EVALUATE GRAPH
	//99% some sort of exception structure is the way to go
	//just catch it all the way up before the first Evaluate() call


	if (PositionInput->Connection != nullptr) {
		UGFN_R_BaseVector* node = Cast<UGFN_R_BaseVector>(PositionInput->Connection->OwningNode);
		position = node->Evaluate(pos);
	}
	if (RotationInput->Connection != nullptr) {
		UGFN_R_BaseVector* node = Cast<UGFN_R_BaseVector>(RotationInput->Connection->OwningNode);
		rotation = node->Evaluate(pos);
	}
	if (RadiusInput->Connection != nullptr) {
		UGFN_R_BaseFloat* node = Cast<UGFN_R_BaseFloat>(RadiusInput->Connection->OwningNode);
		radius = node->Evaluate(pos);
	}

	FVector3f transformedPos = pos - position;
	transformedPos = FRotator3f::MakeFromEuler(rotation).RotateVector(transformedPos);
	

	return transformedPos.Length() - radius;

	//return (transformedPos.X * transformedPos.X) + (transformedPos.Y * transformedPos.Y) + (transformedPos.Z * transformedPos.Z) - (radius * radius);

}


TArray<UEdGraphPin*> UGFN_E_PrimitiveCube::CreateInputPins(UEdGraphPin* fromPin)
{
	TArray<UEdGraphPin*> InputPins;
	PositionInput = CreateCustomPin(EGPD_Input, "Position", EGeoFlowReturnType::Vector);
	RotationInput = CreateCustomPin(EGPD_Input, "Rotation", EGeoFlowReturnType::Vector);
	RadiusInput = CreateCustomPin(EGPD_Input, "Size", EGeoFlowReturnType::Float);
	InputPins.Add(PositionInput);
	InputPins.Add(RotationInput);
	InputPins.Add(RadiusInput);
	return InputPins;
}
UGFN_R_Base* UGFN_E_PrimitiveCube::CreateRuntimeNode(UGeoFlowRuntimeGraph* runtimeGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap<FGuid, UGeoFlowRuntimePin*>& idToPinMap)
{
	UGFN_R_PrimitiveCube* runtimeNode = InitRuntimeNode<UGFN_R_PrimitiveCube>(runtimeGraph);
	//PositionInput
	UGeoFlowRuntimePin* positionRuntimePin = InitRuntimePin(runtimeNode, PositionInput, connections, idToPinMap, EGeoFlowReturnType::Vector);
	runtimeNode->position = GetVectorDefaultValue(PositionInput);
	runtimeNode->PositionInput = positionRuntimePin;
	//RotationInput
	UGeoFlowRuntimePin* rotationRuntimePin = InitRuntimePin(runtimeNode, RotationInput, connections, idToPinMap, EGeoFlowReturnType::Vector);
	runtimeNode->rotation = GetVectorDefaultValue(RotationInput);
	runtimeNode->RotationInput = rotationRuntimePin;
	//RadiusInput
	UGeoFlowRuntimePin* RadiusRuntimePin = InitRuntimePin(runtimeNode, RadiusInput, connections, idToPinMap, EGeoFlowReturnType::Float);
	runtimeNode->radius = GetFloatDefaultValue(RadiusInput);
	runtimeNode->RadiusInput = RadiusRuntimePin;
	//Output
	UGeoFlowRuntimePin* OutputRuntimePin = InitRuntimePin(runtimeNode, Output, connections, idToPinMap, EGeoFlowReturnType::Float);
	runtimeNode->Output = OutputRuntimePin;

	return runtimeNode;
}

UGFN_E_Base* UGFN_R_PrimitiveCube::CreateEditorNode(UEdGraph* _workingGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap<FGuid, UEdGraphPin*>& idToPinMap)
{
	UGFN_E_PrimitiveCube* newNode = InitUiNode<UGFN_E_PrimitiveCube>(_workingGraph);
	//PositionInput
	UEdGraphPin* positionUiPin = InitUiPin(newNode, PositionInput, connections, idToPinMap);
	newNode->PositionInput = positionUiPin;
	newNode->position = position;
	SetVectorDefaultValue(positionUiPin, position);
	//RotationInput
	UEdGraphPin* rotationUiPin = InitUiPin(newNode, RotationInput, connections, idToPinMap);
	newNode->RotationInput = rotationUiPin;
	newNode->rotation = rotation;
	SetVectorDefaultValue(rotationUiPin, rotation);
	//radiusinput
	UEdGraphPin* RadiusUiPin = InitUiPin(newNode, RadiusInput, connections, idToPinMap);
	newNode->RadiusInput = RadiusUiPin;
	newNode->radius = radius;
	SetFloatDefaultValue(RadiusUiPin, radius);
	//output
	UEdGraphPin* OutputUiPin = InitUiPin(newNode, Output, connections, idToPinMap);
	newNode->Output = OutputUiPin;

	return newNode;
}




float UGFN_R_PrimitiveCube::Evaluate(const FVector3f& pos)
{
	if (PositionInput->Connection != nullptr) {
		UGFN_R_BaseVector* node = Cast<UGFN_R_BaseVector>(PositionInput->Connection->OwningNode);
		position = node->Evaluate(pos);
	}
	if (RotationInput->Connection != nullptr) {
		UGFN_R_BaseVector* node = Cast<UGFN_R_BaseVector>(RotationInput->Connection->OwningNode);
		rotation = node->Evaluate(pos);
	}
	if (RadiusInput->Connection != nullptr) {
		UGFN_R_BaseFloat* node = Cast<UGFN_R_BaseFloat>(RadiusInput->Connection->OwningNode);
		radius = node->Evaluate(pos);
	}

	FVector3f transformedPos = pos - position;
	transformedPos = FRotator3f::MakeFromEuler(rotation).RotateVector(transformedPos);
	FVector3f q = CompWiseAbs(transformedPos) - FVector3f(radius);


	return CompWiseMax(q, FVector3f(0.0)).Length() + FMath::Min(FMath::Max(q.X, FMath::Max(q.Y, q.Z)), 0.0);
}


TArray<UEdGraphPin*> UGFN_E_PrimitiveBox::CreateInputPins(UEdGraphPin* fromPin)
{
	TArray<UEdGraphPin*> InputPins;
	PositionInput = CreateCustomPin(EGPD_Input, "Position", EGeoFlowReturnType::Vector);
	RotationInput = CreateCustomPin(EGPD_Input, "Rotation", EGeoFlowReturnType::Vector);
	RadiusInput = CreateCustomPin(EGPD_Input, "Size XYZ", EGeoFlowReturnType::Vector);
	InputPins.Add(PositionInput);
	InputPins.Add(RotationInput);
	InputPins.Add(RadiusInput);
	return InputPins;
}
UGFN_R_Base* UGFN_E_PrimitiveBox::CreateRuntimeNode(UGeoFlowRuntimeGraph* runtimeGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap<FGuid, UGeoFlowRuntimePin*>& idToPinMap)
{
	UGFN_R_PrimitiveBox* runtimeNode = InitRuntimeNode<UGFN_R_PrimitiveBox>(runtimeGraph);
	//PositionInput
	UGeoFlowRuntimePin* positionRuntimePin = InitRuntimePin(runtimeNode, PositionInput, connections, idToPinMap, EGeoFlowReturnType::Vector);
	runtimeNode->position = GetVectorDefaultValue(PositionInput);
	runtimeNode->PositionInput = positionRuntimePin;
	//RotationInput
	UGeoFlowRuntimePin* rotationRuntimePin = InitRuntimePin(runtimeNode, RotationInput, connections, idToPinMap, EGeoFlowReturnType::Vector);
	runtimeNode->rotation = GetVectorDefaultValue(RotationInput);
	runtimeNode->RotationInput = rotationRuntimePin;
	//RadiusInput
	UGeoFlowRuntimePin* RadiusRuntimePin = InitRuntimePin(runtimeNode, RadiusInput, connections, idToPinMap, EGeoFlowReturnType::Vector);
	runtimeNode->radius = GetVectorDefaultValue(RadiusInput);
	runtimeNode->RadiusInput = RadiusRuntimePin;
	//Output
	UGeoFlowRuntimePin* OutputRuntimePin = InitRuntimePin(runtimeNode, Output, connections, idToPinMap, EGeoFlowReturnType::Float);
	runtimeNode->Output = OutputRuntimePin;

	return runtimeNode;
}

UGFN_E_Base* UGFN_R_PrimitiveBox::CreateEditorNode(UEdGraph* _workingGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap<FGuid, UEdGraphPin*>& idToPinMap)
{
	UGFN_E_PrimitiveBox* newNode = InitUiNode<UGFN_E_PrimitiveBox>(_workingGraph);
	//PositionInput
	UEdGraphPin* positionUiPin = InitUiPin(newNode, PositionInput, connections, idToPinMap);
	newNode->PositionInput = positionUiPin;
	newNode->position = position;
	SetVectorDefaultValue(positionUiPin, position);
	//RotationInput
	UEdGraphPin* rotationUiPin = InitUiPin(newNode, RotationInput, connections, idToPinMap);
	newNode->RotationInput = rotationUiPin;
	newNode->rotation = rotation;
	SetVectorDefaultValue(rotationUiPin, rotation);
	//radiusinput
	UEdGraphPin* RadiusUiPin = InitUiPin(newNode, RadiusInput, connections, idToPinMap);
	newNode->RadiusInput = RadiusUiPin;
	newNode->radius = radius;
	SetVectorDefaultValue(RadiusUiPin, radius);
	//output
	UEdGraphPin* OutputUiPin = InitUiPin(newNode, Output, connections, idToPinMap);
	newNode->Output = OutputUiPin;
	
	return newNode;
}



float UGFN_R_PrimitiveBox::Evaluate(const FVector3f& pos)
{
	if (PositionInput->Connection != nullptr) {
		UGFN_R_BaseVector* node = Cast<UGFN_R_BaseVector>(PositionInput->Connection->OwningNode);
		position = node->Evaluate(pos);
	}
	if (RotationInput->Connection != nullptr) {
		UGFN_R_BaseVector* node = Cast<UGFN_R_BaseVector>(RotationInput->Connection->OwningNode);
		rotation = node->Evaluate(pos);
	}
	if (RadiusInput->Connection != nullptr) {
		UGFN_R_BaseVector* node = Cast<UGFN_R_BaseVector>(RadiusInput->Connection->OwningNode);
		radius = node->Evaluate(pos);
	}

	FVector3f transformedPos = pos - position;
	transformedPos = FRotator3f::MakeFromEuler(rotation).RotateVector(transformedPos);
	FVector3f q = CompWiseAbs(transformedPos) - radius;
	

	return CompWiseMax(q, FVector3f(0.0)).Length() + FMath::Min(FMath::Max(q.X, FMath::Max(q.Y, q.Z)), 0.0);
}


TArray<UEdGraphPin*> UGFN_E_PrimitiveEllipsoid::CreateInputPins(UEdGraphPin* fromPin)
{
	TArray<UEdGraphPin*> InputPins;
	PositionInput = CreateCustomPin(EGPD_Input, "Position", EGeoFlowReturnType::Vector);
	RotationInput = CreateCustomPin(EGPD_Input, "Rotation", EGeoFlowReturnType::Vector);
	RadiusInput = CreateCustomPin(EGPD_Input, "Radius XYZ", EGeoFlowReturnType::Vector);
	InputPins.Add(PositionInput);
	InputPins.Add(RotationInput);
	InputPins.Add(RadiusInput);
	return InputPins;
}
UGFN_R_Base* UGFN_E_PrimitiveEllipsoid::CreateRuntimeNode(UGeoFlowRuntimeGraph* runtimeGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap<FGuid, UGeoFlowRuntimePin*>& idToPinMap)
{
	UGFN_R_PrimitiveEllipsoid* runtimeNode = InitRuntimeNode<UGFN_R_PrimitiveEllipsoid>(runtimeGraph);
	//PositionInput
	UGeoFlowRuntimePin* positionRuntimePin = InitRuntimePin(runtimeNode, PositionInput, connections, idToPinMap, EGeoFlowReturnType::Vector);
	runtimeNode->position = GetVectorDefaultValue(PositionInput);
	runtimeNode->PositionInput = positionRuntimePin;
	//RotationInput
	UGeoFlowRuntimePin* rotationRuntimePin = InitRuntimePin(runtimeNode, RotationInput, connections, idToPinMap, EGeoFlowReturnType::Vector);
	runtimeNode->rotation = GetVectorDefaultValue(RotationInput);
	runtimeNode->RotationInput = rotationRuntimePin;
	//RadiusInput
	UGeoFlowRuntimePin* RadiusRuntimePin = InitRuntimePin(runtimeNode, RadiusInput, connections, idToPinMap, EGeoFlowReturnType::Vector);
	runtimeNode->radius = GetVectorDefaultValue(RadiusInput);
	runtimeNode->RadiusInput = RadiusRuntimePin;
	//Output
	UGeoFlowRuntimePin* OutputRuntimePin = InitRuntimePin(runtimeNode, Output, connections, idToPinMap, EGeoFlowReturnType::Float);
	runtimeNode->Output = OutputRuntimePin;

	return runtimeNode;
}

UGFN_E_Base* UGFN_R_PrimitiveEllipsoid::CreateEditorNode(UEdGraph* _workingGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap<FGuid, UEdGraphPin*>& idToPinMap)
{
	UGFN_E_PrimitiveEllipsoid* newNode = InitUiNode<UGFN_E_PrimitiveEllipsoid>(_workingGraph);
	//PositionInput
	UEdGraphPin* positionUiPin = InitUiPin(newNode, PositionInput, connections, idToPinMap);
	newNode->PositionInput = positionUiPin;
	newNode->position = position;
	SetVectorDefaultValue(positionUiPin, position);
	//RotationInput
	UEdGraphPin* rotationUiPin = InitUiPin(newNode, RotationInput, connections, idToPinMap);
	newNode->RotationInput = rotationUiPin;
	newNode->rotation = rotation;
	SetVectorDefaultValue(rotationUiPin, rotation);
	//radiusinput
	UEdGraphPin* RadiusUiPin = InitUiPin(newNode, RadiusInput, connections, idToPinMap);
	newNode->RadiusInput = RadiusUiPin;
	newNode->radius = radius;
	SetVectorDefaultValue(RadiusUiPin, radius);
	//output
	UEdGraphPin* OutputUiPin = InitUiPin(newNode, Output, connections, idToPinMap);
	newNode->Output = OutputUiPin;

	return newNode;
}


float UGFN_R_PrimitiveEllipsoid::Evaluate(const FVector3f& pos)
{
	if (PositionInput->Connection != nullptr) {
		UGFN_R_BaseVector* node = Cast<UGFN_R_BaseVector>(PositionInput->Connection->OwningNode);
		position = node->Evaluate(pos);
	}
	if (RotationInput->Connection != nullptr) {
		UGFN_R_BaseVector* node = Cast<UGFN_R_BaseVector>(RotationInput->Connection->OwningNode);
		rotation = node->Evaluate(pos);
	}
	if (RadiusInput->Connection != nullptr) {
		UGFN_R_BaseVector* node = Cast<UGFN_R_BaseVector>(RadiusInput->Connection->OwningNode);
		radius = node->Evaluate(pos);
	}

	FVector3f transformedPos = pos - position;
	
	transformedPos = FRotator3f::MakeFromEuler(rotation).RotateVector(transformedPos);
	FVector3f k0Vec = transformedPos / radius;
	FVector3f k1Vec = transformedPos / (radius * radius);
	float k0 = k0Vec.Length();
	float k1 = k1Vec.Length();

	return k0 * (k0 - 1.0) / k1;
}


TArray<UEdGraphPin*> UGFN_E_PrimitiveCone::CreateInputPins(UEdGraphPin* fromPin)
{
	TArray<UEdGraphPin*> InputPins;
	PositionInput = CreateCustomPin(EGPD_Input, "Position", EGeoFlowReturnType::Vector);
	RotationInput = CreateCustomPin(EGPD_Input, "Rotation", EGeoFlowReturnType::Vector);
	AngleInput = CreateCustomPin(EGPD_Input, "Angle", EGeoFlowReturnType::Float);
	HeightInput = CreateCustomPin(EGPD_Input, "Height", EGeoFlowReturnType::Float);
	InputPins.Add(PositionInput);
	InputPins.Add(RotationInput);
	InputPins.Add(AngleInput);
	InputPins.Add(HeightInput);
	return InputPins;
}
UGFN_R_Base* UGFN_E_PrimitiveCone::CreateRuntimeNode(UGeoFlowRuntimeGraph* runtimeGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap<FGuid, UGeoFlowRuntimePin*>& idToPinMap)
{
	UGFN_R_PrimitiveCone* runtimeNode = InitRuntimeNode<UGFN_R_PrimitiveCone>(runtimeGraph);
	//PositionInput
	UGeoFlowRuntimePin* positionRuntimePin = InitRuntimePin(runtimeNode, PositionInput, connections, idToPinMap, EGeoFlowReturnType::Vector);
	runtimeNode->position = GetVectorDefaultValue(PositionInput);
	runtimeNode->PositionInput = positionRuntimePin;
	//RotationInput
	UGeoFlowRuntimePin* rotationRuntimePin = InitRuntimePin(runtimeNode, RotationInput, connections, idToPinMap, EGeoFlowReturnType::Vector);
	runtimeNode->rotation = GetVectorDefaultValue(RotationInput);
	runtimeNode->RotationInput = rotationRuntimePin;
	//AngleInput
	UGeoFlowRuntimePin* AngleRuntimePin = InitRuntimePin(runtimeNode, AngleInput, connections, idToPinMap, EGeoFlowReturnType::Float);
	runtimeNode->angle = GetFloatDefaultValue(AngleInput);
	runtimeNode->AngleInput = AngleRuntimePin;
	//HeightInput
	UGeoFlowRuntimePin* HeightRuntimePin = InitRuntimePin(runtimeNode, HeightInput, connections, idToPinMap, EGeoFlowReturnType::Float);
	runtimeNode->height = GetFloatDefaultValue(HeightInput);
	runtimeNode->HeightInput = HeightRuntimePin;
	//Output
	UGeoFlowRuntimePin* OutputRuntimePin = InitRuntimePin(runtimeNode, Output, connections, idToPinMap, EGeoFlowReturnType::Float);
	runtimeNode->Output = OutputRuntimePin;

	return runtimeNode;
}

UGFN_E_Base* UGFN_R_PrimitiveCone::CreateEditorNode(UEdGraph* _workingGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap<FGuid, UEdGraphPin*>& idToPinMap)
{
	UGFN_E_PrimitiveCone* newNode = InitUiNode<UGFN_E_PrimitiveCone>(_workingGraph);
	//PositionInput
	UEdGraphPin* positionUiPin = InitUiPin(newNode, PositionInput, connections, idToPinMap);
	newNode->PositionInput = positionUiPin;
	newNode->position = position;
	SetVectorDefaultValue(positionUiPin, position);
	//RotationInput
	UEdGraphPin* rotationUiPin = InitUiPin(newNode, RotationInput, connections, idToPinMap);
	newNode->RotationInput = rotationUiPin;
	newNode->rotation = rotation;
	SetVectorDefaultValue(rotationUiPin, rotation);
	//angleinput
	UEdGraphPin* AngleUIPin = InitUiPin(newNode, AngleInput, connections, idToPinMap);
	newNode->AngleInput = AngleUIPin;
	newNode->angle = angle;
	SetFloatDefaultValue(AngleUIPin, angle);
	//Heightinput
	UEdGraphPin* HeightUIPin = InitUiPin(newNode, HeightInput, connections, idToPinMap);
	newNode->HeightInput = HeightUIPin;
	newNode->height = height;
	SetFloatDefaultValue(HeightUIPin, height);
	//output
	UEdGraphPin* OutputUiPin = InitUiPin(newNode, Output, connections, idToPinMap);
	newNode->Output = OutputUiPin;

	return newNode;
}

float UGFN_R_PrimitiveCone::Evaluate(const FVector3f& pos)
{
	//TODO NEED A WAY TO FAIL TO EVALUATE GRAPH
	//99% some sort of exception structure is the way to go
	//just catch it all the way up before the first Evaluate() call


	if (PositionInput->Connection != nullptr) {
		UGFN_R_BaseVector* node = Cast<UGFN_R_BaseVector>(PositionInput->Connection->OwningNode);
		position = node->Evaluate(pos);
	}
	if (RotationInput->Connection != nullptr) {
		UGFN_R_BaseVector* node = Cast<UGFN_R_BaseVector>(RotationInput->Connection->OwningNode);
		rotation = node->Evaluate(pos);
	}
	if (AngleInput->Connection != nullptr) {
		UGFN_R_BaseFloat* node = Cast<UGFN_R_BaseFloat>(AngleInput->Connection->OwningNode);
		angle = node->Evaluate(pos);
	}
	if (HeightInput->Connection != nullptr) {
		UGFN_R_BaseFloat* node = Cast<UGFN_R_BaseFloat>(HeightInput->Connection->OwningNode);
		height = node->Evaluate(pos);
	}
	

	FVector3f transformedPos = pos - position;
	transformedPos = FRotator3f::MakeFromEuler(rotation).RotateVector(transformedPos);
	FVector2f c(FMath::Sin(FMath::DegreesToRadians(angle)), FMath::Cos(FMath::DegreesToRadians(angle)));
	
	//* exact version - hangs on certain angles idk why

	FVector2f q = height * FVector2f((c.X / c.Y), -1.0f);
	FVector2f w = FVector2f(FVector2f(transformedPos.X, transformedPos.Z).Length(), transformedPos.Y);
	FVector2f a = w - q * FMath::Clamp((FVector2f::DotProduct(w, q) / FVector2f::DotProduct(q, q)), 0.0f, 1.0f);
	FVector2f b = w - q * FVector2f((FMath::Clamp(w.X/q.X,0.0f,1.0f)), 1.0f);
	float k = FMath::Sign(q.Y);
	float d = FMath::Min(FVector2f::DotProduct(a, a), FVector2f::DotProduct(b, b));
	float s = FMath::Max(k * (w.X * q.Y - w.Y * q.X), k * (w.Y - q.Y));
	return FMath::Sqrt(d) * FMath::Sign(s);
	
	//float q = FVector2f(transformedPos.X, transformedPos.Z).Length();
	//return FMath::Max(FVector2f::DotProduct(c, FVector2f(q, transformedPos.Y)), 0.0f - height - transformedPos.Y);
}



TArray<UEdGraphPin*> UGFN_E_PrimitiveCylinder::CreateInputPins(UEdGraphPin* fromPin)
{
	TArray<UEdGraphPin*> InputPins;
	PositionInput = CreateCustomPin(EGPD_Input, "Position", EGeoFlowReturnType::Vector);
	RotationInput = CreateCustomPin(EGPD_Input, "Rotation", EGeoFlowReturnType::Vector);
	RadiusInput = CreateCustomPin(EGPD_Input, "Radius", EGeoFlowReturnType::Float);
	HeightInput = CreateCustomPin(EGPD_Input, "Height", EGeoFlowReturnType::Float);
	InputPins.Add(PositionInput);
	InputPins.Add(RotationInput);
	InputPins.Add(RadiusInput);
	InputPins.Add(HeightInput);
	return InputPins;
}
UGFN_R_Base* UGFN_E_PrimitiveCylinder::CreateRuntimeNode(UGeoFlowRuntimeGraph* runtimeGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap<FGuid, UGeoFlowRuntimePin*>& idToPinMap)
{
	UGFN_R_PrimitiveCylinder* runtimeNode = InitRuntimeNode<UGFN_R_PrimitiveCylinder>(runtimeGraph);
	//PositionInput
	UGeoFlowRuntimePin* positionRuntimePin = InitRuntimePin(runtimeNode, PositionInput, connections, idToPinMap, EGeoFlowReturnType::Vector);
	runtimeNode->position = GetVectorDefaultValue(PositionInput);
	runtimeNode->PositionInput = positionRuntimePin;
	//RotationInput
	UGeoFlowRuntimePin* rotationRuntimePin = InitRuntimePin(runtimeNode, RotationInput, connections, idToPinMap, EGeoFlowReturnType::Vector);
	runtimeNode->rotation = GetVectorDefaultValue(RotationInput);
	runtimeNode->RotationInput = rotationRuntimePin;
	//RadiusInput
	UGeoFlowRuntimePin* RadiusRuntimePin = InitRuntimePin(runtimeNode, RadiusInput, connections, idToPinMap, EGeoFlowReturnType::Float);
	runtimeNode->radius = GetFloatDefaultValue(RadiusInput);
	runtimeNode->RadiusInput = RadiusRuntimePin;
	//HeightInput
	UGeoFlowRuntimePin* HeightRuntimePin = InitRuntimePin(runtimeNode, HeightInput, connections, idToPinMap, EGeoFlowReturnType::Float);
	runtimeNode->height = GetFloatDefaultValue(HeightInput);
	runtimeNode->HeightInput = HeightRuntimePin;
	//Output
	UGeoFlowRuntimePin* OutputRuntimePin = InitRuntimePin(runtimeNode, Output, connections, idToPinMap, EGeoFlowReturnType::Float);
	runtimeNode->Output = OutputRuntimePin;

	return runtimeNode;
}

UGFN_E_Base* UGFN_R_PrimitiveCylinder::CreateEditorNode(UEdGraph* _workingGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap<FGuid, UEdGraphPin*>& idToPinMap)
{
	UGFN_E_PrimitiveCylinder* newNode = InitUiNode<UGFN_E_PrimitiveCylinder>(_workingGraph);
	//PositionInput
	UEdGraphPin* positionUiPin = InitUiPin(newNode, PositionInput, connections, idToPinMap);
	newNode->PositionInput = positionUiPin;
	newNode->position = position;
	SetVectorDefaultValue(positionUiPin, position);
	//RotationInput
	UEdGraphPin* rotationUiPin = InitUiPin(newNode, RotationInput, connections, idToPinMap);
	newNode->RotationInput = rotationUiPin;
	newNode->rotation = rotation;
	SetVectorDefaultValue(rotationUiPin, rotation);
	//radiusinput
	UEdGraphPin* RadiusUIPin = InitUiPin(newNode, RadiusInput, connections, idToPinMap);
	newNode->RadiusInput = RadiusUIPin;
	newNode->radius = radius;
	SetFloatDefaultValue(RadiusUIPin, radius);
	//Heightinput
	UEdGraphPin* HeightUIPin = InitUiPin(newNode, HeightInput, connections, idToPinMap);
	newNode->HeightInput = HeightUIPin;
	newNode->height = height;
	SetFloatDefaultValue(HeightUIPin, height);
	//output
	UEdGraphPin* OutputUiPin = InitUiPin(newNode, Output, connections, idToPinMap);
	newNode->Output = OutputUiPin;

	return newNode;
}


float UGFN_R_PrimitiveCylinder::Evaluate(const FVector3f& pos)
{
	//TODO NEED A WAY TO FAIL TO EVALUATE GRAPH
	//99% some sort of exception structure is the way to go
	//just catch it all the way up before the first Evaluate() call


	if (PositionInput->Connection != nullptr) {
		UGFN_R_BaseVector* node = Cast<UGFN_R_BaseVector>(PositionInput->Connection->OwningNode);
		position = node->Evaluate(pos);
	}
	if (RotationInput->Connection != nullptr) {
		UGFN_R_BaseVector* node = Cast<UGFN_R_BaseVector>(RotationInput->Connection->OwningNode);
		rotation = node->Evaluate(pos);
	}
	if (RadiusInput->Connection != nullptr) {
		UGFN_R_BaseFloat* node = Cast<UGFN_R_BaseFloat>(RadiusInput->Connection->OwningNode);
		radius = node->Evaluate(pos);
	}
	if (HeightInput->Connection != nullptr) {
		UGFN_R_BaseFloat* node = Cast<UGFN_R_BaseFloat>(HeightInput->Connection->OwningNode);
		height = node->Evaluate(pos);
	}

	FVector3f transformedPos = pos - position;
	transformedPos = FRotator3f::MakeFromEuler(rotation).RotateVector(transformedPos);
	FVector2f d = CompWiseAbs(FVector2f(FVector2f(transformedPos.X, transformedPos.Z).Length(), transformedPos.Y)) - FVector2f(radius, height);
	return FMath::Min(FMath::Max(d.X, d.Y), 0.0) + CompWiseMax(d, FVector2f(0.0f)).Length();
}
