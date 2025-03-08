#include "Nodes/GeoFlowArrayPrimitiveNodes.h"
#include "GeoFlowRuntimeGraph.h"
#include "GeoFlowVectorMath.h"

//https://iquilezles.org/articles/distfunctions/
//for all evals

TArray<FVector3f> UGFN_R_BaseArrayPrimitive::GetPositions()
{
	if (TransformsInput->Connection != nullptr) {
		UGFN_R_BaseArray* node = Cast<UGFN_R_BaseArray>(TransformsInput->Connection->OwningNode);
		//this isnt technically very correct BUT i'm fairly certain that changing the positon of a ArrayPrimitive
		//based on where its sampled from can't produce anything except garbage
		transforms = node->Evaluate(FVector3f(0.0f));
	}
	TArray<FVector3f> output;
	for (FGeoFlowTransform t : transforms) {
		output.Add(t.position);
	}
	return output;
}

TArray<UEdGraphPin*> UGFN_E_ArrayPrimitiveSphere::CreateInputPins(UEdGraphPin* fromPin)
{
	TArray<UEdGraphPin*> InputPins;
	TransformsInput = CreateCustomPin(EGPD_Input, "Transforms", EGeoFlowReturnType::Array);
	RadiusInput = CreateCustomPin(EGPD_Input, "Radius", EGeoFlowReturnType::Float);
	InputPins.Add(TransformsInput);
	InputPins.Add(RadiusInput);
	return InputPins;
}
UGFN_R_Base* UGFN_E_ArrayPrimitiveSphere::CreateRuntimeNode(UGeoFlowRuntimeGraph* runtimeGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap<FGuid, UGeoFlowRuntimePin*>& idToPinMap)
{
	UGFN_R_ArrayPrimitiveSphere* runtimeNode = InitRuntimeNode<UGFN_R_ArrayPrimitiveSphere>(runtimeGraph);
	//PositionInput
	UGeoFlowRuntimePin* positionRuntimePin = InitRuntimePin(runtimeNode, TransformsInput, connections, idToPinMap, EGeoFlowReturnType::Array);
	runtimeNode->TransformsInput = positionRuntimePin;
	runtimeNode->transforms = transforms;
	//RadiusInput
	UGeoFlowRuntimePin* RadiusRuntimePin = InitRuntimePin(runtimeNode, RadiusInput, connections, idToPinMap, EGeoFlowReturnType::Float);
	runtimeNode->radius = GetFloatDefaultValue(RadiusInput);
	runtimeNode->RadiusInput = RadiusRuntimePin;
	//Output
	UGeoFlowRuntimePin* OutputRuntimePin = InitRuntimePin(runtimeNode, Output, connections, idToPinMap, EGeoFlowReturnType::Float);
	runtimeNode->Output = OutputRuntimePin;

	return runtimeNode;
}

UGFN_E_Base* UGFN_R_ArrayPrimitiveSphere::CreateEditorNode(UEdGraph* _workingGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap<FGuid, UEdGraphPin*>& idToPinMap)
{
	UGFN_E_ArrayPrimitiveSphere* newNode = InitUiNode<UGFN_E_ArrayPrimitiveSphere>(_workingGraph);
	//PositionInput
	UEdGraphPin* positionUiPin = InitUiPin(newNode, TransformsInput, connections, idToPinMap);
	newNode->TransformsInput = positionUiPin;
	newNode->transforms = transforms;

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

float UGFN_R_ArrayPrimitiveSphere::Evaluate(const FVector3f& pos)
{
	if (TransformsInput->Connection != nullptr) {
		UGFN_R_BaseArray* node = Cast<UGFN_R_BaseArray>(TransformsInput->Connection->OwningNode);
		transforms = node->Evaluate(pos);
	}

	if (RadiusInput->Connection != nullptr) {
		UGFN_R_BaseFloat* node = Cast<UGFN_R_BaseFloat>(RadiusInput->Connection->OwningNode);
		radius = node->Evaluate(pos);
	}
	TArray<float> outputs;

	for (FGeoFlowTransform t : transforms) {
		FVector3f position = t.position;
		FVector3f rotation = t.rotation;

		FVector3f transformedPos = pos - position;
		transformedPos = FRotator3f::MakeFromEuler(rotation).RotateVector(transformedPos);
		outputs.Add(transformedPos.Length() - radius);
	}
	float output = FLT_MAX;
	for (float num : outputs) {
		if (num < output) {
			output = num;
		}
	}
	return output;
}


TArray<UEdGraphPin*> UGFN_E_ArrayPrimitiveCube::CreateInputPins(UEdGraphPin* fromPin)
{
	TArray<UEdGraphPin*> InputPins;
	TransformsInput = CreateCustomPin(EGPD_Input, "Transforms", EGeoFlowReturnType::Array);
	RadiusInput = CreateCustomPin(EGPD_Input, "Size", EGeoFlowReturnType::Float);
	InputPins.Add(TransformsInput);
	InputPins.Add(RadiusInput);
	return InputPins;
}
UGFN_R_Base* UGFN_E_ArrayPrimitiveCube::CreateRuntimeNode(UGeoFlowRuntimeGraph* runtimeGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap<FGuid, UGeoFlowRuntimePin*>& idToPinMap)
{
	UGFN_R_ArrayPrimitiveCube* runtimeNode = InitRuntimeNode<UGFN_R_ArrayPrimitiveCube>(runtimeGraph);
	//PositionInput
	UGeoFlowRuntimePin* positionRuntimePin = InitRuntimePin(runtimeNode, TransformsInput, connections, idToPinMap, EGeoFlowReturnType::Array);
	runtimeNode->TransformsInput = positionRuntimePin;
	runtimeNode->transforms = transforms;
	//RadiusInput
	UGeoFlowRuntimePin* RadiusRuntimePin = InitRuntimePin(runtimeNode, RadiusInput, connections, idToPinMap, EGeoFlowReturnType::Float);
	runtimeNode->radius = GetFloatDefaultValue(RadiusInput);
	runtimeNode->RadiusInput = RadiusRuntimePin;
	//Output
	UGeoFlowRuntimePin* OutputRuntimePin = InitRuntimePin(runtimeNode, Output, connections, idToPinMap, EGeoFlowReturnType::Float);
	runtimeNode->Output = OutputRuntimePin;

	return runtimeNode;
}

UGFN_E_Base* UGFN_R_ArrayPrimitiveCube::CreateEditorNode(UEdGraph* _workingGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap<FGuid, UEdGraphPin*>& idToPinMap)
{
	UGFN_E_ArrayPrimitiveCube* newNode = InitUiNode<UGFN_E_ArrayPrimitiveCube>(_workingGraph);
	//PositionInput
	UEdGraphPin* positionUiPin = InitUiPin(newNode, TransformsInput, connections, idToPinMap);
	newNode->TransformsInput = positionUiPin;
	newNode->transforms = transforms;
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




float UGFN_R_ArrayPrimitiveCube::Evaluate(const FVector3f& pos)
{
	
	if (RadiusInput->Connection != nullptr) {
		UGFN_R_BaseFloat* node = Cast<UGFN_R_BaseFloat>(RadiusInput->Connection->OwningNode);
		radius = node->Evaluate(pos);
	}

	if (TransformsInput->Connection != nullptr) {
		UGFN_R_BaseArray* node = Cast<UGFN_R_BaseArray>(TransformsInput->Connection->OwningNode);
		transforms = node->Evaluate(pos);
	}

	TArray<float> outputs;

	for (FGeoFlowTransform t : transforms) {
		FVector3f position = t.position;
		FVector3f rotation = t.rotation;

		FVector3f transformedPos = pos - position;
		transformedPos = FRotator3f::MakeFromEuler(rotation).RotateVector(transformedPos);
		FVector3f q = CompWiseAbs(transformedPos) - FVector3f(radius);


		outputs.Add(CompWiseMax(q, FVector3f(0.0)).Length() + FMath::Min(FMath::Max(q.X, FMath::Max(q.Y, q.Z)), 0.0));
	}
	float output = FLT_MAX;
	for (float num : outputs) {
		if (num < output) {
			output = num;
		}
	}
	return output;
}


TArray<UEdGraphPin*> UGFN_E_ArrayPrimitiveBox::CreateInputPins(UEdGraphPin* fromPin)
{
	TArray<UEdGraphPin*> InputPins;
	TransformsInput = CreateCustomPin(EGPD_Input, "Transforms", EGeoFlowReturnType::Array);
	RadiusInput = CreateCustomPin(EGPD_Input, "Size XYZ", EGeoFlowReturnType::Vector);
	InputPins.Add(TransformsInput);
	InputPins.Add(RadiusInput);
	return InputPins;
}
UGFN_R_Base* UGFN_E_ArrayPrimitiveBox::CreateRuntimeNode(UGeoFlowRuntimeGraph* runtimeGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap<FGuid, UGeoFlowRuntimePin*>& idToPinMap)
{
	UGFN_R_ArrayPrimitiveBox* runtimeNode = InitRuntimeNode<UGFN_R_ArrayPrimitiveBox>(runtimeGraph);
	//PositionInput
	UGeoFlowRuntimePin* positionRuntimePin = InitRuntimePin(runtimeNode, TransformsInput, connections, idToPinMap, EGeoFlowReturnType::Array);
	runtimeNode->TransformsInput = positionRuntimePin;
	runtimeNode->transforms = transforms;

	//RadiusInput
	UGeoFlowRuntimePin* RadiusRuntimePin = InitRuntimePin(runtimeNode, RadiusInput, connections, idToPinMap, EGeoFlowReturnType::Vector);
	runtimeNode->radius = GetVectorDefaultValue(RadiusInput);
	runtimeNode->RadiusInput = RadiusRuntimePin;
	//Output
	UGeoFlowRuntimePin* OutputRuntimePin = InitRuntimePin(runtimeNode, Output, connections, idToPinMap, EGeoFlowReturnType::Float);
	runtimeNode->Output = OutputRuntimePin;

	return runtimeNode;
}

UGFN_E_Base* UGFN_R_ArrayPrimitiveBox::CreateEditorNode(UEdGraph* _workingGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap<FGuid, UEdGraphPin*>& idToPinMap)
{
	UGFN_E_ArrayPrimitiveBox* newNode = InitUiNode<UGFN_E_ArrayPrimitiveBox>(_workingGraph);
	//PositionInput
	UEdGraphPin* positionUiPin = InitUiPin(newNode, TransformsInput, connections, idToPinMap);
	newNode->TransformsInput = positionUiPin;
	newNode->transforms = transforms;

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



float UGFN_R_ArrayPrimitiveBox::Evaluate(const FVector3f& pos)
{

	if (RadiusInput->Connection != nullptr) {
		UGFN_R_BaseVector* node = Cast<UGFN_R_BaseVector>(RadiusInput->Connection->OwningNode);
		radius = node->Evaluate(pos);
	}


	if (TransformsInput->Connection != nullptr) {
		UGFN_R_BaseArray* node = Cast<UGFN_R_BaseArray>(TransformsInput->Connection->OwningNode);
		transforms = node->Evaluate(pos);
	}
	TArray<float> outputs;

	for (FGeoFlowTransform t : transforms) {
		FVector3f position =t.position;
		FVector3f rotation =t.rotation;


	FVector3f transformedPos = pos - position;
	transformedPos = FRotator3f::MakeFromEuler(rotation).RotateVector(transformedPos);
	FVector3f q = CompWiseAbs(transformedPos) - radius;


	outputs.Add(CompWiseMax(q, FVector3f(0.0)).Length() + FMath::Min(FMath::Max(q.X, FMath::Max(q.Y, q.Z)), 0.0));
	}
	float output = FLT_MAX;
	for (float num : outputs) {
		if (num < output) {
			output = num;
		}
	}
	return output;
}


TArray<UEdGraphPin*> UGFN_E_ArrayPrimitiveEllipsoid::CreateInputPins(UEdGraphPin* fromPin)
{
	TArray<UEdGraphPin*> InputPins;
	TransformsInput = CreateCustomPin(EGPD_Input, "Transforms", EGeoFlowReturnType::Array);
	RadiusInput = CreateCustomPin(EGPD_Input, "Radius XYZ", EGeoFlowReturnType::Vector);
	InputPins.Add(TransformsInput);
	InputPins.Add(RadiusInput);
	return InputPins;
}
UGFN_R_Base* UGFN_E_ArrayPrimitiveEllipsoid::CreateRuntimeNode(UGeoFlowRuntimeGraph* runtimeGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap<FGuid, UGeoFlowRuntimePin*>& idToPinMap)
{
	UGFN_R_ArrayPrimitiveEllipsoid* runtimeNode = InitRuntimeNode<UGFN_R_ArrayPrimitiveEllipsoid>(runtimeGraph);
	//PositionInput
	UGeoFlowRuntimePin* positionRuntimePin = InitRuntimePin(runtimeNode, TransformsInput, connections, idToPinMap, EGeoFlowReturnType::Array);
	runtimeNode->TransformsInput = positionRuntimePin;
	runtimeNode->transforms = transforms;
	//RadiusInput
	UGeoFlowRuntimePin* RadiusRuntimePin = InitRuntimePin(runtimeNode, RadiusInput, connections, idToPinMap, EGeoFlowReturnType::Vector);
	runtimeNode->radius = GetVectorDefaultValue(RadiusInput);
	runtimeNode->RadiusInput = RadiusRuntimePin;
	//Output
	UGeoFlowRuntimePin* OutputRuntimePin = InitRuntimePin(runtimeNode, Output, connections, idToPinMap, EGeoFlowReturnType::Float);
	runtimeNode->Output = OutputRuntimePin;

	return runtimeNode;
}

UGFN_E_Base* UGFN_R_ArrayPrimitiveEllipsoid::CreateEditorNode(UEdGraph* _workingGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap<FGuid, UEdGraphPin*>& idToPinMap)
{
	UGFN_E_ArrayPrimitiveEllipsoid* newNode = InitUiNode<UGFN_E_ArrayPrimitiveEllipsoid>(_workingGraph);
	//PositionInput
	UEdGraphPin* positionUiPin = InitUiPin(newNode, TransformsInput, connections, idToPinMap);
	newNode->TransformsInput = positionUiPin;
	newNode->transforms = transforms;
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


float UGFN_R_ArrayPrimitiveEllipsoid::Evaluate(const FVector3f& pos)
{
	if (RadiusInput->Connection != nullptr) {
		UGFN_R_BaseVector* node = Cast<UGFN_R_BaseVector>(RadiusInput->Connection->OwningNode);
		radius = node->Evaluate(pos);
	}
	if (TransformsInput->Connection != nullptr) {
		UGFN_R_BaseArray* node = Cast<UGFN_R_BaseArray>(TransformsInput->Connection->OwningNode);
		transforms = node->Evaluate(pos);
	}
	TArray<float> outputs;
	for (FGeoFlowTransform t : transforms) {
		FVector3f position = t.position;
		FVector3f rotation = t.rotation;

		FVector3f transformedPos = pos - position;

		transformedPos = FRotator3f::MakeFromEuler(rotation).RotateVector(transformedPos);
		FVector3f k0Vec = transformedPos / radius;
		FVector3f k1Vec = transformedPos / (radius * radius);
		float k0 = k0Vec.Length();
		float k1 = k1Vec.Length();

		outputs.Add( k0 * (k0 - 1.0) / k1);
	}
	float output = FLT_MAX;
	for (float num : outputs) {
		if (num < output) {
			output = num;
		}
	}
	return output;
}


TArray<UEdGraphPin*> UGFN_E_ArrayPrimitiveCone::CreateInputPins(UEdGraphPin* fromPin)
{
	TArray<UEdGraphPin*> InputPins;
	TransformsInput = CreateCustomPin(EGPD_Input, "Transforms", EGeoFlowReturnType::Array);

	AngleInput = CreateCustomPin(EGPD_Input, "Angle", EGeoFlowReturnType::Float);
	HeightInput = CreateCustomPin(EGPD_Input, "Height", EGeoFlowReturnType::Float);
	InputPins.Add(TransformsInput);

	InputPins.Add(AngleInput);
	InputPins.Add(HeightInput);
	return InputPins;
}
UGFN_R_Base* UGFN_E_ArrayPrimitiveCone::CreateRuntimeNode(UGeoFlowRuntimeGraph* runtimeGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap<FGuid, UGeoFlowRuntimePin*>& idToPinMap)
{
	UGFN_R_ArrayPrimitiveCone* runtimeNode = InitRuntimeNode<UGFN_R_ArrayPrimitiveCone>(runtimeGraph);
	//PositionInput
	UGeoFlowRuntimePin* positionRuntimePin = InitRuntimePin(runtimeNode, TransformsInput, connections, idToPinMap, EGeoFlowReturnType::Array);
	runtimeNode->TransformsInput = positionRuntimePin;
	runtimeNode->transforms = transforms;

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

UGFN_E_Base* UGFN_R_ArrayPrimitiveCone::CreateEditorNode(UEdGraph* _workingGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap<FGuid, UEdGraphPin*>& idToPinMap)
{
	UGFN_E_ArrayPrimitiveCone* newNode = InitUiNode<UGFN_E_ArrayPrimitiveCone>(_workingGraph);
	//PositionInput
	UEdGraphPin* positionUiPin = InitUiPin(newNode, TransformsInput, connections, idToPinMap);
	newNode->TransformsInput = positionUiPin;
	newNode->transforms = transforms;

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

float UGFN_R_ArrayPrimitiveCone::Evaluate(const FVector3f& pos)
{

	if (AngleInput->Connection != nullptr) {
		UGFN_R_BaseFloat* node = Cast<UGFN_R_BaseFloat>(AngleInput->Connection->OwningNode);
		angle = node->Evaluate(pos);
	}
	if (HeightInput->Connection != nullptr) {
		UGFN_R_BaseFloat* node = Cast<UGFN_R_BaseFloat>(HeightInput->Connection->OwningNode);
		height = node->Evaluate(pos);
	}
	if (TransformsInput->Connection != nullptr) {
		UGFN_R_BaseArray* node = Cast<UGFN_R_BaseArray>(TransformsInput->Connection->OwningNode);
		transforms = node->Evaluate(pos);
	}

	TArray<float> outputs;

	for (FGeoFlowTransform t : transforms) {
		FVector3f position = t.position;
		FVector3f rotation = t.rotation;

		FVector3f transformedPos = pos - position;
		transformedPos = FRotator3f::MakeFromEuler(rotation).RotateVector(transformedPos);
		FVector2f c(FMath::Sin(FMath::DegreesToRadians(angle)), FMath::Cos(FMath::DegreesToRadians(angle)));

		//* exact version

		FVector2f q = height * FVector2f((c.X / c.Y), -1.0f);
		FVector2f w = FVector2f(FVector2f(transformedPos.X, transformedPos.Z).Length(), transformedPos.Y);
		FVector2f a = w - q * FMath::Clamp((FVector2f::DotProduct(w, q) / FVector2f::DotProduct(q, q)), 0.0f, 1.0f);
		FVector2f b = w - q * FVector2f((FMath::Clamp(w.X / q.X, 0.0f, 1.0f)), 1.0f);
		float k = FMath::Sign(q.Y);
		float d = FMath::Min(FVector2f::DotProduct(a, a), FVector2f::DotProduct(b, b));
		float s = FMath::Max(k * (w.X * q.Y - w.Y * q.X), k * (w.Y - q.Y));
		outputs.Add( FMath::Sqrt(d) * FMath::Sign(s));
	}
	float output = FLT_MAX;
	for (float num : outputs) {
		if (num < output) {
			output = num;
		}
	}
	return output;

}



TArray<UEdGraphPin*> UGFN_E_ArrayPrimitiveCylinder::CreateInputPins(UEdGraphPin* fromPin)
{
	TArray<UEdGraphPin*> InputPins;
	TransformsInput = CreateCustomPin(EGPD_Input, "Transforms", EGeoFlowReturnType::Array);
	RadiusInput = CreateCustomPin(EGPD_Input, "Radius", EGeoFlowReturnType::Float);
	HeightInput = CreateCustomPin(EGPD_Input, "Height", EGeoFlowReturnType::Float);
	InputPins.Add(TransformsInput);
	InputPins.Add(RadiusInput);
	InputPins.Add(HeightInput);
	return InputPins;
}
UGFN_R_Base* UGFN_E_ArrayPrimitiveCylinder::CreateRuntimeNode(UGeoFlowRuntimeGraph* runtimeGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap<FGuid, UGeoFlowRuntimePin*>& idToPinMap)
{
	UGFN_R_ArrayPrimitiveCylinder* runtimeNode = InitRuntimeNode<UGFN_R_ArrayPrimitiveCylinder>(runtimeGraph);
	//PositionInput
	UGeoFlowRuntimePin* positionRuntimePin = InitRuntimePin(runtimeNode, TransformsInput, connections, idToPinMap, EGeoFlowReturnType::Array);
	runtimeNode->TransformsInput = positionRuntimePin;
	runtimeNode->transforms = transforms;

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

UGFN_E_Base* UGFN_R_ArrayPrimitiveCylinder::CreateEditorNode(UEdGraph* _workingGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap<FGuid, UEdGraphPin*>& idToPinMap)
{
	UGFN_E_ArrayPrimitiveCylinder* newNode = InitUiNode<UGFN_E_ArrayPrimitiveCylinder>(_workingGraph);
	//PositionInput
	UEdGraphPin* positionUiPin = InitUiPin(newNode, TransformsInput, connections, idToPinMap);
	newNode->TransformsInput = positionUiPin;
	newNode->transforms = transforms;

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


float UGFN_R_ArrayPrimitiveCylinder::Evaluate(const FVector3f& pos)
{

	if (RadiusInput->Connection != nullptr) {
		UGFN_R_BaseFloat* node = Cast<UGFN_R_BaseFloat>(RadiusInput->Connection->OwningNode);
		radius = node->Evaluate(pos);
	}
	if (HeightInput->Connection != nullptr) {
		UGFN_R_BaseFloat* node = Cast<UGFN_R_BaseFloat>(HeightInput->Connection->OwningNode);
		height = node->Evaluate(pos);
	}
	if (TransformsInput->Connection != nullptr) {
		UGFN_R_BaseArray* node = Cast<UGFN_R_BaseArray>(TransformsInput->Connection->OwningNode);
		transforms = node->Evaluate(pos);
	}

	TArray<float> outputs;

	for (FGeoFlowTransform t : transforms) {
		FVector3f position = t.position;
		FVector3f rotation = t.rotation;

		FVector3f transformedPos = pos - position;
		transformedPos = FRotator3f::MakeFromEuler(rotation).RotateVector(transformedPos);
		FVector2f d = CompWiseAbs(FVector2f(FVector2f(transformedPos.X, transformedPos.Z).Length(), transformedPos.Y)) - FVector2f(radius, height);
		outputs.Add(FMath::Min(FMath::Max(d.X, d.Y), 0.0) + CompWiseMax(d, FVector2f(0.0f)).Length());
	}
	float output = FLT_MAX;
	for (float num : outputs) {
		if (num < output) {
			output = num;
		}
	}
	return output;

}
