#pragma once
#include "GeoFlowNodeTypes.h"
#include "GeoFlowPinDefaultValueOps.h"
#include "GeoFlowConstantNodes.generated.h"
/*
	GeoFlowConstantNodes.h
	Defines Editor and Runtime Nodes
	For:
		Constant return value Nodes
*/

/*
	Editor
*/
UCLASS()
class GEOFLOWCORE_API UGFN_E_ConstantBool : public UGFN_E_BaseBool {
	GENERATED_BODY()
public:
	virtual EGeoFlowNodeType NodeType() const override { return EGeoFlowNodeType::ConstBool; }

	virtual FText GetNodeTitle(ENodeTitleType::Type titleType) const override { return FText::FromString("Constant boolean"); }

	virtual UGFN_R_Base* CreateRuntimeNode(UGeoFlowRuntimeGraph* runtimeGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap < FGuid, UGeoFlowRuntimePin*>& idToPinMap) override;
	virtual TArray<UEdGraphPin*> CreateInputPins(UEdGraphPin* fromPin) {
		TArray<UEdGraphPin*> output;
		ValuePin = CreateCustomPin(EGPD_Input, "Value", EGeoFlowReturnType::Bool);
		output.Add(ValuePin);
		ValuePin->bNotConnectable = true;
		return output;
	};
	virtual void PinDefaultValueChanged(UEdGraphPin* Pin) override {
		if (Pin == ValuePin) {
			Value = GetBoolDefaultValue(Pin);
		}
		Super::PinDefaultValueChanged(Pin);
	}
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override {
		if (PropertyChangedEvent.GetMemberPropertyName() == GET_MEMBER_NAME_CHECKED(UGFN_E_ConstantBool, Value)) {
			SetBoolDefaultValue(ValuePin, Value);
		}
		Super::PostEditChangeProperty(PropertyChangedEvent);
	}
	UEdGraphPin* ValuePin;
	UPROPERTY(EditAnywhere)
	bool Value = false;
};
UCLASS()
class GEOFLOWCORE_API UGFN_E_ConstantInt : public UGFN_E_BaseInt {
	GENERATED_BODY()
public:
	virtual EGeoFlowNodeType NodeType() const override { return EGeoFlowNodeType::ConstInt; }

	virtual FText GetNodeTitle(ENodeTitleType::Type titleType) const override { return FText::FromString("Constant integer"); }

	virtual UGFN_R_Base* CreateRuntimeNode(UGeoFlowRuntimeGraph* runtimeGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap < FGuid, UGeoFlowRuntimePin*>& idToPinMap) override;
	virtual void PinDefaultValueChanged(UEdGraphPin* Pin) override {
		if (Pin == ValuePin) {
			Value = GetIntDefaultValue(Pin);
		}
		Super::PinDefaultValueChanged(Pin);
	}
	virtual TArray<UEdGraphPin*> CreateInputPins(UEdGraphPin* fromPin) {
		TArray<UEdGraphPin*> output;
		ValuePin = CreateCustomPin(EGPD_Input, "Value", EGeoFlowReturnType::Int);
		output.Add(ValuePin);
		ValuePin->bNotConnectable = true;
		return output;
	};
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override {
		if (PropertyChangedEvent.GetMemberPropertyName() == GET_MEMBER_NAME_CHECKED(UGFN_E_ConstantInt, Value)) {
			SetIntDefaultValue(ValuePin, Value);
		}
		Super::PostEditChangeProperty(PropertyChangedEvent);
	}
	UEdGraphPin* ValuePin;
	UPROPERTY(EditAnywhere)
	int Value = 0;
};
UCLASS()
class GEOFLOWCORE_API UGFN_E_ConstantFloat : public UGFN_E_BaseFloat {
	GENERATED_BODY()
public:
	virtual EGeoFlowNodeType NodeType() const override { return EGeoFlowNodeType::ConstDouble; }

	virtual FText GetNodeTitle(ENodeTitleType::Type titleType) const override { return FText::FromString("Constant float"); }

	virtual UGFN_R_Base* CreateRuntimeNode(UGeoFlowRuntimeGraph* runtimeGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap < FGuid, UGeoFlowRuntimePin*>& idToPinMap) override;
	virtual void PinDefaultValueChanged(UEdGraphPin* Pin) override {
		if (Pin == ValuePin) {
			Value = GetFloatDefaultValue(Pin);
		}
		Super::PinDefaultValueChanged(Pin);
	}
	virtual TArray<UEdGraphPin*> CreateInputPins(UEdGraphPin* fromPin) {
		/*base class does not make pins*/
		TArray<UEdGraphPin*> output;
		ValuePin = CreateCustomPin(EGPD_Input, "Value", EGeoFlowReturnType::Float);
		output.Add(ValuePin);
		ValuePin->bNotConnectable = true;
		return output;
	};
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override {
		if (PropertyChangedEvent.GetMemberPropertyName() == GET_MEMBER_NAME_CHECKED(UGFN_E_ConstantFloat, Value)) {
			SetFloatDefaultValue(ValuePin, Value);
		}
		Super::PostEditChangeProperty(PropertyChangedEvent);
	}
	UEdGraphPin* ValuePin;
	UPROPERTY(EditAnywhere)
	double Value = 0.0;
};
UCLASS(ShowCategories = ("Viewport"))
class GEOFLOWCORE_API UGFN_E_ConstantVector : public UGFN_E_BaseVector {
	GENERATED_BODY()
public:
	virtual EGeoFlowNodeType NodeType() const override { return EGeoFlowNodeType::ConstVector; }

	virtual FText GetNodeTitle(ENodeTitleType::Type titleType) const override { return FText::FromString("Constant vector"); }

	virtual UGFN_R_Base* CreateRuntimeNode(UGeoFlowRuntimeGraph* runtimeGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap < FGuid, UGeoFlowRuntimePin*>& idToPinMap) override;
	virtual TArray<UEdGraphPin*> CreateInputPins(UEdGraphPin* fromPin) {
		TArray<UEdGraphPin*> output;
		ValuePin = CreateCustomPin(EGPD_Input, "Value", EGeoFlowReturnType::Vector);
		output.Add(ValuePin);
		ValuePin->bNotConnectable = true;
		
		return output;
	};
	virtual void PinDefaultValueChanged(UEdGraphPin* Pin) override {
		if (Pin == ValuePin) {
			Value = GetVectorDefaultValue(Pin);
		}
		Super::PinDefaultValueChanged(Pin);
	}
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override {
		if (PropertyChangedEvent.GetMemberPropertyName() == GET_MEMBER_NAME_CHECKED(UGFN_E_ConstantVector, Value)) {
			SetVectorDefaultValue(ValuePin, Value);
		}
		Super::PostEditChangeProperty(PropertyChangedEvent);
	}
	UEdGraphPin* ValuePin;
	UPROPERTY(EditAnywhere)
	FVector3f Value = FVector3f::ZeroVector;

	virtual void DrawInViewport(UWorld* InWorld) override;
	
};
UCLASS()
class GEOFLOWCORE_API UGFN_E_ConstantArray : public UGFN_E_BaseArray {
	GENERATED_BODY()
public:
	virtual EGeoFlowNodeType NodeType() const override { return EGeoFlowNodeType::ConstArray; }

	virtual FText GetNodeTitle(ENodeTitleType::Type titleType) const override { return FText::FromString("Constant vector array"); }

	virtual UGFN_R_Base* CreateRuntimeNode(UGeoFlowRuntimeGraph* runtimeGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap < FGuid, UGeoFlowRuntimePin*>& idToPinMap) override;
	virtual TArray<UEdGraphPin*> CreateInputPins(UEdGraphPin* fromPin) {
		TArray<UEdGraphPin*> output;
		/*
		ValuePin = CreateCustomPin(EGPD_Input, "Value", EGeoFlowReturnType::Vector);
		output.Add(ValuePin);
		ValuePin->bNotConnectable = true;
		*/
		return output;
	};
	
	//UEdGraphPin* ValuePin;
	UPROPERTY(EditAnywhere)
	TArray<FGeoFlowTransform> Value = TArray<FGeoFlowTransform>();
};
/*
	Runtime
*/
UCLASS()
class GEOFLOWCORE_API UGFN_R_ConstantBool : public UGFN_R_BaseBool {
	GENERATED_BODY()
public:
	virtual EGeoFlowReturnType NodeReturnType() const override{ return EGeoFlowReturnType::Bool; }
	virtual EGeoFlowNodeType NodeType() const override { return EGeoFlowNodeType::ConstBool; }

	virtual UGFN_E_Base* CreateEditorNode(UEdGraph* _workingGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap < FGuid, UEdGraphPin*>& idToPinMap) override;
	UPROPERTY()
	bool Value;
	
	virtual bool Evaluate(const FVector3f& pos) override { return Value; }
};
UCLASS()
class GEOFLOWCORE_API UGFN_R_ConstantInt : public UGFN_R_BaseInt {
	GENERATED_BODY()
public:
	virtual EGeoFlowReturnType NodeReturnType() const override { return EGeoFlowReturnType::Int; }
	virtual EGeoFlowNodeType NodeType() const override { return EGeoFlowNodeType::ConstInt; }

	virtual UGFN_E_Base* CreateEditorNode(UEdGraph* _workingGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap < FGuid, UEdGraphPin*>& idToPinMap) override;
	UPROPERTY()
	int Value;
	
	virtual int Evaluate(const FVector3f& pos) override { return Value; }
};
UCLASS()
class GEOFLOWCORE_API UGFN_R_ConstantFloat : public UGFN_R_BaseFloat {
	GENERATED_BODY()
public:
	virtual EGeoFlowReturnType NodeReturnType() const override { return EGeoFlowReturnType::Float; }
	virtual EGeoFlowNodeType NodeType() const override { return EGeoFlowNodeType::ConstDouble; }

	virtual UGFN_E_Base* CreateEditorNode(UEdGraph* _workingGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap < FGuid, UEdGraphPin*>& idToPinMap) override;
	UPROPERTY()
	float Value;

	virtual float Evaluate(const FVector3f& pos) override { return Value; }
};
UCLASS()
class GEOFLOWCORE_API UGFN_R_ConstantVector : public UGFN_R_BaseVector {
	GENERATED_BODY()
public:
	virtual EGeoFlowReturnType NodeReturnType() const override { return EGeoFlowReturnType::Vector; }
	virtual EGeoFlowNodeType NodeType() const override { return EGeoFlowNodeType::ConstVector; }

	virtual UGFN_E_Base* CreateEditorNode(UEdGraph* _workingGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap < FGuid, UEdGraphPin*>& idToPinMap) override;
	UPROPERTY()
	FVector3f Value;

	virtual FVector3f Evaluate(const FVector3f& pos) override { return Value; }

};

UCLASS()
class GEOFLOWCORE_API UGFN_R_ConstantArray : public UGFN_R_BaseArray {
	GENERATED_BODY()
public:
	virtual EGeoFlowReturnType NodeReturnType() const override { return EGeoFlowReturnType::Array; }
	virtual EGeoFlowNodeType NodeType() const override { return EGeoFlowNodeType::ConstArray; }
	
	virtual UGFN_E_Base* CreateEditorNode(UEdGraph* _workingGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap < FGuid, UEdGraphPin*>& idToPinMap) override;
	UPROPERTY()
	TArray<FGeoFlowTransform> Value;

	virtual TArray<FGeoFlowTransform> Evaluate(const FVector3f& pos) override { return Value; }

};

GEOFLOWCORE_API bool IsConstantNode(UEdGraphNode* Node);