#pragma once
#include "GeoFlowPinDefaultValueOps.h"
#include "EdGraph/EdGraphPin.h"
#include "GeoFlowNodeTypes.h"
#include "GeoFlowArrayPrimitiveNodes.generated.h"

UCLASS()
class GEOFLOWCORE_API UGFN_E_BaseArrayPrimitive : public UGFN_E_BaseFloat {
	GENERATED_BODY()
public:
	virtual EGeoFlowNodeType NodeType() const override { return EGeoFlowNodeType::BaseArrayPrimitive; }
	virtual FText GetNodeTitle(ENodeTitleType::Type titleType) const override { return FText::FromString("Default Primitive Array node"); }
	virtual FLinearColor GetNodeTitleColor() const override { return FLinearColor::FromSRGBColor(FColor::Blue); }

	UEdGraphPin* TransformsInput = nullptr;
	UPROPERTY(EditAnywhere, Category = "Node Properties")
	TArray<FGeoFlowTransform> transforms;
};
UCLASS()
class GEOFLOWCORE_API UGFN_R_BaseArrayPrimitive : public UGFN_R_BaseFloat {
	GENERATED_BODY()
public:
	virtual EGeoFlowNodeType NodeType() const override { return EGeoFlowNodeType::BaseArrayPrimitive; }

	UPROPERTY()
	UGeoFlowRuntimePin* TransformsInput = nullptr;
	UPROPERTY()
	TArray<FGeoFlowTransform> transforms;
	TArray<FVector3f> GetPositions();
};
UCLASS()
class GEOFLOWCORE_API UGFN_E_ArrayPrimitiveSphere : public UGFN_E_BaseArrayPrimitive {
	GENERATED_BODY()
public:
	virtual EGeoFlowNodeType NodeType() const override { return EGeoFlowNodeType::ArrayPrimitiveSphere; }
	virtual FText GetNodeTitle(ENodeTitleType::Type titleType) const override { return FText::FromString("Sphere Primitive Array"); }

	virtual UGFN_R_Base* CreateRuntimeNode(UGeoFlowRuntimeGraph* runtimeGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap < FGuid, UGeoFlowRuntimePin*>& idToPinMap) override;
	virtual TArray<UEdGraphPin*> CreateInputPins(UEdGraphPin* fromPin) override;
	virtual void PinDefaultValueChanged(UEdGraphPin* Pin) override {
		Super::PinDefaultValueChanged(Pin);
		if (Pin == RadiusInput) {
			radius = GetFloatDefaultValue(Pin);
		}
	}
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override {
		Super::PostEditChangeProperty(PropertyChangedEvent);
		if (PropertyChangedEvent.GetMemberPropertyName() == GET_MEMBER_NAME_CHECKED(UGFN_E_ArrayPrimitiveSphere, radius)) {
			SetFloatDefaultValue(RadiusInput, radius);
		}
	}

	UEdGraphPin* RadiusInput = nullptr;
	UPROPERTY(EditAnywhere, Category = "Node Properties")
	float radius;
};
UCLASS()
class GEOFLOWCORE_API UGFN_R_ArrayPrimitiveSphere : public UGFN_R_BaseArrayPrimitive {
	GENERATED_BODY()
public:
	virtual EGeoFlowNodeType NodeType() const override { return EGeoFlowNodeType::ArrayPrimitiveSphere; }

	virtual UGFN_E_Base* CreateEditorNode(UEdGraph* _workingGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap < FGuid, UEdGraphPin*>& idToPinMap) override;

	UPROPERTY()
	UGeoFlowRuntimePin* RadiusInput = nullptr;

	UPROPERTY()
	float radius;
	virtual float Evaluate(const FVector3f& pos) override;
};

UCLASS()
class GEOFLOWCORE_API UGFN_E_ArrayPrimitiveCube : public UGFN_E_BaseArrayPrimitive {
	GENERATED_BODY()
public:
	virtual EGeoFlowNodeType NodeType() const override { return EGeoFlowNodeType::ArrayPrimitiveCube; }
	virtual FText GetNodeTitle(ENodeTitleType::Type titleType) const override { return FText::FromString("Cube Primitive Array"); }

	virtual UGFN_R_Base* CreateRuntimeNode(UGeoFlowRuntimeGraph* runtimeGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap < FGuid, UGeoFlowRuntimePin*>& idToPinMap) override;
	virtual TArray<UEdGraphPin*> CreateInputPins(UEdGraphPin* fromPin) override;
	virtual void PinDefaultValueChanged(UEdGraphPin* Pin) override {
		Super::PinDefaultValueChanged(Pin);
		if (Pin == RadiusInput) {
			radius = GetFloatDefaultValue(Pin);
		}
	}
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override {
		Super::PostEditChangeProperty(PropertyChangedEvent);
		if (PropertyChangedEvent.GetMemberPropertyName() == GET_MEMBER_NAME_CHECKED(UGFN_E_ArrayPrimitiveCube, radius)) {
			SetFloatDefaultValue(RadiusInput, radius);
		}
	}

	UEdGraphPin* RadiusInput = nullptr;
	UPROPERTY(EditAnywhere, Category = "Node Properties")
	float radius;
};
UCLASS()
class GEOFLOWCORE_API UGFN_R_ArrayPrimitiveCube : public UGFN_R_BaseArrayPrimitive {
	GENERATED_BODY()
public:
	virtual EGeoFlowNodeType NodeType() const override { return EGeoFlowNodeType::ArrayPrimitiveCube; }

	virtual UGFN_E_Base* CreateEditorNode(UEdGraph* _workingGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap < FGuid, UEdGraphPin*>& idToPinMap) override;

	UPROPERTY()
	UGeoFlowRuntimePin* RadiusInput = nullptr;
	UPROPERTY()
	float radius;
	virtual float Evaluate(const FVector3f& pos) override;
};


UCLASS()
class GEOFLOWCORE_API UGFN_E_ArrayPrimitiveBox : public UGFN_E_BaseArrayPrimitive {
	GENERATED_BODY()
public:
	virtual EGeoFlowNodeType NodeType() const override { return EGeoFlowNodeType::ArrayPrimitiveBox; }
	virtual FText GetNodeTitle(ENodeTitleType::Type titleType) const override { return FText::FromString("Box Primitive Array"); }

	virtual UGFN_R_Base* CreateRuntimeNode(UGeoFlowRuntimeGraph* runtimeGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap < FGuid, UGeoFlowRuntimePin*>& idToPinMap) override;
	virtual TArray<UEdGraphPin*> CreateInputPins(UEdGraphPin* fromPin) override;
	virtual void PinDefaultValueChanged(UEdGraphPin* Pin) override {
		Super::PinDefaultValueChanged(Pin);
		if (Pin == RadiusInput) {
			radius = GetVectorDefaultValue(Pin);
		}
	}
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override {
		Super::PostEditChangeProperty(PropertyChangedEvent);
		if (PropertyChangedEvent.GetMemberPropertyName() == GET_MEMBER_NAME_CHECKED(UGFN_E_ArrayPrimitiveBox, radius)) {
			SetVectorDefaultValue(RadiusInput, radius);
		}
	}

	UEdGraphPin* RadiusInput = nullptr;
	UPROPERTY(EditAnywhere, Category = "Node Properties", DisplayName = "Extents")
	FVector3f radius;
};
UCLASS()
class GEOFLOWCORE_API UGFN_R_ArrayPrimitiveBox : public UGFN_R_BaseArrayPrimitive {
	GENERATED_BODY()
public:
	virtual EGeoFlowNodeType NodeType() const override { return EGeoFlowNodeType::ArrayPrimitiveBox; }

	virtual UGFN_E_Base* CreateEditorNode(UEdGraph* _workingGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap < FGuid, UEdGraphPin*>& idToPinMap) override;

	UPROPERTY()
	UGeoFlowRuntimePin* RadiusInput = nullptr;
	UPROPERTY()
	FVector3f radius;
	virtual float Evaluate(const FVector3f& pos) override;
};


UCLASS()
class GEOFLOWCORE_API UGFN_E_ArrayPrimitiveEllipsoid : public UGFN_E_BaseArrayPrimitive {
	GENERATED_BODY()
public:
	virtual EGeoFlowNodeType NodeType() const override { return EGeoFlowNodeType::ArrayPrimitiveEllipsoid; }
	virtual FText GetNodeTitle(ENodeTitleType::Type titleType) const override { return FText::FromString("Ellipsoid Primitive Array"); }

	virtual UGFN_R_Base* CreateRuntimeNode(UGeoFlowRuntimeGraph* runtimeGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap < FGuid, UGeoFlowRuntimePin*>& idToPinMap) override;
	virtual TArray<UEdGraphPin*> CreateInputPins(UEdGraphPin* fromPin) override;
	virtual void PinDefaultValueChanged(UEdGraphPin* Pin) override {
		Super::PinDefaultValueChanged(Pin);
		if (Pin == RadiusInput) {
			radius = GetVectorDefaultValue(Pin);
		}
	}
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override {
		Super::PostEditChangeProperty(PropertyChangedEvent);
		if (PropertyChangedEvent.GetMemberPropertyName() == GET_MEMBER_NAME_CHECKED(UGFN_E_ArrayPrimitiveEllipsoid, radius)) {
			SetVectorDefaultValue(RadiusInput, radius);
		}
	}
	UEdGraphPin* RadiusInput = nullptr;
	UPROPERTY(EditAnywhere, Category = "Node Properties", DisplayName = "Extents")
	FVector3f radius;
};
UCLASS()
class GEOFLOWCORE_API UGFN_R_ArrayPrimitiveEllipsoid : public UGFN_R_BaseArrayPrimitive {
	GENERATED_BODY()
public:
	virtual EGeoFlowNodeType NodeType() const override { return EGeoFlowNodeType::ArrayPrimitiveEllipsoid; }

	virtual UGFN_E_Base* CreateEditorNode(UEdGraph* _workingGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap < FGuid, UEdGraphPin*>& idToPinMap) override;

	UPROPERTY()
	UGeoFlowRuntimePin* RadiusInput = nullptr;
	UPROPERTY()
	FVector3f radius;
	virtual float Evaluate(const FVector3f& pos) override;
};

UCLASS()
class GEOFLOWCORE_API UGFN_E_ArrayPrimitiveCone : public UGFN_E_BaseArrayPrimitive {
	GENERATED_BODY()
public:
	virtual EGeoFlowNodeType NodeType() const override { return EGeoFlowNodeType::ArrayPrimitiveCone; }
	virtual FText GetNodeTitle(ENodeTitleType::Type titleType) const override { return FText::FromString("Cone Primitive Array"); }

	virtual UGFN_R_Base* CreateRuntimeNode(UGeoFlowRuntimeGraph* runtimeGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap < FGuid, UGeoFlowRuntimePin*>& idToPinMap) override;
	virtual TArray<UEdGraphPin*> CreateInputPins(UEdGraphPin* fromPin) override;

	virtual void PinDefaultValueChanged(UEdGraphPin* Pin) override {
		Super::PinDefaultValueChanged(Pin);
		if (Pin == AngleInput) {
			angle = GetFloatDefaultValue(Pin);
		}
		else if (Pin == HeightInput) {
			height = GetFloatDefaultValue(Pin);
		}
	}
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override {
		Super::PostEditChangeProperty(PropertyChangedEvent);
		if (PropertyChangedEvent.GetMemberPropertyName() == GET_MEMBER_NAME_CHECKED(UGFN_E_ArrayPrimitiveCone, angle)) {
			SetFloatDefaultValue(AngleInput, angle);
		}
		else if (PropertyChangedEvent.GetMemberPropertyName() == GET_MEMBER_NAME_CHECKED(UGFN_E_ArrayPrimitiveCone, height)) {
			SetFloatDefaultValue(HeightInput, height);
		}
	}
	UEdGraphPin* AngleInput = nullptr;
	UEdGraphPin* HeightInput = nullptr;
	UPROPERTY(EditAnywhere, Category = "Node Properties")
	float angle;
	UPROPERTY(EditAnywhere, Category = "Node Properties")
	float height;
};
UCLASS()
class GEOFLOWCORE_API UGFN_R_ArrayPrimitiveCone : public UGFN_R_BaseArrayPrimitive {
	GENERATED_BODY()
public:
	virtual EGeoFlowNodeType NodeType() const override { return EGeoFlowNodeType::ArrayPrimitiveCone; }

	virtual UGFN_E_Base* CreateEditorNode(UEdGraph* _workingGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap < FGuid, UEdGraphPin*>& idToPinMap) override;

	UPROPERTY()
	UGeoFlowRuntimePin* AngleInput = nullptr;
	UPROPERTY()
	UGeoFlowRuntimePin* HeightInput = nullptr;

	UPROPERTY()
	float angle;
	UPROPERTY()
	float height;
	virtual float Evaluate(const FVector3f& pos) override;
};

UCLASS()
class GEOFLOWCORE_API UGFN_E_ArrayPrimitiveCylinder : public UGFN_E_BaseArrayPrimitive {
	GENERATED_BODY()
public:
	virtual EGeoFlowNodeType NodeType() const override { return EGeoFlowNodeType::ArrayPrimitiveCylinder; }
	virtual FText GetNodeTitle(ENodeTitleType::Type titleType) const override { return FText::FromString("Cylinder Primitive Array"); }

	virtual UGFN_R_Base* CreateRuntimeNode(UGeoFlowRuntimeGraph* runtimeGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap < FGuid, UGeoFlowRuntimePin*>& idToPinMap) override;
	virtual TArray<UEdGraphPin*> CreateInputPins(UEdGraphPin* fromPin) override;
	virtual void PinDefaultValueChanged(UEdGraphPin* Pin) override {
		Super::PinDefaultValueChanged(Pin);
		if (Pin == RadiusInput) {
			radius = GetFloatDefaultValue(Pin);
		}
		else if (Pin == HeightInput) {
			height = GetFloatDefaultValue(Pin);
		}
	}
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override {
		Super::PostEditChangeProperty(PropertyChangedEvent);
		if (PropertyChangedEvent.GetMemberPropertyName() == GET_MEMBER_NAME_CHECKED(UGFN_E_ArrayPrimitiveCylinder, radius)) {
			SetFloatDefaultValue(RadiusInput, radius);
		}
		else if (PropertyChangedEvent.GetMemberPropertyName() == GET_MEMBER_NAME_CHECKED(UGFN_E_ArrayPrimitiveCylinder, height)) {
			SetFloatDefaultValue(HeightInput, height);
		}
	}

	UEdGraphPin* RadiusInput = nullptr;
	UEdGraphPin* HeightInput = nullptr;
	UPROPERTY(EditAnywhere, Category = "Node Properties")
	float radius;
	UPROPERTY(EditAnywhere, Category = "Node Properties")
	float height;
};
UCLASS()
class GEOFLOWCORE_API UGFN_R_ArrayPrimitiveCylinder : public UGFN_R_BaseArrayPrimitive {
	GENERATED_BODY()
public:
	virtual EGeoFlowNodeType NodeType() const override { return EGeoFlowNodeType::ArrayPrimitiveCylinder; }

	virtual UGFN_E_Base* CreateEditorNode(UEdGraph* _workingGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap < FGuid, UEdGraphPin*>& idToPinMap) override;

	UPROPERTY()
	UGeoFlowRuntimePin* RadiusInput = nullptr;
	UPROPERTY()
	UGeoFlowRuntimePin* HeightInput = nullptr;

	UPROPERTY()
	float radius;
	UPROPERTY()
	float height;
	virtual float Evaluate(const FVector3f& pos) override;
};