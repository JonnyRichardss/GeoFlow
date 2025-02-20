#include "GeoFlowPrimitiveNodes.h"
#include "GeoFlowRuntimeGraph.h"
#include "GeoFlowPinDefaultValueOps.h"

//https://iquilezles.org/articles/distfunctions/
//for all evals
static FVector3f CompWiseAbs(FVector3f in) {
	return FVector3f(FMath::Abs(in.X), FMath::Abs(in.Y), FMath::Abs(in.Z));
}
static FVector3f CompWiseMax(FVector3f a, FVector3f b) {
	return FVector3f(FMath::Max(a.X, b.X), FMath::Max(a.Y, b.Y), FMath::Max(a.Z, b.Z));
}
static FVector3f CompWiseMin(FVector3f a, FVector3f b) {
	return FVector3f(FMath::Min(a.X, b.X), FMath::Min(a.Y, b.Y), FMath::Min(a.Z, b.Z));
}

TArray<UEdGraphPin*> UGFN_E_PrimitiveSphere::CreateInputPins(UEdGraphPin* fromPin)
{
	TArray<UEdGraphPin*> InputPins;
	PositionInput = CreateCustomPin(EGPD_Input, "Position", EGeoFlowReturnType::Vector);
	RadiusInput = CreateCustomPin(EGPD_Input, "Radius", EGeoFlowReturnType::Float);
	InputPins.Add(PositionInput);
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
	SetVectorDefaultValue(positionUiPin, position);
	//radiusinput
	UEdGraphPin* RadiusUiPin = InitUiPin(newNode, RadiusInput, connections, idToPinMap);
	newNode->RadiusInput = RadiusUiPin;
	SetFloatDefaultValue(RadiusUiPin, radius);
	//output
	UEdGraphPin* OutputUiPin = InitUiPin(newNode, Output, connections, idToPinMap);
	newNode->Output = OutputUiPin;

	return newNode;
}

FString UGFN_R_PrimitiveSphere::CreateShaderEvalCall(TArray<FString>& PinDeclarations)
{
	FString positionVal;
	FString RadiusVal;
	if (PositionInput->Connection != nullptr) {
		UGFN_R_BaseVector* node = Cast<UGFN_R_BaseVector>(PositionInput->Connection->OwningNode);
		positionVal.Appendf(TEXT("PIN_%s"), *(PositionInput->PinId.ToString(EGuidFormats::DigitsLower)));
		PinDeclarations.Add(FString::Printf(TEXT("float3 %s = %s;\n"), *positionVal, *(node->CreateShaderEvalCall(PinDeclarations))));
	}
	else {
		positionVal.Appendf(TEXT("float3(%f,%f,%f)"), position.X,position.Y,position.Z);
	}
	if (RadiusInput->Connection != nullptr) {
		UGFN_R_BaseFloat* node = Cast<UGFN_R_BaseFloat>(RadiusInput->Connection->OwningNode);
		RadiusVal.Appendf(TEXT("PIN_%s"), *(RadiusInput->PinId.ToString(EGuidFormats::DigitsLower)));
		PinDeclarations.Add(FString::Printf(TEXT("float %s = %s;\n"), *RadiusVal, *(node->CreateShaderEvalCall(PinDeclarations))));
	}
	else {
		RadiusVal.Appendf(TEXT("%f"), radius);
	}

	return FString::Printf(TEXT("PrimitiveSphere(pos,%s,%s)"),*positionVal,*RadiusVal);
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
	if (RadiusInput->Connection != nullptr) {
		UGFN_R_BaseFloat* node = Cast<UGFN_R_BaseFloat>(RadiusInput->Connection->OwningNode);
		radius = node->Evaluate(pos);
	}

	FVector3f transformedPos = pos - position;

	return transformedPos.Length() - radius;

	//return (transformedPos.X * transformedPos.X) + (transformedPos.Y * transformedPos.Y) + (transformedPos.Z * transformedPos.Z) - (radius * radius);

}



TArray<UEdGraphPin*> UGFN_E_PrimitiveBox::CreateInputPins(UEdGraphPin* fromPin)
{
	TArray<UEdGraphPin*> InputPins;
	PositionInput = CreateCustomPin(EGPD_Input, "Position", EGeoFlowReturnType::Vector);
	RadiusInput = CreateCustomPin(EGPD_Input, "Size XYZ", EGeoFlowReturnType::Vector);
	InputPins.Add(PositionInput);
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
	SetVectorDefaultValue(positionUiPin, position);
	//radiusinput
	UEdGraphPin* RadiusUiPin = InitUiPin(newNode, RadiusInput, connections, idToPinMap);
	newNode->RadiusInput = RadiusUiPin;
	SetVectorDefaultValue(RadiusUiPin, radius);
	//output
	UEdGraphPin* OutputUiPin = InitUiPin(newNode, Output, connections, idToPinMap);
	newNode->Output = OutputUiPin;
	
	return newNode;
}




FString UGFN_R_PrimitiveBox::CreateShaderEvalCall(TArray<FString>& PinDeclarations)
{
	FString positionVal;
	FString RadiusVal;
	if (PositionInput->Connection != nullptr) {
		UGFN_R_BaseVector* node = Cast<UGFN_R_BaseVector>(PositionInput->Connection->OwningNode);
		positionVal.Appendf(TEXT("PIN_%s"), *(PositionInput->PinId.ToString(EGuidFormats::DigitsLower)));
		PinDeclarations.Add(FString::Printf(TEXT("float3 %s = %s;\n"), *positionVal, *(node->CreateShaderEvalCall(PinDeclarations))));
	}
	else {
		positionVal.Appendf(TEXT("float3(%f,%f,%f)"), position.X, position.Y, position.Z);
	}
	if (RadiusInput->Connection != nullptr) {
		UGFN_R_BaseVector* node = Cast<UGFN_R_BaseVector>(RadiusInput->Connection->OwningNode);
		RadiusVal.Appendf(TEXT("PIN_%s"), *(RadiusInput->PinId.ToString(EGuidFormats::DigitsLower)));
		PinDeclarations.Add(FString::Printf(TEXT("float3 %s = %s;\n"), *RadiusVal, *(node->CreateShaderEvalCall(PinDeclarations))));
	}
	else {
		RadiusVal.Appendf(TEXT("float3(%f,%f,%f)"), radius.X, radius.Y, radius.Z);
	}

	return FString::Printf(TEXT("PrimitiveBox(pos,%s,%s)"), *positionVal, *RadiusVal);
}

float UGFN_R_PrimitiveBox::Evaluate(const FVector3f& pos)
{
	if (PositionInput->Connection != nullptr) {
		UGFN_R_BaseVector* node = Cast<UGFN_R_BaseVector>(PositionInput->Connection->OwningNode);
		position = node->Evaluate(pos);
	}
	if (RadiusInput->Connection != nullptr) {
		UGFN_R_BaseVector* node = Cast<UGFN_R_BaseVector>(RadiusInput->Connection->OwningNode);
		radius = node->Evaluate(pos);
	}

	FVector3f transformedPos = pos - position;
	FVector3f q = CompWiseAbs(transformedPos) - radius;
	

	return CompWiseMax(q, FVector3f(0.0)).Length() + FMath::Min(FMath::Max(q.X, FMath::Max(q.Y, q.Z)), 0.0);
}


TArray<UEdGraphPin*> UGFN_E_PrimitiveEllipsoid::CreateInputPins(UEdGraphPin* fromPin)
{
	TArray<UEdGraphPin*> InputPins;
	PositionInput = CreateCustomPin(EGPD_Input, "Position", EGeoFlowReturnType::Vector);
	RadiusInput = CreateCustomPin(EGPD_Input, "Radius XYZ", EGeoFlowReturnType::Vector);
	InputPins.Add(PositionInput);
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
	SetVectorDefaultValue(positionUiPin, position);
	//radiusinput
	UEdGraphPin* RadiusUiPin = InitUiPin(newNode, RadiusInput, connections, idToPinMap);
	
	newNode->RadiusInput = RadiusUiPin;
	SetVectorDefaultValue(RadiusUiPin, radius);
	//output
	UEdGraphPin* OutputUiPin = InitUiPin(newNode, Output, connections, idToPinMap);
	newNode->Output = OutputUiPin;

	return newNode;
}



FString UGFN_R_PrimitiveEllipsoid::CreateShaderEvalCall(TArray<FString>& PinDeclarations)
{
	FString positionVal;
	FString RadiusVal;
	if (PositionInput->Connection != nullptr) {
		UGFN_R_BaseVector* node = Cast<UGFN_R_BaseVector>(PositionInput->Connection->OwningNode);
		positionVal.Appendf(TEXT("PIN_%s"), *(PositionInput->PinId.ToString(EGuidFormats::DigitsLower)));
		PinDeclarations.Add(FString::Printf(TEXT("float3 %s = %s;\n"), *positionVal, *(node->CreateShaderEvalCall(PinDeclarations))));
	}
	else {
		positionVal.Appendf(TEXT("float3(%f,%f,%f)"), position.X, position.Y, position.Z);
	}
	if (RadiusInput->Connection != nullptr) {
		UGFN_R_BaseVector* node = Cast<UGFN_R_BaseVector>(RadiusInput->Connection->OwningNode);
		RadiusVal.Appendf(TEXT("PIN_%s"), *(RadiusInput->PinId.ToString(EGuidFormats::DigitsLower)));
		PinDeclarations.Add(FString::Printf(TEXT("float3 %s = %s;\n"), *RadiusVal, *(node->CreateShaderEvalCall(PinDeclarations))));
	}
	else {
		RadiusVal.Appendf(TEXT("float3(%f,%f,%f)"), radius.X,radius.Y,radius.Z);
	}

	return FString::Printf(TEXT("PrimitiveEllipsoid(pos,%s,%s)"), *positionVal, *RadiusVal);
}

float UGFN_R_PrimitiveEllipsoid::Evaluate(const FVector3f& pos)
{
	if (PositionInput->Connection != nullptr) {
		UGFN_R_BaseVector* node = Cast<UGFN_R_BaseVector>(PositionInput->Connection->OwningNode);
		position = node->Evaluate(pos);
	}
	if (RadiusInput->Connection != nullptr) {
		UGFN_R_BaseVector* node = Cast<UGFN_R_BaseVector>(RadiusInput->Connection->OwningNode);
		radius = node->Evaluate(pos);
	}

	FVector3f transformedPos = pos - position;
	FVector3f k0Vec = transformedPos / radius;
	FVector3f k1Vec = transformedPos / (radius * radius);
	float k0 = k0Vec.Length();
	float k1 = k1Vec.Length();

	return k0 * (k0 - 1.0) / k1;
}

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
