#pragma once

#include "GeoFlowNodeEnum.h"
#include "GeoFlowNodeTypes.Generated.h"
/*
	GeoFlowNodeTypes.h
	Defines Editor and Runtime Nodes
	For:
		Base class
		Output node
		All different possible return types	
*/

class UGFN_R_Base;
class UGeoFlowRuntimeGraph;
class UGeoFlowRuntimePin;
/*
	Editor
*/
UCLASS()
class GEOFLOWCORE_API UGFN_E_Base : public UEdGraphNode
{
	GENERATED_BODY()
public:
	virtual EGeoFlowReturnType NodeReturnType() const { return EGeoFlowReturnType::None; }
	virtual EGeoFlowNodeType NodeType() const { return EGeoFlowNodeType::Base; }
	
	virtual UGFN_R_Base* CreateRuntimeNode(UGeoFlowRuntimeGraph* runtimeGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap < FGuid, UGeoFlowRuntimePin*>& idToPinMap) { return nullptr; /*only saveable nodes override this*/ }
	virtual TArray<UEdGraphPin*> CreateInputPins(UEdGraphPin* fromPin) {
		/*base class does not make pins*/
		return TArray<UEdGraphPin*>();
	};
	virtual TArray<UEdGraphPin*> CreateOutputPins() {
		/*Derived classes without outputs MUST override this to nothing*/ 
		TArray<UEdGraphPin*> OutputPins;
		Output = CreateCustomPin(EEdGraphPinDirection::EGPD_Output, TEXT("Output"),NodeReturnType());
		OutputPins.Add(Output);
		return OutputPins;
	};
	
	bool needsResave = false;
	virtual void PinDefaultValueChanged(UEdGraphPin* Pin) override { needsResave = true; }
	virtual void PinConnectionListChanged(UEdGraphPin* Pin) override { needsResave = true; }



	virtual FText GetNodeTitle(ENodeTitleType::Type titleType) const override { return FText::FromString("Default node title"); }
	virtual FLinearColor GetNodeTitleColor() const override { return FLinearColor::Green; }
	virtual bool CanUserDeleteNode() const override { return true; }

	virtual void GetNodeContextMenuActions(class UToolMenu* menu, class UGraphNodeContextMenuContext* context)const override;

	UGeoFlowRuntimePin* InitRuntimePin(UGFN_R_Base* runtimeNode, UEdGraphPin* UiPin, TArray<std::pair<FGuid, FGuid>>& connections, TMap<FGuid, UGeoFlowRuntimePin*>& idToPinMap, EGeoFlowReturnType returnType = EGeoFlowReturnType::None);

	UEdGraphPin* CreateCustomPin(EEdGraphPinDirection direction, FName name, EGeoFlowReturnType ConnectionType);
	template<typename T>
	T* InitRuntimeNode(UGeoFlowRuntimeGraph* runtimeGraph);

	UEdGraphPin* Output = nullptr;
};
UCLASS()
class GEOFLOWCORE_API UGFN_E_Output : public UGFN_E_Base {
	GENERATED_BODY()
public:
	virtual EGeoFlowReturnType NodeReturnType() const override { return EGeoFlowReturnType::None; }
	virtual EGeoFlowNodeType NodeType() const override { return EGeoFlowNodeType::Output; }

	virtual UGFN_R_Base* CreateRuntimeNode(UGeoFlowRuntimeGraph* runtimeGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap < FGuid, UGeoFlowRuntimePin*>& idToPinMap) override;
	virtual TArray<UEdGraphPin*> CreateInputPins(UEdGraphPin* fromPin) override;
	virtual TArray<UEdGraphPin*> CreateOutputPins() {
		/*no output on final node*/ 
		return TArray<UEdGraphPin*>();
	}
	virtual FText GetNodeTitle(ENodeTitleType::Type titleType) const override { return FText::FromString("Output"); }
	virtual FLinearColor GetNodeTitleColor() const override { return FLinearColor::Black; }
	virtual bool CanUserDeleteNode() const override { return false; }

	virtual void GetNodeContextMenuActions(class UToolMenu* menu, class UGraphNodeContextMenuContext* context)const override;
private:
	FName GetNextInputName() const;
};
UCLASS()
class GEOFLOWCORE_API UGFN_E_BaseBool : public UGFN_E_Base {
	GENERATED_BODY()
public:
	virtual EGeoFlowReturnType NodeReturnType() const override { return EGeoFlowReturnType::Bool; }
	virtual EGeoFlowNodeType NodeType() const override { return EGeoFlowNodeType::BaseBool; }


	virtual FText GetNodeTitle(ENodeTitleType::Type titleType) const override { return FText::FromString("Default Boolean Node"); }
	virtual FLinearColor GetNodeTitleColor() const override { return FLinearColor::Red; }
};
UCLASS()
class GEOFLOWCORE_API UGFN_E_BaseInt : public UGFN_E_Base {
	GENERATED_BODY()
public:
	virtual EGeoFlowReturnType NodeReturnType() const override { return EGeoFlowReturnType::Int; }
	virtual EGeoFlowNodeType NodeType() const override { return EGeoFlowNodeType::BaseInt; }

	virtual FText GetNodeTitle(ENodeTitleType::Type titleType) const override { return FText::FromString("Default Integer Node"); }
	virtual FLinearColor GetNodeTitleColor() const override { return FLinearColor::Blue; }
};
UCLASS()
class GEOFLOWCORE_API UGFN_E_BaseFloat : public UGFN_E_Base {
	GENERATED_BODY()
public:
	virtual EGeoFlowReturnType NodeReturnType() const override { return EGeoFlowReturnType::Float; }
	virtual EGeoFlowNodeType NodeType() const override { return EGeoFlowNodeType::BaseFloat; }

	virtual FText GetNodeTitle(ENodeTitleType::Type titleType) const override { return FText::FromString("Default Float node"); }
	virtual FLinearColor GetNodeTitleColor() const override { return FLinearColor::Green; }
};
UCLASS()
class GEOFLOWCORE_API UGFN_E_BaseVector : public UGFN_E_Base {
	GENERATED_BODY()
public:
	virtual EGeoFlowReturnType NodeReturnType() const override { return EGeoFlowReturnType::Vector; }
	virtual EGeoFlowNodeType NodeType() const override { return EGeoFlowNodeType::BaseVector; }


	virtual FText GetNodeTitle(ENodeTitleType::Type titleType) const override { return FText::FromString("Default Vector node"); }
	virtual FLinearColor GetNodeTitleColor() const override { return FLinearColor::Yellow; }
};
/*
	Runtime
*/
UCLASS()
class GEOFLOWCORE_API UGFN_R_Base : public UObject {
	GENERATED_BODY()
public:
	UPROPERTY()
	FVector2D NodePosition;
	virtual EGeoFlowReturnType NodeReturnType() const { return EGeoFlowReturnType::None; }
	virtual EGeoFlowNodeType NodeType() const { return EGeoFlowNodeType::Base; }

	virtual UGFN_E_Base* CreateEditorNode(UEdGraph* _workingGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap < FGuid, UEdGraphPin*>& idToPinMap) { return nullptr; /*only saveable nodes override this*/ }
	UEdGraphPin* InitUiPin(UGFN_E_Base* newNode, UGeoFlowRuntimePin* runtimePin, TArray<std::pair<FGuid, FGuid>>& connections, TMap < FGuid, UEdGraphPin*>& idToPinMap);
	template<typename T>
	T* InitUiNode(UEdGraph* _workingGraph);
	virtual FString CreateShaderEvalCall(TArray<FString>& PinDeclarations) { return FString(); }
	UPROPERTY()
	UGeoFlowRuntimePin* Output = nullptr;
};
UCLASS()
class GEOFLOWCORE_API UGFN_R_Output : public UGFN_R_Base {
	GENERATED_BODY()
public:
	virtual EGeoFlowReturnType NodeReturnType() const override { return EGeoFlowReturnType::None; }
	virtual EGeoFlowNodeType NodeType() const override { return EGeoFlowNodeType::Output; }

	virtual UGFN_E_Base* CreateEditorNode(UEdGraph* _workingGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap < FGuid, UEdGraphPin*>& idToPinMap);
	virtual float Evaluate(const FVector3f& pos);
	virtual FString CreateShaderEvalCall(TArray<FString>& PinDeclarations) override;
	UPROPERTY()
	TArray<UGeoFlowRuntimePin*> InputPins;
};
UCLASS()
class GEOFLOWCORE_API UGFN_R_BaseBool : public UGFN_R_Base {
	GENERATED_BODY()
public:
	virtual EGeoFlowReturnType NodeReturnType() const override { return EGeoFlowReturnType::Bool; }
	virtual EGeoFlowNodeType NodeType() const override { return EGeoFlowNodeType::BaseBool; }
	virtual bool Evaluate(const FVector3f& pos) { return false; }
};
UCLASS()
class GEOFLOWCORE_API UGFN_R_BaseInt : public UGFN_R_Base {
	GENERATED_BODY()
public:
	virtual EGeoFlowReturnType NodeReturnType() const override { return EGeoFlowReturnType::Int; }
	virtual EGeoFlowNodeType NodeType() const override { return EGeoFlowNodeType::BaseInt; }
	virtual int Evaluate(const FVector3f& pos) { return 0; }
};
UCLASS()
class GEOFLOWCORE_API UGFN_R_BaseFloat : public UGFN_R_Base {
	GENERATED_BODY()
public:
	virtual EGeoFlowReturnType NodeReturnType() const override { return EGeoFlowReturnType::Float; }
	virtual EGeoFlowNodeType NodeType() const override { return EGeoFlowNodeType::BaseFloat; }
	virtual float Evaluate(const FVector3f& pos) { return 0.0; }
};
UCLASS()
class GEOFLOWCORE_API UGFN_R_BaseVector : public UGFN_R_Base {
	GENERATED_BODY()
public:
	virtual EGeoFlowReturnType NodeReturnType() const override { return EGeoFlowReturnType::Vector; }
	virtual EGeoFlowNodeType NodeType() const override { return EGeoFlowNodeType::BaseVector; }
	virtual FVector3f Evaluate(const FVector3f& pos) { return FVector3f::ZeroVector; }
};

template<typename T>
inline T* UGFN_R_Base::InitUiNode(UEdGraph* _workingGraph)
{
	T* newNode = NewObject<T>(_workingGraph);
	newNode->CreateNewGuid();
	newNode->NodePosX = NodePosition.X;
	newNode->NodePosY = NodePosition.Y;
	return newNode;
}

template<typename T>
inline T* UGFN_E_Base::InitRuntimeNode(UGeoFlowRuntimeGraph* runtimeGraph)
{
	T* runtimeNode = NewObject<T>(runtimeGraph);
	runtimeNode->NodePosition = FVector2D(NodePosX, NodePosY);
	return runtimeNode;
}
