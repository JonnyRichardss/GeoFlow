#pragma once
#include "EdGraph/EdGraphPin.h"
#include "GeoFlowNodeTypes.h"
#include "GeoFlowPinDefaultValueOps.h"
#include "GeoFlowPlacementNodes.generated.h"

UCLASS()
class GEOFLOWCORE_API UGFN_E_Mirror : public UGFN_E_BaseArray {
	GENERATED_BODY()
public:
	virtual EGeoFlowNodeType NodeType() const override { return EGeoFlowNodeType::Mirror; }
	virtual FText GetNodeTitle(ENodeTitleType::Type titleType) const override { return FText::FromString("Mirror"); }

	virtual UGFN_R_Base* CreateRuntimeNode(UGeoFlowRuntimeGraph* runtimeGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap < FGuid, UGeoFlowRuntimePin*>& idToPinMap) override;
	virtual TArray<UEdGraphPin*> CreateInputPins(UEdGraphPin* fromPin) override;
	virtual FLinearColor GetNodeTitleColor() const override { return FLinearColor::FromSRGBColor(FColor::FromHex(TEXT("#0F52BA"))); }
	virtual void PinDefaultValueChanged(UEdGraphPin* Pin) override {
		if (Pin == PositionInput) {
			position = GetVectorDefaultValue(Pin);
		}
		if (Pin == RotationInput) {
			rotation = GetVectorDefaultValue(Pin);
		}
		if (Pin == XPin) {
			mirrorX = GetBoolDefaultValue(Pin);
		}
		if (Pin == YPin) {
			mirrorY = GetBoolDefaultValue(Pin);
		}
		if (Pin == ZPin) {
			mirrorZ = GetBoolDefaultValue(Pin);
		}
		Super::PinDefaultValueChanged(Pin);
	}
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override {
		if (PropertyChangedEvent.GetMemberPropertyName() == GET_MEMBER_NAME_CHECKED(UGFN_E_Mirror, position)) {
			SetVectorDefaultValue(PositionInput, position);
		}
		if (PropertyChangedEvent.GetMemberPropertyName() == GET_MEMBER_NAME_CHECKED(UGFN_E_Mirror, rotation)) {
			SetVectorDefaultValue(RotationInput, rotation);
		}
		if (PropertyChangedEvent.GetMemberPropertyName() == GET_MEMBER_NAME_CHECKED(UGFN_E_Mirror, mirrorX)) {
			SetBoolDefaultValue(XPin, mirrorX);
		}
		if (PropertyChangedEvent.GetMemberPropertyName() == GET_MEMBER_NAME_CHECKED(UGFN_E_Mirror, mirrorY)) {
			SetBoolDefaultValue(YPin, mirrorY);
		}
		if (PropertyChangedEvent.GetMemberPropertyName() == GET_MEMBER_NAME_CHECKED(UGFN_E_Mirror, mirrorZ)) {
			SetBoolDefaultValue(ZPin, mirrorZ);
		}
		Super::PostEditChangeProperty(PropertyChangedEvent);
	}
	UEdGraphPin* PositionInput = nullptr;
	UEdGraphPin* RotationInput = nullptr;
	UEdGraphPin* XPin = nullptr;
	UEdGraphPin* YPin = nullptr;
	UEdGraphPin* ZPin = nullptr;
	UPROPERTY(EditAnywhere, Category = "Node Properties")
	FVector3f position;
	UPROPERTY(EditAnywhere, Category = "Node Properties")
	FVector3f rotation;
	UPROPERTY(EditAnywhere, Category = "Node Properties")
	bool mirrorX;
	UPROPERTY(EditAnywhere, Category = "Node Properties")
	bool mirrorY;
	UPROPERTY(EditAnywhere, Category = "Node Properties")
	bool mirrorZ;

};
UCLASS()
class GEOFLOWCORE_API UGFN_R_Mirror : public UGFN_R_BaseArray {
	GENERATED_BODY()
public:
	virtual EGeoFlowNodeType NodeType() const override { return EGeoFlowNodeType::Mirror; }

	virtual UGFN_E_Base* CreateEditorNode(UEdGraph* _workingGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap < FGuid, UEdGraphPin*>& idToPinMap) override;

	UPROPERTY()
	UGeoFlowRuntimePin* PositionInput = nullptr;
	UPROPERTY()
	UGeoFlowRuntimePin* RotationInput = nullptr;

	UPROPERTY()
	FVector3f position;
	UPROPERTY()
	FVector3f rotation;
	UPROPERTY()
	bool mirrorX;
	UPROPERTY()
	bool mirrorY;
	UPROPERTY()
	bool mirrorZ;
	virtual TArray<FGeoFlowTransform> Evaluate(const FVector3f& pos) override;
};
UCLASS()
class GEOFLOWCORE_API UGFN_E_Grid : public UGFN_E_BaseArray {
	GENERATED_BODY()
public:
	virtual EGeoFlowNodeType NodeType() const override { return EGeoFlowNodeType::Grid; }
	virtual FText GetNodeTitle(ENodeTitleType::Type titleType) const override { return FText::FromString("Grid"); }

	virtual UGFN_R_Base* CreateRuntimeNode(UGeoFlowRuntimeGraph* runtimeGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap < FGuid, UGeoFlowRuntimePin*>& idToPinMap) override;
	virtual TArray<UEdGraphPin*> CreateInputPins(UEdGraphPin* fromPin) override;
	virtual FLinearColor GetNodeTitleColor() const override { return FLinearColor::FromSRGBColor(FColor::FromHex(TEXT("#0F52BA"))); }
	virtual void PinDefaultValueChanged(UEdGraphPin* Pin) override {
		if (Pin == PositionInput) {
			position = GetVectorDefaultValue(Pin);
		}
		if (Pin == RotationInput) {
			rotation = GetVectorDefaultValue(Pin);
		}
		if (Pin == CopiesPin) {
			copies = GetIntVectorDefaultValue(Pin);
		}
		if (Pin == SpacingPin) {
			spacing = GetVectorDefaultValue(Pin);
		}
		Super::PinDefaultValueChanged(Pin);
	}
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override {
		if (PropertyChangedEvent.GetMemberPropertyName() == GET_MEMBER_NAME_CHECKED(UGFN_E_Grid, position)) {
			SetVectorDefaultValue(PositionInput, position);
		}
		if (PropertyChangedEvent.GetMemberPropertyName() == GET_MEMBER_NAME_CHECKED(UGFN_E_Grid, rotation)) {
			SetVectorDefaultValue(RotationInput, rotation);
		}
		if (PropertyChangedEvent.GetMemberPropertyName() == GET_MEMBER_NAME_CHECKED(UGFN_E_Grid, copies)) {
			SetIntVectorDefaultValue(CopiesPin, copies);
		}
		if (PropertyChangedEvent.GetMemberPropertyName() == GET_MEMBER_NAME_CHECKED(UGFN_E_Grid, spacing)) {
			SetVectorDefaultValue(SpacingPin, spacing);
		}

		Super::PostEditChangeProperty(PropertyChangedEvent);
	}
	UEdGraphPin* PositionInput = nullptr;
	UEdGraphPin* RotationInput = nullptr;
	UEdGraphPin* CopiesPin = nullptr;
	UEdGraphPin* SpacingPin = nullptr;
	UPROPERTY(EditAnywhere, Category = "Node Properties")
	FVector3f position;
	UPROPERTY(EditAnywhere, Category = "Node Properties")
	FVector3f rotation;
	UPROPERTY(EditAnywhere, Category = "Node Properties")
	FGeoFlowIntVector copies = { 1,1,1 };
	UPROPERTY(EditAnywhere, Category = "Node Properties")
	FVector3f spacing;

};
UCLASS()
class GEOFLOWCORE_API UGFN_R_Grid : public UGFN_R_BaseArray {
	GENERATED_BODY()
public:
	virtual EGeoFlowNodeType NodeType() const override { return EGeoFlowNodeType::Grid; }

	virtual UGFN_E_Base* CreateEditorNode(UEdGraph* _workingGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap < FGuid, UEdGraphPin*>& idToPinMap) override;

	UPROPERTY()
	UGeoFlowRuntimePin* PositionInput = nullptr;
	UPROPERTY()
	UGeoFlowRuntimePin* RotationInput = nullptr;
	UPROPERTY()
	FVector3f position;
	UPROPERTY()
	FVector3f rotation;
	UPROPERTY()
	FGeoFlowIntVector copies = { 1,1,1 };
	UPROPERTY()
	FVector3f spacing;
	virtual TArray<FGeoFlowTransform> Evaluate(const FVector3f& pos) override;
};