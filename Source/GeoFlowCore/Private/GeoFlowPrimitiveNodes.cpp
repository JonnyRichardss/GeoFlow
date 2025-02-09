#include "GeoFlowPrimitiveNodes.h"
#include "GeoFlowRuntimeGraph.h"
#include "GeoFlowPinDefaultValueOps.h"

//https://iquilezles.org/articles/distfunctions/
//for all evals
static FVector3d CompWiseAbs(FVector3d in) {
	return FVector3d(FMath::Abs(in.X), FMath::Abs(in.Y), FMath::Abs(in.Z));
}
static FVector3d CompWiseMax(FVector3d a, FVector3d b) {
	return FVector3d(FMath::Max(a.X, b.X), FMath::Max(a.Y, b.Y), FMath::Max(a.Z, b.Z));
}
static FVector3d CompWiseMin(FVector3d a, FVector3d b) {
	return FVector3d(FMath::Min(a.X, b.X), FMath::Min(a.Y, b.Y), FMath::Min(a.Z, b.Z));
}

TArray<UEdGraphPin*> UGFN_E_PrimitiveSphere::CreateInputPins(UEdGraphPin* fromPin)
{
	TArray<UEdGraphPin*> InputPins;
	CentreInput = CreateCustomPin(EGPD_Input, "Position", EGeoFlowReturnType::Vector);
	RadiusInput = CreateCustomPin(EGPD_Input, "Radius", EGeoFlowReturnType::Double);
	InputPins.Add(CentreInput);
	InputPins.Add(RadiusInput);
	return InputPins;
}
UGFN_R_Base* UGFN_E_PrimitiveSphere::CreateRuntimeNode(UGeoFlowRuntimeGraph* runtimeGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap<FGuid, UGeoFlowRuntimePin*>& idToPinMap)
{
	UGFN_R_PrimitiveSphere* runtimeNode = InitRuntimeNode<UGFN_R_PrimitiveSphere>(runtimeGraph);
	//CentreInput
	UGeoFlowRuntimePin* CentreRuntimePin = InitRuntimePin(runtimeNode, CentreInput, connections, idToPinMap, EGeoFlowReturnType::Vector);
	runtimeNode->centre = GetVectorDefaultValue(CentreInput);
	runtimeNode->CentreInput = CentreRuntimePin;
	//RadiusInput
	UGeoFlowRuntimePin* RadiusRuntimePin = InitRuntimePin(runtimeNode, RadiusInput, connections, idToPinMap, EGeoFlowReturnType::Double); 
	runtimeNode->radius = GetDoubleDefaultValue(RadiusInput);
	runtimeNode->RadiusInput = RadiusRuntimePin;
	//Output
	UGeoFlowRuntimePin* OutputRuntimePin = InitRuntimePin(runtimeNode, Output, connections, idToPinMap, EGeoFlowReturnType::Double);
	runtimeNode->Output = OutputRuntimePin;

	return runtimeNode;
}

UGFN_E_Base* UGFN_R_PrimitiveSphere::CreateEditorNode(UEdGraph* _workingGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap<FGuid, UEdGraphPin*>& idToPinMap)
{
	UGFN_E_PrimitiveSphere* newNode = InitUiNode<UGFN_E_PrimitiveSphere>(_workingGraph);
	//centreinput
	UEdGraphPin* CentreUiPin = InitUiPin(newNode, CentreInput, connections, idToPinMap);
	newNode->CentreInput = CentreUiPin;
	SetVectorDefaultValue(CentreUiPin, centre);
	//radiusinput
	UEdGraphPin* RadiusUiPin = InitUiPin(newNode, RadiusInput, connections, idToPinMap);
	newNode->RadiusInput = RadiusUiPin;
	SetDoubleDefaultValue(RadiusUiPin, radius);
	//output
	UEdGraphPin* OutputUiPin = InitUiPin(newNode, Output, connections, idToPinMap);
	newNode->Output = OutputUiPin;

	return newNode;
}

double UGFN_R_PrimitiveSphere::Evaluate(const FVector3d& pos)
{
	//TODO NEED A WAY TO FAIL TO EVALUATE GRAPH
	//99% some sort of exception structure is the way to go
	//just catch it all the way up before the first Evaluate() call


	if (CentreInput->Connection != nullptr) {
		UGFN_R_BaseVector* node = Cast<UGFN_R_BaseVector>(CentreInput->Connection->OwningNode);
		centre = node->Evaluate(pos);
	}
	if (RadiusInput->Connection != nullptr) {
		UGFN_R_BaseDouble* node = Cast<UGFN_R_BaseDouble>(RadiusInput->Connection->OwningNode);
		radius = node->Evaluate(pos);
	}

	FVector3d transformedPos = pos - centre;

	return transformedPos.Length() - radius;

	//return (transformedPos.X * transformedPos.X) + (transformedPos.Y * transformedPos.Y) + (transformedPos.Z * transformedPos.Z) - (radius * radius);

}



TArray<UEdGraphPin*> UGFN_E_PrimitiveBox::CreateInputPins(UEdGraphPin* fromPin)
{
	TArray<UEdGraphPin*> InputPins;
	CentreInput = CreateCustomPin(EGPD_Input, "Position", EGeoFlowReturnType::Vector);
	RadiusInput = CreateCustomPin(EGPD_Input, "Size XYZ", EGeoFlowReturnType::Vector);
	InputPins.Add(CentreInput);
	InputPins.Add(RadiusInput);
	return InputPins;
}
UGFN_R_Base* UGFN_E_PrimitiveBox::CreateRuntimeNode(UGeoFlowRuntimeGraph* runtimeGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap<FGuid, UGeoFlowRuntimePin*>& idToPinMap)
{
	UGFN_R_PrimitiveBox* runtimeNode = InitRuntimeNode<UGFN_R_PrimitiveBox>(runtimeGraph);
	//CentreInput
	UGeoFlowRuntimePin* CentreRuntimePin = InitRuntimePin(runtimeNode, CentreInput, connections, idToPinMap, EGeoFlowReturnType::Vector);
	runtimeNode->centre = GetVectorDefaultValue(CentreInput);
	runtimeNode->CentreInput = CentreRuntimePin;
	//RadiusInput
	UGeoFlowRuntimePin* RadiusRuntimePin = InitRuntimePin(runtimeNode, RadiusInput, connections, idToPinMap, EGeoFlowReturnType::Vector);
	runtimeNode->radius = GetVectorDefaultValue(RadiusInput);
	runtimeNode->RadiusInput = RadiusRuntimePin;
	//Output
	UGeoFlowRuntimePin* OutputRuntimePin = InitRuntimePin(runtimeNode, Output, connections, idToPinMap, EGeoFlowReturnType::Double);
	runtimeNode->Output = OutputRuntimePin;

	return runtimeNode;
}

UGFN_E_Base* UGFN_R_PrimitiveBox::CreateEditorNode(UEdGraph* _workingGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap<FGuid, UEdGraphPin*>& idToPinMap)
{
	UGFN_E_PrimitiveBox* newNode = InitUiNode<UGFN_E_PrimitiveBox>(_workingGraph);
	//centreinput
	UEdGraphPin* CentreUiPin = InitUiPin(newNode, CentreInput, connections, idToPinMap);
	newNode->CentreInput = CentreUiPin;
	SetVectorDefaultValue(CentreUiPin, centre);
	//radiusinput
	UEdGraphPin* RadiusUiPin = InitUiPin(newNode, RadiusInput, connections, idToPinMap);
	newNode->RadiusInput = RadiusUiPin;
	SetVectorDefaultValue(RadiusUiPin, radius);
	//output
	UEdGraphPin* OutputUiPin = InitUiPin(newNode, Output, connections, idToPinMap);
	newNode->Output = OutputUiPin;
	
	return newNode;
}


double UGFN_R_PrimitiveBox::Evaluate(const FVector3d& pos)
{
	if (CentreInput->Connection != nullptr) {
		UGFN_R_BaseVector* node = Cast<UGFN_R_BaseVector>(CentreInput->Connection->OwningNode);
		centre = node->Evaluate(pos);
	}
	if (RadiusInput->Connection != nullptr) {
		UGFN_R_BaseVector* node = Cast<UGFN_R_BaseVector>(RadiusInput->Connection->OwningNode);
		radius = node->Evaluate(pos);
	}

	FVector3d transformedPos = pos - centre;
	FVector3d q = CompWiseAbs(transformedPos) - radius;
	

	return CompWiseMax(q, FVector3d(0.0)).Length() + FMath::Min(FMath::Max(q.X, FMath::Max(q.Y, q.Z)), 0.0);
}



TArray<UEdGraphPin*> UGFN_E_PrimitivePlane::CreateInputPins(UEdGraphPin* fromPin)
{
	TArray<UEdGraphPin*> InputPins;
	NormalInput = CreateCustomPin(EGPD_Input, "Normal Axis", EGeoFlowReturnType::Vector);
	OffsetInput = CreateCustomPin(EGPD_Input, "Offset", EGeoFlowReturnType::Double);
	InputPins.Add(NormalInput);
	InputPins.Add(OffsetInput);
	return InputPins;
}
UGFN_R_Base* UGFN_E_PrimitivePlane::CreateRuntimeNode(UGeoFlowRuntimeGraph* runtimeGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap<FGuid, UGeoFlowRuntimePin*>& idToPinMap)
{
	UGFN_R_PrimitivePlane* runtimeNode = InitRuntimeNode<UGFN_R_PrimitivePlane>(runtimeGraph);
	//NormalInput
	UGeoFlowRuntimePin* CentreRuntimePin = InitRuntimePin(runtimeNode, NormalInput, connections, idToPinMap, EGeoFlowReturnType::Vector);
	runtimeNode->normal = GetVectorDefaultValue(NormalInput);
	runtimeNode->NormalInput = CentreRuntimePin;
	//RadiusInput
	UGeoFlowRuntimePin* RadiusRuntimePin = InitRuntimePin(runtimeNode, OffsetInput, connections, idToPinMap, EGeoFlowReturnType::Double);
	runtimeNode->offset = GetDoubleDefaultValue(OffsetInput);
	runtimeNode->OffsetInput = RadiusRuntimePin;
	//Output
	UGeoFlowRuntimePin* OutputRuntimePin = InitRuntimePin(runtimeNode, Output, connections, idToPinMap, EGeoFlowReturnType::Double);
	runtimeNode->Output = OutputRuntimePin;

	return runtimeNode;
}

UGFN_E_Base* UGFN_R_PrimitivePlane::CreateEditorNode(UEdGraph* _workingGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap<FGuid, UEdGraphPin*>& idToPinMap)
{
	auto newNode = InitUiNode<UGFN_E_PrimitivePlane>(_workingGraph);
	//centreinput
	UEdGraphPin* CentreUiPin = InitUiPin(newNode, NormalInput, connections, idToPinMap);
	newNode->NormalInput = CentreUiPin;
	SetVectorDefaultValue(CentreUiPin, normal);
	//radiusinput
	UEdGraphPin* RadiusUiPin = InitUiPin(newNode, OffsetInput, connections, idToPinMap);
	newNode->OffsetInput = RadiusUiPin;
	SetDoubleDefaultValue(RadiusUiPin, offset);
	//output
	UEdGraphPin* OutputUiPin = InitUiPin(newNode, Output, connections, idToPinMap);
	newNode->Output = OutputUiPin;
	return newNode;
}

double UGFN_R_PrimitivePlane::Evaluate(const FVector3d& pos)
{
	//TODO NEED A WAY TO FAIL TO EVALUATE GRAPH
	//99% some sort of exception structure is the way to go
	//just catch it all the way up before the first Evaluate() call


	if (NormalInput->Connection != nullptr) {
		UGFN_R_BaseVector* node = Cast<UGFN_R_BaseVector>(NormalInput->Connection->OwningNode);
		normal = node->Evaluate(pos);
	}
	if (OffsetInput->Connection != nullptr) {
		UGFN_R_BaseDouble* node = Cast<UGFN_R_BaseDouble>(OffsetInput->Connection->OwningNode);
		offset = node->Evaluate(pos);
	}

	normal.Normalize();
	return FVector3d::DotProduct(pos,normal) + offset;

}


TArray<UEdGraphPin*> UGFN_E_PrimitiveEllipsoid::CreateInputPins(UEdGraphPin* fromPin)
{
	TArray<UEdGraphPin*> InputPins;
	CentreInput = CreateCustomPin(EGPD_Input, "Position", EGeoFlowReturnType::Vector);
	RadiusInput = CreateCustomPin(EGPD_Input, "Radius XYZ", EGeoFlowReturnType::Vector);
	InputPins.Add(CentreInput);
	InputPins.Add(RadiusInput);
	return InputPins;
}
UGFN_R_Base* UGFN_E_PrimitiveEllipsoid::CreateRuntimeNode(UGeoFlowRuntimeGraph* runtimeGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap<FGuid, UGeoFlowRuntimePin*>& idToPinMap)
{
	UGFN_R_PrimitiveEllipsoid* runtimeNode = InitRuntimeNode<UGFN_R_PrimitiveEllipsoid>(runtimeGraph);
	//CentreInput
	UGeoFlowRuntimePin* CentreRuntimePin = InitRuntimePin(runtimeNode, CentreInput, connections, idToPinMap, EGeoFlowReturnType::Vector);
	runtimeNode->centre = GetVectorDefaultValue(CentreInput);
	runtimeNode->CentreInput = CentreRuntimePin;
	//RadiusInput
	UGeoFlowRuntimePin* RadiusRuntimePin = InitRuntimePin(runtimeNode, RadiusInput, connections, idToPinMap, EGeoFlowReturnType::Vector);
	runtimeNode->radius = GetVectorDefaultValue(RadiusInput);
	runtimeNode->RadiusInput = RadiusRuntimePin;
	//Output
	UGeoFlowRuntimePin* OutputRuntimePin = InitRuntimePin(runtimeNode, Output, connections, idToPinMap, EGeoFlowReturnType::Double);
	runtimeNode->Output = OutputRuntimePin;

	return runtimeNode;
}

UGFN_E_Base* UGFN_R_PrimitiveEllipsoid::CreateEditorNode(UEdGraph* _workingGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap<FGuid, UEdGraphPin*>& idToPinMap)
{
	UGFN_E_PrimitiveEllipsoid* newNode = InitUiNode<UGFN_E_PrimitiveEllipsoid>(_workingGraph);
	//centreinput
	UEdGraphPin* CentreUiPin = InitUiPin(newNode, CentreInput, connections, idToPinMap);
	newNode->CentreInput = CentreUiPin;
	SetVectorDefaultValue(CentreUiPin, centre);
	//radiusinput
	UEdGraphPin* RadiusUiPin = InitUiPin(newNode, RadiusInput, connections, idToPinMap);
	
	newNode->RadiusInput = RadiusUiPin;
	SetVectorDefaultValue(RadiusUiPin, radius);
	//output
	UEdGraphPin* OutputUiPin = InitUiPin(newNode, Output, connections, idToPinMap);
	newNode->Output = OutputUiPin;

	return newNode;
}

double UGFN_R_PrimitiveEllipsoid::Evaluate(const FVector3d& pos)
{
	if (CentreInput->Connection != nullptr) {
		UGFN_R_BaseVector* node = Cast<UGFN_R_BaseVector>(CentreInput->Connection->OwningNode);
		centre = node->Evaluate(pos);
	}
	if (RadiusInput->Connection != nullptr) {
		UGFN_R_BaseVector* node = Cast<UGFN_R_BaseVector>(RadiusInput->Connection->OwningNode);
		radius = node->Evaluate(pos);
	}

	FVector3d transformedPos = pos - centre;
	FVector3d k0Vec = transformedPos / radius;
	FVector3d k1Vec = transformedPos / (radius * radius);
	double k0 = k0Vec.Length();
	double k1 = k1Vec.Length();

	return k0 * (k0 - 1.0) / k1;
}
