#pragma once
#include "EdGraph/EdGraphPin.h"
#include "GeoFlowNodeTypes.h"
#include "GeoFlowPinDefaultValueOps.h"
#include "GeoFlowMathNodes.generated.h"



UCLASS()
class GEOFLOWCORE_API UGFN_E_MakeVector : public UGFN_E_BaseVector {
	GENERATED_BODY()
public:
	virtual EGeoFlowNodeType NodeType() const override { return EGeoFlowNodeType::MakeVector; }
	virtual FText GetNodeTitle(ENodeTitleType::Type titleType) const override { return FText::FromString("Make Vector"); }
	virtual UGFN_R_Base* CreateRuntimeNode(UGeoFlowRuntimeGraph* runtimeGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap < FGuid, UGeoFlowRuntimePin*>& idToPinMap) override;
	virtual TArray<UEdGraphPin*> CreateInputPins(UEdGraphPin* fromPin) override;
	virtual void PinDefaultValueChanged(UEdGraphPin* Pin) override {
		if (Pin == Input) {
			input = GetFloatDefaultValue(Pin);
		}
		Super::PinDefaultValueChanged(Pin);
	}
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override {
		if (PropertyChangedEvent.GetMemberPropertyName() == GET_MEMBER_NAME_CHECKED(UGFN_E_MakeVector, input)) {
			SetFloatDefaultValue(Input, input);
		}

		Super::PostEditChangeProperty(PropertyChangedEvent);
	}
	UEdGraphPin* Input = nullptr;
	UPROPERTY(EditAnywhere, Category = "Node Properties")
	float input;
};
UCLASS()
class GEOFLOWCORE_API UGFN_R_MakeVector : public UGFN_R_BaseVector {
	GENERATED_BODY()
public:
	virtual EGeoFlowNodeType NodeType() const override { return EGeoFlowNodeType::FloatAdd; }

	virtual UGFN_E_Base* CreateEditorNode(UEdGraph* _workingGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap < FGuid, UEdGraphPin*>& idToPinMap) override;
	UPROPERTY()
	UGeoFlowRuntimePin* Input = nullptr;
	UPROPERTY()
	float inputVal;
	virtual FVector3f Evaluate(const FVector3f& pos) override;
};
UCLASS()
class GEOFLOWCORE_API UGFN_E_FloatMathBase : public UGFN_E_BaseFloat {
	GENERATED_BODY()
public:
	virtual EGeoFlowNodeType NodeType() const override { return EGeoFlowNodeType::FloatMath; }
	virtual FText GetNodeTitle(ENodeTitleType::Type titleType) const override { return FText::FromString("FloatMath"); }

	UGFN_R_Base* CreateRuntimeNodeSpecial(UGFN_R_FloatMathBase* runtimeNode, UGeoFlowRuntimeGraph* runtimeGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap < FGuid, UGeoFlowRuntimePin*>& idToPinMap);
	virtual TArray<UEdGraphPin*> CreateInputPins(UEdGraphPin* fromPin) override;
	virtual void PinDefaultValueChanged(UEdGraphPin* Pin) override {
		if (Pin == InputA) {
			a = GetFloatDefaultValue(Pin);
		}
		if (Pin == InputB) {
			b = GetFloatDefaultValue(Pin);
		}
		Super::PinDefaultValueChanged(Pin);
	}
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override {
		if (PropertyChangedEvent.GetMemberPropertyName() == GET_MEMBER_NAME_CHECKED(UGFN_E_FloatMathBase, a)) {
			SetFloatDefaultValue(InputA, a);
		}
		if (PropertyChangedEvent.GetMemberPropertyName() == GET_MEMBER_NAME_CHECKED(UGFN_E_FloatMathBase, b)) {
			SetFloatDefaultValue(InputB, b);
		}
		Super::PostEditChangeProperty(PropertyChangedEvent);
	}
	UEdGraphPin* InputA = nullptr;
	UEdGraphPin* InputB = nullptr;
	UPROPERTY(EditAnywhere, Category = "Node Properties")
	float a;
	UPROPERTY(EditAnywhere, Category = "Node Properties")
	float b;


};
UCLASS()
class GEOFLOWCORE_API UGFN_R_FloatMathBase : public UGFN_R_BaseFloat {
	GENERATED_BODY()
public:
	virtual EGeoFlowNodeType NodeType() const override { return EGeoFlowNodeType::FloatMath; }

	UGFN_E_Base* CreateEditorNodeSpecial(UGFN_E_FloatMathBase* newNode,UEdGraph* _workingGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap < FGuid, UEdGraphPin*>& idToPinMap);

	UPROPERTY()
	UGeoFlowRuntimePin* InputA = nullptr;
	UPROPERTY()
	UGeoFlowRuntimePin* InputB = nullptr;

	UPROPERTY()
	float a;
	UPROPERTY()
	float b;
};
UCLASS()
class GEOFLOWCORE_API UGFN_E_FloatAdd : public UGFN_E_FloatMathBase {
	GENERATED_BODY()
public:
	virtual EGeoFlowNodeType NodeType() const override { return EGeoFlowNodeType::FloatAdd; }
	virtual FText GetNodeTitle(ENodeTitleType::Type titleType) const override { return FText::FromString("Float Add"); }
	virtual UGFN_R_Base* CreateRuntimeNode(UGeoFlowRuntimeGraph* runtimeGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap < FGuid, UGeoFlowRuntimePin*>& idToPinMap) override;
};
UCLASS()
class GEOFLOWCORE_API UGFN_R_FloatAdd : public UGFN_R_FloatMathBase {
	GENERATED_BODY()
public:
	virtual EGeoFlowNodeType NodeType() const override { return EGeoFlowNodeType::FloatAdd; }

	virtual UGFN_E_Base* CreateEditorNode(UEdGraph* _workingGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap < FGuid, UEdGraphPin*>& idToPinMap) override;
	virtual float Evaluate(const FVector3f& pos) override;
};
UCLASS()
class GEOFLOWCORE_API UGFN_E_FloatMultiply : public UGFN_E_FloatMathBase {
	GENERATED_BODY()
public:
	virtual EGeoFlowNodeType NodeType() const override { return EGeoFlowNodeType::FloatMultiply; }
	virtual FText GetNodeTitle(ENodeTitleType::Type titleType) const override { return FText::FromString("Float Multiply"); }
	virtual UGFN_R_Base* CreateRuntimeNode(UGeoFlowRuntimeGraph* runtimeGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap < FGuid, UGeoFlowRuntimePin*>& idToPinMap) override;
};
UCLASS()
class GEOFLOWCORE_API UGFN_R_FloatMultiply : public UGFN_R_FloatMathBase {
	GENERATED_BODY()
public:
	virtual EGeoFlowNodeType NodeType() const override { return EGeoFlowNodeType::FloatMultiply; }

	virtual UGFN_E_Base* CreateEditorNode(UEdGraph* _workingGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap < FGuid, UEdGraphPin*>& idToPinMap) override;
	virtual float Evaluate(const FVector3f& pos) override;
};

UCLASS()
class GEOFLOWCORE_API UGFN_E_FloatSubtract : public UGFN_E_FloatMathBase {
	GENERATED_BODY()
public:
	virtual EGeoFlowNodeType NodeType() const override { return EGeoFlowNodeType::FloatSubtract; }
	virtual FText GetNodeTitle(ENodeTitleType::Type titleType) const override { return FText::FromString("Float Subtract"); }
	virtual UGFN_R_Base* CreateRuntimeNode(UGeoFlowRuntimeGraph* runtimeGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap < FGuid, UGeoFlowRuntimePin*>& idToPinMap) override;
};
UCLASS()
class GEOFLOWCORE_API UGFN_R_FloatSubtract : public UGFN_R_FloatMathBase {
	GENERATED_BODY()
public:
	virtual EGeoFlowNodeType NodeType() const override { return EGeoFlowNodeType::FloatSubtract; }

	virtual UGFN_E_Base* CreateEditorNode(UEdGraph* _workingGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap < FGuid, UEdGraphPin*>& idToPinMap) override;
	virtual float Evaluate(const FVector3f& pos) override;
};

UCLASS()
class GEOFLOWCORE_API UGFN_E_FloatDivide : public UGFN_E_FloatMathBase {
	GENERATED_BODY()
public:
	virtual EGeoFlowNodeType NodeType() const override { return EGeoFlowNodeType::FloatDivide; }
	virtual FText GetNodeTitle(ENodeTitleType::Type titleType) const override { return FText::FromString("Float Divide"); }
	virtual UGFN_R_Base* CreateRuntimeNode(UGeoFlowRuntimeGraph* runtimeGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap < FGuid, UGeoFlowRuntimePin*>& idToPinMap) override;
};
UCLASS()
class GEOFLOWCORE_API UGFN_R_FloatDivide : public UGFN_R_FloatMathBase {
	GENERATED_BODY()
public:
	virtual EGeoFlowNodeType NodeType() const override { return EGeoFlowNodeType::FloatDivide; }

	virtual UGFN_E_Base* CreateEditorNode(UEdGraph* _workingGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap < FGuid, UEdGraphPin*>& idToPinMap) override;
	virtual float Evaluate(const FVector3f& pos) override;
};



UCLASS()
class GEOFLOWCORE_API UGFN_E_VectorMathBase : public UGFN_E_BaseVector {
	GENERATED_BODY()
public:
	virtual EGeoFlowNodeType NodeType() const override { return EGeoFlowNodeType::VectorMath; }
	virtual FText GetNodeTitle(ENodeTitleType::Type titleType) const override { return FText::FromString("VectorMath"); }

	UGFN_R_Base* CreateRuntimeNodeSpecial(UGFN_R_VectorMathBase* runtimeNode, UGeoFlowRuntimeGraph* runtimeGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap < FGuid, UGeoFlowRuntimePin*>& idToPinMap);
	virtual TArray<UEdGraphPin*> CreateInputPins(UEdGraphPin* fromPin) override;
	virtual void PinDefaultValueChanged(UEdGraphPin* Pin) override {
		if (Pin == InputA) {
			a = GetVectorDefaultValue(Pin);
		}
		if (Pin == InputB) {
			b = GetVectorDefaultValue(Pin);
		}
		Super::PinDefaultValueChanged(Pin);
	}
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override {
		if (PropertyChangedEvent.GetMemberPropertyName() == GET_MEMBER_NAME_CHECKED(UGFN_E_VectorMathBase, a)) {
			SetVectorDefaultValue(InputA, a);
		}
		if (PropertyChangedEvent.GetMemberPropertyName() == GET_MEMBER_NAME_CHECKED(UGFN_E_VectorMathBase, b)) {
			SetVectorDefaultValue(InputB, b);
		}
		Super::PostEditChangeProperty(PropertyChangedEvent);
	}
	UEdGraphPin* InputA = nullptr;
	UEdGraphPin* InputB = nullptr;
	UPROPERTY(EditAnywhere, Category = "Node Properties")
	FVector3f a;
	UPROPERTY(EditAnywhere, Category = "Node Properties")
	FVector3f b;

};
UCLASS()
class GEOFLOWCORE_API UGFN_R_VectorMathBase : public UGFN_R_BaseVector {
	GENERATED_BODY()
public:
	virtual EGeoFlowNodeType NodeType() const override { return EGeoFlowNodeType::VectorMath; }

	UGFN_E_Base* CreateEditorNodeSpecial(UGFN_E_VectorMathBase* newNode,UEdGraph* _workingGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap < FGuid, UEdGraphPin*>& idToPinMap);

	UPROPERTY()
	UGeoFlowRuntimePin* InputA = nullptr;
	UPROPERTY()
	UGeoFlowRuntimePin* InputB = nullptr;

	UPROPERTY()
	FVector3f a;
	UPROPERTY()
	FVector3f b;
};


UCLASS()
class GEOFLOWCORE_API UGFN_E_VectorAdd : public UGFN_E_VectorMathBase {
	GENERATED_BODY()
public:
	virtual EGeoFlowNodeType NodeType() const override { return EGeoFlowNodeType::VectorAdd; }
	virtual FText GetNodeTitle(ENodeTitleType::Type titleType) const override { return FText::FromString("Vector Add"); }
	virtual UGFN_R_Base* CreateRuntimeNode(UGeoFlowRuntimeGraph* runtimeGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap < FGuid, UGeoFlowRuntimePin*>& idToPinMap) override;
};
UCLASS()
class GEOFLOWCORE_API UGFN_R_VectorAdd : public UGFN_R_VectorMathBase {
	GENERATED_BODY()
public:
	virtual EGeoFlowNodeType NodeType() const override { return EGeoFlowNodeType::VectorAdd; }

	virtual UGFN_E_Base* CreateEditorNode(UEdGraph* _workingGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap < FGuid, UEdGraphPin*>& idToPinMap) override;
	virtual FVector3f Evaluate(const FVector3f& pos) override;
};
UCLASS()
class GEOFLOWCORE_API UGFN_E_VectorMultiply : public UGFN_E_VectorMathBase {
	GENERATED_BODY()
public:
	virtual EGeoFlowNodeType NodeType() const override { return EGeoFlowNodeType::VectorMultiply; }
	virtual FText GetNodeTitle(ENodeTitleType::Type titleType) const override { return FText::FromString("Vector Multiply"); }
	virtual UGFN_R_Base* CreateRuntimeNode(UGeoFlowRuntimeGraph* runtimeGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap < FGuid, UGeoFlowRuntimePin*>& idToPinMap) override;
};
UCLASS()
class GEOFLOWCORE_API UGFN_R_VectorMultiply : public UGFN_R_VectorMathBase {
	GENERATED_BODY()
public:
	virtual EGeoFlowNodeType NodeType() const override { return EGeoFlowNodeType::VectorMultiply; }

	virtual UGFN_E_Base* CreateEditorNode(UEdGraph* _workingGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap < FGuid, UEdGraphPin*>& idToPinMap) override;
	virtual FVector3f Evaluate(const FVector3f& pos) override;
};

UCLASS()
class GEOFLOWCORE_API UGFN_E_VectorSubtract : public UGFN_E_VectorMathBase {
	GENERATED_BODY()
public:
	virtual EGeoFlowNodeType NodeType() const override { return EGeoFlowNodeType::VectorSubtract; }
	virtual FText GetNodeTitle(ENodeTitleType::Type titleType) const override { return FText::FromString("Vector Subtract"); }
	virtual UGFN_R_Base* CreateRuntimeNode(UGeoFlowRuntimeGraph* runtimeGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap < FGuid, UGeoFlowRuntimePin*>& idToPinMap) override;
};
UCLASS()
class GEOFLOWCORE_API UGFN_R_VectorSubtract : public UGFN_R_VectorMathBase {
	GENERATED_BODY()
public:
	virtual EGeoFlowNodeType NodeType() const override { return EGeoFlowNodeType::VectorSubtract; }

	virtual UGFN_E_Base* CreateEditorNode(UEdGraph* _workingGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap < FGuid, UEdGraphPin*>& idToPinMap) override;
	virtual FVector3f Evaluate(const FVector3f& pos) override;
};

UCLASS()
class GEOFLOWCORE_API UGFN_E_VectorDivide : public UGFN_E_VectorMathBase {
	GENERATED_BODY()
public:
	virtual EGeoFlowNodeType NodeType() const override { return EGeoFlowNodeType::VectorDivide; }
	virtual FText GetNodeTitle(ENodeTitleType::Type titleType) const override { return FText::FromString("Vector Divide"); }
	virtual UGFN_R_Base* CreateRuntimeNode(UGeoFlowRuntimeGraph* runtimeGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap < FGuid, UGeoFlowRuntimePin*>& idToPinMap) override;
};
UCLASS()
class GEOFLOWCORE_API UGFN_R_VectorDivide : public UGFN_R_VectorMathBase {
	GENERATED_BODY()
public:
	virtual EGeoFlowNodeType NodeType() const override { return EGeoFlowNodeType::VectorDivide; }

	virtual UGFN_E_Base* CreateEditorNode(UEdGraph* _workingGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap < FGuid, UEdGraphPin*>& idToPinMap) override;
	virtual FVector3f Evaluate(const FVector3f& pos) override;
};