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
	UGFN_R_PrimitiveSphere* runtimeNode = NewObject<UGFN_R_PrimitiveSphere>(runtimeGraph);
	runtimeNode->Position = FVector2D(NodePosX, NodePosY);

	//CentreInput
	UGeoFlowRuntimePin* CentreRuntimePin = NewObject<UGeoFlowRuntimePin>(runtimeNode);
	CentreRuntimePin->PinName = CentreInput->PinName;
	CentreRuntimePin->PinId = CentreInput->PinId;
	if (CentreInput->HasAnyConnections()) {
		std::pair<FGuid, FGuid> connection = std::make_pair(CentreInput->PinId, CentreInput->LinkedTo[0]->PinId);
		connections.Add(connection);
	}
	else {
		//only updating default value when we have no connection *should* mean that it persists underneath
		//not entirely sure if this is true
		runtimeNode->centre = GetVectorDefaultValue(CentreInput);
	}
	idToPinMap.Add(CentreInput->PinId, CentreRuntimePin);
	runtimeNode->CentreInput = CentreRuntimePin;
	CentreRuntimePin->OwningNode = runtimeNode;

	//RadiusInput
	UGeoFlowRuntimePin* RadiusRuntimePin = NewObject<UGeoFlowRuntimePin>(runtimeNode);
	RadiusRuntimePin->PinName = RadiusInput->PinName;
	RadiusRuntimePin->PinId = RadiusInput->PinId;
	if (RadiusInput->HasAnyConnections()) {
		std::pair<FGuid, FGuid> connection = std::make_pair(RadiusInput->PinId, RadiusInput->LinkedTo[0]->PinId);
		connections.Add(connection);
	}
	else {
		runtimeNode->radius = GetDoubleDefaultValue(RadiusInput);
	}
	idToPinMap.Add(RadiusInput->PinId, RadiusRuntimePin);
	runtimeNode->RadiusInput = RadiusRuntimePin;
	RadiusRuntimePin->OwningNode = runtimeNode;

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

UGFN_E_Base* UGFN_R_PrimitiveSphere::CreateEditorNode(UEdGraph* _workingGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap<FGuid, UEdGraphPin*>& idToPinMap)
{
	UGFN_E_PrimitiveSphere* newNode = NewObject<UGFN_E_PrimitiveSphere>(_workingGraph);
	newNode->CreateNewGuid();
	newNode->NodePosX = Position.X;
	newNode->NodePosY = Position.Y;

	//centreinput
	UEdGraphPin* CentreUiPin = newNode->CreateCustomPin(EEdGraphPinDirection::EGPD_Input, CentreInput->PinName, EGeoFlowReturnType::Vector);
	newNode->CentreInput = CentreUiPin;
	CentreUiPin->PinId = CentreInput->PinId;
	if (CentreInput->Connection != nullptr) {
		connections.Add(std::make_pair(CentreInput->PinId, CentreInput->Connection->PinId));
	}
	SetVectorDefaultValue(CentreUiPin, centre);
	idToPinMap.Add(CentreInput->PinId, CentreUiPin);

	//radiusinput
	UEdGraphPin* RadiusUiPin = newNode->CreateCustomPin(EEdGraphPinDirection::EGPD_Input, RadiusInput->PinName, EGeoFlowReturnType::Double);
	newNode->RadiusInput = RadiusUiPin;
	RadiusUiPin->PinId = RadiusInput->PinId;
	if (RadiusInput->Connection != nullptr) {
		connections.Add(std::make_pair(RadiusInput->PinId, RadiusInput->Connection->PinId));
	}
	SetDoubleDefaultValue(RadiusUiPin, radius);
	idToPinMap.Add(RadiusInput->PinId, RadiusUiPin);

	//output
	UEdGraphPin* OutputUiPin = newNode->CreateCustomPin(EEdGraphPinDirection::EGPD_Output, Output->PinName, EGeoFlowReturnType::Double);
	newNode->Output = OutputUiPin;
	OutputUiPin->PinId = Output->PinId;
	if (Output->Connection != nullptr) {
		connections.Add(std::make_pair(Output->PinId, Output->Connection->PinId));
	}
	idToPinMap.Add(Output->PinId, OutputUiPin);
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
	UGFN_R_PrimitiveBox* runtimeNode = NewObject<UGFN_R_PrimitiveBox>(runtimeGraph);
	runtimeNode->Position = FVector2D(NodePosX, NodePosY);

	//CentreInput
	UGeoFlowRuntimePin* CentreRuntimePin = NewObject<UGeoFlowRuntimePin>(runtimeNode);
	CentreRuntimePin->PinName = CentreInput->PinName;
	CentreRuntimePin->PinId = CentreInput->PinId;
	if (CentreInput->HasAnyConnections()) {
		std::pair<FGuid, FGuid> connection = std::make_pair(CentreInput->PinId, CentreInput->LinkedTo[0]->PinId);
		connections.Add(connection);
	}
	else {
		//only updating default value when we have no connection *should* mean that it persists underneath
		//not entirely sure if this is true
		runtimeNode->centre = GetVectorDefaultValue(CentreInput);
	}
	idToPinMap.Add(CentreInput->PinId, CentreRuntimePin);
	runtimeNode->CentreInput = CentreRuntimePin;
	CentreRuntimePin->OwningNode = runtimeNode;

	//RadiusInput
	UGeoFlowRuntimePin* RadiusRuntimePin = NewObject<UGeoFlowRuntimePin>(runtimeNode);
	RadiusRuntimePin->PinName = RadiusInput->PinName;
	RadiusRuntimePin->PinId = RadiusInput->PinId;
	if (RadiusInput->HasAnyConnections()) {
		std::pair<FGuid, FGuid> connection = std::make_pair(RadiusInput->PinId, RadiusInput->LinkedTo[0]->PinId);
		connections.Add(connection);
	}
	else {
		runtimeNode->radius = GetVectorDefaultValue(RadiusInput);
	}
	idToPinMap.Add(RadiusInput->PinId, RadiusRuntimePin);
	runtimeNode->RadiusInput = RadiusRuntimePin;
	RadiusRuntimePin->OwningNode = runtimeNode;

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

UGFN_E_Base* UGFN_R_PrimitiveBox::CreateEditorNode(UEdGraph* _workingGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap<FGuid, UEdGraphPin*>& idToPinMap)
{
	UGFN_E_PrimitiveBox* newNode = NewObject<UGFN_E_PrimitiveBox>(_workingGraph);
	newNode->CreateNewGuid();
	newNode->NodePosX = Position.X;
	newNode->NodePosY = Position.Y;

	//centreinput
	UEdGraphPin* CentreUiPin = newNode->CreateCustomPin(EEdGraphPinDirection::EGPD_Input, CentreInput->PinName, EGeoFlowReturnType::Vector);
	newNode->CentreInput = CentreUiPin;
	CentreUiPin->PinId = CentreInput->PinId;
	if (CentreInput->Connection != nullptr) {
		connections.Add(std::make_pair(CentreInput->PinId, CentreInput->Connection->PinId));
	}
	SetVectorDefaultValue(CentreUiPin, centre);
	idToPinMap.Add(CentreInput->PinId, CentreUiPin);

	//radiusinput
	UEdGraphPin* RadiusUiPin = newNode->CreateCustomPin(EEdGraphPinDirection::EGPD_Input, RadiusInput->PinName, EGeoFlowReturnType::Double);
	newNode->RadiusInput = RadiusUiPin;
	RadiusUiPin->PinId = RadiusInput->PinId;
	if (RadiusInput->Connection != nullptr) {
		connections.Add(std::make_pair(RadiusInput->PinId, RadiusInput->Connection->PinId));
	}
	SetVectorDefaultValue(RadiusUiPin, radius);
	idToPinMap.Add(RadiusInput->PinId, RadiusUiPin);

	//output
	UEdGraphPin* OutputUiPin = newNode->CreateCustomPin(EEdGraphPinDirection::EGPD_Output, Output->PinName, EGeoFlowReturnType::Double);
	newNode->Output = OutputUiPin;
	OutputUiPin->PinId = Output->PinId;
	if (Output->Connection != nullptr) {
		connections.Add(std::make_pair(Output->PinId, Output->Connection->PinId));
	}
	idToPinMap.Add(Output->PinId, OutputUiPin);
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
	CentreInput = CreateCustomPin(EGPD_Input, "Normal Axis", EGeoFlowReturnType::Vector);
	RadiusInput = CreateCustomPin(EGPD_Input, "Offset", EGeoFlowReturnType::Double);
	InputPins.Add(CentreInput);
	InputPins.Add(RadiusInput);
	return InputPins;
}
UGFN_R_Base* UGFN_E_PrimitivePlane::CreateRuntimeNode(UGeoFlowRuntimeGraph* runtimeGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap<FGuid, UGeoFlowRuntimePin*>& idToPinMap)
{
	UGFN_R_PrimitivePlane* runtimeNode = NewObject<UGFN_R_PrimitivePlane>(runtimeGraph);
	runtimeNode->Position = FVector2D(NodePosX, NodePosY);

	//CentreInput
	UGeoFlowRuntimePin* CentreRuntimePin = NewObject<UGeoFlowRuntimePin>(runtimeNode);
	CentreRuntimePin->PinName = CentreInput->PinName;
	CentreRuntimePin->PinId = CentreInput->PinId;
	if (CentreInput->HasAnyConnections()) {
		std::pair<FGuid, FGuid> connection = std::make_pair(CentreInput->PinId, CentreInput->LinkedTo[0]->PinId);
		connections.Add(connection);
	}
	else {
		//only updating default value when we have no connection *should* mean that it persists underneath
		//not entirely sure if this is true
		runtimeNode->centre = GetVectorDefaultValue(CentreInput);
	}
	idToPinMap.Add(CentreInput->PinId, CentreRuntimePin);
	runtimeNode->CentreInput = CentreRuntimePin;
	CentreRuntimePin->OwningNode = runtimeNode;

	//RadiusInput
	UGeoFlowRuntimePin* RadiusRuntimePin = NewObject<UGeoFlowRuntimePin>(runtimeNode);
	RadiusRuntimePin->PinName = RadiusInput->PinName;
	RadiusRuntimePin->PinId = RadiusInput->PinId;
	if (RadiusInput->HasAnyConnections()) {
		std::pair<FGuid, FGuid> connection = std::make_pair(RadiusInput->PinId, RadiusInput->LinkedTo[0]->PinId);
		connections.Add(connection);
	}
	else {
		runtimeNode->radius = GetDoubleDefaultValue(RadiusInput);
	}
	idToPinMap.Add(RadiusInput->PinId, RadiusRuntimePin);
	runtimeNode->RadiusInput = RadiusRuntimePin;
	RadiusRuntimePin->OwningNode = runtimeNode;

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

UGFN_E_Base* UGFN_R_PrimitivePlane::CreateEditorNode(UEdGraph* _workingGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap<FGuid, UEdGraphPin*>& idToPinMap)
{
	UGFN_E_PrimitivePlane* newNode = NewObject<UGFN_E_PrimitivePlane>(_workingGraph);
	newNode->CreateNewGuid();
	newNode->NodePosX = Position.X;
	newNode->NodePosY = Position.Y;

	//centreinput
	UEdGraphPin* CentreUiPin = newNode->CreateCustomPin(EEdGraphPinDirection::EGPD_Input, CentreInput->PinName, EGeoFlowReturnType::Vector);
	newNode->CentreInput = CentreUiPin;
	CentreUiPin->PinId = CentreInput->PinId;
	if (CentreInput->Connection != nullptr) {
		connections.Add(std::make_pair(CentreInput->PinId, CentreInput->Connection->PinId));
	}
	SetVectorDefaultValue(CentreUiPin, centre);
	idToPinMap.Add(CentreInput->PinId, CentreUiPin);

	//radiusinput
	UEdGraphPin* RadiusUiPin = newNode->CreateCustomPin(EEdGraphPinDirection::EGPD_Input, RadiusInput->PinName, EGeoFlowReturnType::Double);
	newNode->RadiusInput = RadiusUiPin;
	RadiusUiPin->PinId = RadiusInput->PinId;
	if (RadiusInput->Connection != nullptr) {
		connections.Add(std::make_pair(RadiusInput->PinId, RadiusInput->Connection->PinId));
	}
	SetDoubleDefaultValue(RadiusUiPin, radius);
	idToPinMap.Add(RadiusInput->PinId, RadiusUiPin);

	//output
	UEdGraphPin* OutputUiPin = newNode->CreateCustomPin(EEdGraphPinDirection::EGPD_Output, Output->PinName, EGeoFlowReturnType::Double);
	newNode->Output = OutputUiPin;
	OutputUiPin->PinId = Output->PinId;
	if (Output->Connection != nullptr) {
		connections.Add(std::make_pair(Output->PinId, Output->Connection->PinId));
	}
	idToPinMap.Add(Output->PinId, OutputUiPin);
	return newNode;
}

double UGFN_R_PrimitivePlane::Evaluate(const FVector3d& pos)
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
	FVector3d normal = centre;
	normal.Normalize();
	return FVector3d::DotProduct(transformedPos,normal) + radius;

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
	UGFN_R_PrimitiveEllipsoid* runtimeNode = NewObject<UGFN_R_PrimitiveEllipsoid>(runtimeGraph);
	runtimeNode->Position = FVector2D(NodePosX, NodePosY);

	//CentreInput
	UGeoFlowRuntimePin* CentreRuntimePin = NewObject<UGeoFlowRuntimePin>(runtimeNode);
	CentreRuntimePin->PinName = CentreInput->PinName;
	CentreRuntimePin->PinId = CentreInput->PinId;
	if (CentreInput->HasAnyConnections()) {
		std::pair<FGuid, FGuid> connection = std::make_pair(CentreInput->PinId, CentreInput->LinkedTo[0]->PinId);
		connections.Add(connection);
	}
	else {
		//only updating default value when we have no connection *should* mean that it persists underneath
		//not entirely sure if this is true
		runtimeNode->centre = GetVectorDefaultValue(CentreInput);
	}
	idToPinMap.Add(CentreInput->PinId, CentreRuntimePin);
	runtimeNode->CentreInput = CentreRuntimePin;
	CentreRuntimePin->OwningNode = runtimeNode;

	//RadiusInput
	UGeoFlowRuntimePin* RadiusRuntimePin = NewObject<UGeoFlowRuntimePin>(runtimeNode);
	RadiusRuntimePin->PinName = RadiusInput->PinName;
	RadiusRuntimePin->PinId = RadiusInput->PinId;
	if (RadiusInput->HasAnyConnections()) {
		std::pair<FGuid, FGuid> connection = std::make_pair(RadiusInput->PinId, RadiusInput->LinkedTo[0]->PinId);
		connections.Add(connection);
	}
	else {
		runtimeNode->radius = GetVectorDefaultValue(RadiusInput);
	}
	idToPinMap.Add(RadiusInput->PinId, RadiusRuntimePin);
	runtimeNode->RadiusInput = RadiusRuntimePin;
	RadiusRuntimePin->OwningNode = runtimeNode;

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

UGFN_E_Base* UGFN_R_PrimitiveEllipsoid::CreateEditorNode(UEdGraph* _workingGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap<FGuid, UEdGraphPin*>& idToPinMap)
{
	UGFN_E_PrimitiveEllipsoid* newNode = NewObject<UGFN_E_PrimitiveEllipsoid>(_workingGraph);
	newNode->CreateNewGuid();
	newNode->NodePosX = Position.X;
	newNode->NodePosY = Position.Y;

	//centreinput
	UEdGraphPin* CentreUiPin = newNode->CreateCustomPin(EEdGraphPinDirection::EGPD_Input, CentreInput->PinName, EGeoFlowReturnType::Vector);
	newNode->CentreInput = CentreUiPin;
	CentreUiPin->PinId = CentreInput->PinId;
	if (CentreInput->Connection != nullptr) {
		connections.Add(std::make_pair(CentreInput->PinId, CentreInput->Connection->PinId));
	}
	SetVectorDefaultValue(CentreUiPin, centre);
	idToPinMap.Add(CentreInput->PinId, CentreUiPin);

	//radiusinput
	UEdGraphPin* RadiusUiPin = newNode->CreateCustomPin(EEdGraphPinDirection::EGPD_Input, RadiusInput->PinName, EGeoFlowReturnType::Double);
	newNode->RadiusInput = RadiusUiPin;
	RadiusUiPin->PinId = RadiusInput->PinId;
	if (RadiusInput->Connection != nullptr) {
		connections.Add(std::make_pair(RadiusInput->PinId, RadiusInput->Connection->PinId));
	}
	SetVectorDefaultValue(RadiusUiPin, radius);
	idToPinMap.Add(RadiusInput->PinId, RadiusUiPin);

	//output
	UEdGraphPin* OutputUiPin = newNode->CreateCustomPin(EEdGraphPinDirection::EGPD_Output, Output->PinName, EGeoFlowReturnType::Double);
	newNode->Output = OutputUiPin;
	OutputUiPin->PinId = Output->PinId;
	if (Output->Connection != nullptr) {
		connections.Add(std::make_pair(Output->PinId, Output->Connection->PinId));
	}
	idToPinMap.Add(Output->PinId, OutputUiPin);
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
