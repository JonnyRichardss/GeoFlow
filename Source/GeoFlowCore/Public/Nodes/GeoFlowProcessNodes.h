#pragma once
#include "EdGraph/EdGraphPin.h"
#include "GeoFlowNodeTypes.h"
#include "GeoFlowPinDefaultValueOps.h"
#include "GeoFlowProcessNodes.generated.h"

UCLASS()
class GEOFLOWCORE_API UGFN_E_Smin : public UGFN_E_BaseFloat {
	GENERATED_BODY()
public:
	virtual EGeoFlowNodeType NodeType() const override { return EGeoFlowNodeType::Smin; }
	virtual FText GetNodeTitle(ENodeTitleType::Type titleType) const override { return FText::FromString("Smoothed Union"); }

	virtual UGFN_R_Base* CreateRuntimeNode(UGeoFlowRuntimeGraph* runtimeGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap < FGuid, UGeoFlowRuntimePin*>& idToPinMap) override;
	virtual TArray<UEdGraphPin*> CreateInputPins(UEdGraphPin* fromPin) override;
	virtual FLinearColor GetNodeTitleColor() const override { return FLinearColor::White; }
	virtual void PinDefaultValueChanged(UEdGraphPin* Pin) override {
		if (Pin == InputSmoothing) {
			smoothing = GetFloatDefaultValue(Pin);
		}
		Super::PinDefaultValueChanged(Pin);
	}
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override {
		if (PropertyChangedEvent.GetMemberPropertyName() == GET_MEMBER_NAME_CHECKED(UGFN_E_Smin, smoothing)) {
			SetFloatDefaultValue(InputSmoothing, smoothing);
		}
		Super::PostEditChangeProperty(PropertyChangedEvent);
	}
	UEdGraphPin* InputA = nullptr;
	UEdGraphPin* InputB = nullptr;
	UEdGraphPin* InputSmoothing = nullptr;
	UPROPERTY(EditAnywhere,Category="Node Properties")
	float smoothing;
};
UCLASS()
class GEOFLOWCORE_API UGFN_R_Smin : public UGFN_R_BaseFloat {
	GENERATED_BODY()
public:
	virtual EGeoFlowNodeType NodeType() const override { return EGeoFlowNodeType::Smin; }

	virtual UGFN_E_Base* CreateEditorNode(UEdGraph* _workingGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap < FGuid, UEdGraphPin*>& idToPinMap) override;
	
	UPROPERTY()
	UGeoFlowRuntimePin* InputA = nullptr;
	UPROPERTY()
	UGeoFlowRuntimePin* InputB = nullptr;
	UPROPERTY()
	UGeoFlowRuntimePin* InputSmoothing = nullptr;
	UPROPERTY()
	float a;
	UPROPERTY()
	float b;
	UPROPERTY()
	float smoothing;
	virtual float Evaluate(const FVector3f& pos) override;
};


UCLASS()
class GEOFLOWCORE_API UGFN_E_Smax : public UGFN_E_BaseFloat {
	GENERATED_BODY()
public:
	virtual EGeoFlowNodeType NodeType() const override { return EGeoFlowNodeType::Smax; }
	virtual FText GetNodeTitle(ENodeTitleType::Type titleType) const override { return FText::FromString("Smoothed Subtraction"); }

	virtual UGFN_R_Base* CreateRuntimeNode(UGeoFlowRuntimeGraph* runtimeGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap < FGuid, UGeoFlowRuntimePin*>& idToPinMap) override;
	virtual TArray<UEdGraphPin*> CreateInputPins(UEdGraphPin* fromPin) override;
	virtual FLinearColor GetNodeTitleColor() const override { return FLinearColor::Red; }
	virtual void PinDefaultValueChanged(UEdGraphPin* Pin) override {
		if (Pin == InputSmoothing) {
			smoothing = GetFloatDefaultValue(Pin);
		}
		Super::PinDefaultValueChanged(Pin);
	}
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override {
		if (PropertyChangedEvent.GetMemberPropertyName() == GET_MEMBER_NAME_CHECKED(UGFN_E_Smax, smoothing)) {
			SetFloatDefaultValue(InputSmoothing, smoothing);
		}
		Super::PostEditChangeProperty(PropertyChangedEvent);
	}
	UEdGraphPin* InputA = nullptr;
	UEdGraphPin* InputB = nullptr;
	UEdGraphPin* InputSmoothing = nullptr;
	UPROPERTY(EditAnywhere, Category = "Node Properties")
	float smoothing;
};
UCLASS()
class GEOFLOWCORE_API UGFN_R_Smax : public UGFN_R_BaseFloat {
	GENERATED_BODY()
public:
	virtual EGeoFlowNodeType NodeType() const override { return EGeoFlowNodeType::Smax; }

	virtual UGFN_E_Base* CreateEditorNode(UEdGraph* _workingGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap < FGuid, UEdGraphPin*>& idToPinMap) override;

	UPROPERTY()
	UGeoFlowRuntimePin* InputA = nullptr;
	UPROPERTY()
	UGeoFlowRuntimePin* InputB = nullptr;
	UPROPERTY()
	UGeoFlowRuntimePin* InputSmoothing = nullptr;
	UPROPERTY()
	float a;
	UPROPERTY()
	float b;
	UPROPERTY()
	float smoothing;
	virtual float Evaluate(const FVector3f& pos) override;
};



UCLASS()
class GEOFLOWCORE_API UGFN_E_Min : public UGFN_E_BaseFloat {
	GENERATED_BODY()
public:
	virtual EGeoFlowNodeType NodeType() const override { return EGeoFlowNodeType::Min; }
	virtual FText GetNodeTitle(ENodeTitleType::Type titleType) const override { return FText::FromString("Union"); }

	virtual UGFN_R_Base* CreateRuntimeNode(UGeoFlowRuntimeGraph* runtimeGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap < FGuid, UGeoFlowRuntimePin*>& idToPinMap) override;
	virtual TArray<UEdGraphPin*> CreateInputPins(UEdGraphPin* fromPin) override;
	virtual FLinearColor GetNodeTitleColor() const override { return FLinearColor::White; }

	UEdGraphPin* InputA = nullptr;
	UEdGraphPin* InputB = nullptr;

};
UCLASS()
class GEOFLOWCORE_API UGFN_R_Min : public UGFN_R_BaseFloat {
	GENERATED_BODY()
public:
	virtual EGeoFlowNodeType NodeType() const override { return EGeoFlowNodeType::Min; }

	virtual UGFN_E_Base* CreateEditorNode(UEdGraph* _workingGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap < FGuid, UEdGraphPin*>& idToPinMap) override;

	UPROPERTY()
	UGeoFlowRuntimePin* InputA = nullptr;
	UPROPERTY()
	UGeoFlowRuntimePin* InputB = nullptr;
	
	UPROPERTY()
	float a;
	UPROPERTY()
	float b;
	virtual float Evaluate(const FVector3f& pos) override;
};



UCLASS()
class GEOFLOWCORE_API UGFN_E_Max : public UGFN_E_BaseFloat {
	GENERATED_BODY()
public:
	virtual EGeoFlowNodeType NodeType() const override { return EGeoFlowNodeType::Max; }
	virtual FText GetNodeTitle(ENodeTitleType::Type titleType) const override { return FText::FromString("Subtraction"); }

	virtual UGFN_R_Base* CreateRuntimeNode(UGeoFlowRuntimeGraph* runtimeGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap < FGuid, UGeoFlowRuntimePin*>& idToPinMap) override;
	virtual TArray<UEdGraphPin*> CreateInputPins(UEdGraphPin* fromPin) override;
	virtual FLinearColor GetNodeTitleColor() const override { return FLinearColor::Red; }

	UEdGraphPin* InputA = nullptr;
	UEdGraphPin* InputB = nullptr;

};
UCLASS()
class GEOFLOWCORE_API UGFN_R_Max : public UGFN_R_BaseFloat {
	GENERATED_BODY()
public:
	virtual EGeoFlowNodeType NodeType() const override { return EGeoFlowNodeType::Max; }

	virtual UGFN_E_Base* CreateEditorNode(UEdGraph* _workingGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap < FGuid, UEdGraphPin*>& idToPinMap) override;

	UPROPERTY()
	UGeoFlowRuntimePin* InputA = nullptr;
	UPROPERTY()
	UGeoFlowRuntimePin* InputB = nullptr;

	UPROPERTY()
	float a;
	UPROPERTY()
	float b;
	virtual float Evaluate(const FVector3f& pos) override;
};
