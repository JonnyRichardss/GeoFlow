#pragma once
#include "GeoFlowPinDefaultValueOps.h"
#include "EdGraph/EdGraphPin.h"
#include "GeoFlowNodeTypes.h"
#include "GeoFlowPrimitiveNodes.generated.h"

UCLASS(ShowCategories = ("Viewport"))
class GEOFLOWCORE_API UGFN_E_BasePrimitive : public UGFN_E_BaseFloat {
	GENERATED_BODY()
public:
	virtual EGeoFlowNodeType NodeType() const override { return EGeoFlowNodeType::BasePrimitive; }
	virtual FText GetNodeTitle(ENodeTitleType::Type titleType) const override { return FText::FromString("Default Primitive node"); }
	virtual FLinearColor GetNodeTitleColor() const override { return FLinearColor::FromSRGBColor(FColor::Cyan); }
	virtual void PinDefaultValueChanged(UEdGraphPin* Pin) override {
		if (Pin == PositionInput) {
			position = GetVectorDefaultValue(Pin);
		}
		else if (Pin == RotationInput) {
			rotation = GetVectorDefaultValue(Pin);
		}
		Super::PinDefaultValueChanged(Pin);
	}
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override {
		if (PropertyChangedEvent.GetMemberPropertyName() == GET_MEMBER_NAME_CHECKED(UGFN_E_BasePrimitive, position)) {
			SetVectorDefaultValue(PositionInput, position);
		}
		else if (PropertyChangedEvent.GetMemberPropertyName() == GET_MEMBER_NAME_CHECKED(UGFN_E_BasePrimitive, rotation)) {
			SetVectorDefaultValue(RotationInput, rotation);
		}
		Super::PostEditChangeProperty(PropertyChangedEvent);
	}
	UEdGraphPin* PositionInput = nullptr;
	UEdGraphPin* RotationInput = nullptr;
	UPROPERTY(EditAnywhere,Category="Node Properties")
	FVector3f position;
	UPROPERTY(EditAnywhere,Category="Node Properties")
	FVector3f rotation;
};
UCLASS()
class GEOFLOWCORE_API UGFN_R_BasePrimitive : public UGFN_R_BaseFloat {
	GENERATED_BODY()
public:
	virtual EGeoFlowNodeType NodeType() const override { return EGeoFlowNodeType::BasePrimitive; }

	UPROPERTY()
	UGeoFlowRuntimePin* PositionInput = nullptr;
	UPROPERTY()
	UGeoFlowRuntimePin* RotationInput = nullptr;
	UPROPERTY()
	FVector3f position;
	UPROPERTY()
	FVector3f rotation;

	FVector3f GetPosition();
};
UCLASS()
class GEOFLOWCORE_API UGFN_E_PrimitiveSphere : public UGFN_E_BasePrimitive {
	GENERATED_BODY()
public:
	virtual EGeoFlowNodeType NodeType() const override { return EGeoFlowNodeType::PrimitiveSphere; }
	virtual FText GetNodeTitle(ENodeTitleType::Type titleType) const override { return FText::FromString("Sphere Primitive"); }

	virtual UGFN_R_Base* CreateRuntimeNode(UGeoFlowRuntimeGraph* runtimeGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap < FGuid, UGeoFlowRuntimePin*>& idToPinMap) override;
	virtual TArray<UEdGraphPin*> CreateInputPins(UEdGraphPin* fromPin) override;
	virtual void PinDefaultValueChanged(UEdGraphPin* Pin) override {
		if (Pin == RadiusInput) {
			radius = GetFloatDefaultValue(Pin);
		}
		Super::PinDefaultValueChanged(Pin);
	}
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override {
		if (PropertyChangedEvent.GetMemberPropertyName() == GET_MEMBER_NAME_CHECKED(UGFN_E_PrimitiveSphere, radius)) {
			SetFloatDefaultValue(RadiusInput, radius);
		}
		Super::PostEditChangeProperty(PropertyChangedEvent);
	}
	virtual void DrawInViewport(UWorld* InWorld) override;
	UEdGraphPin* RadiusInput = nullptr;
	UPROPERTY(EditAnywhere,Category="Node Properties")
	float radius;
};
UCLASS()
class GEOFLOWCORE_API UGFN_R_PrimitiveSphere : public UGFN_R_BasePrimitive {
	GENERATED_BODY()
public:
	virtual EGeoFlowNodeType NodeType() const override { return EGeoFlowNodeType::PrimitiveSphere; }

	virtual UGFN_E_Base* CreateEditorNode(UEdGraph* _workingGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap < FGuid, UEdGraphPin*>& idToPinMap) override;

	UPROPERTY()
	UGeoFlowRuntimePin* RadiusInput = nullptr;
	
	UPROPERTY()
	float radius;
	virtual float Evaluate(const FVector3f& pos) override;
};

UCLASS()
class GEOFLOWCORE_API UGFN_E_PrimitiveCube : public UGFN_E_BasePrimitive {
	GENERATED_BODY()
public:
	virtual EGeoFlowNodeType NodeType() const override { return EGeoFlowNodeType::PrimitiveCube; }
	virtual FText GetNodeTitle(ENodeTitleType::Type titleType) const override { return FText::FromString("Cube Primitive"); }

	virtual UGFN_R_Base* CreateRuntimeNode(UGeoFlowRuntimeGraph* runtimeGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap < FGuid, UGeoFlowRuntimePin*>& idToPinMap) override;
	virtual TArray<UEdGraphPin*> CreateInputPins(UEdGraphPin* fromPin) override;
	virtual void PinDefaultValueChanged(UEdGraphPin* Pin) override {
		if (Pin == RadiusInput) {
			radius = GetFloatDefaultValue(Pin);
		}
		Super::PinDefaultValueChanged(Pin);
	}
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override {
		if (PropertyChangedEvent.GetMemberPropertyName() == GET_MEMBER_NAME_CHECKED(UGFN_E_PrimitiveCube, radius)) {
			SetFloatDefaultValue(RadiusInput, radius);
		}
		Super::PostEditChangeProperty(PropertyChangedEvent);
	}
	virtual void DrawInViewport(UWorld* InWorld) override;
	UEdGraphPin* RadiusInput = nullptr;
	UPROPERTY(EditAnywhere,Category="Node Properties")
	float radius;
};
UCLASS()
class GEOFLOWCORE_API UGFN_R_PrimitiveCube : public UGFN_R_BasePrimitive {
	GENERATED_BODY()
public:
	virtual EGeoFlowNodeType NodeType() const override { return EGeoFlowNodeType::PrimitiveCube; }

	virtual UGFN_E_Base* CreateEditorNode(UEdGraph* _workingGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap < FGuid, UEdGraphPin*>& idToPinMap) override;

	UPROPERTY()
	UGeoFlowRuntimePin* RadiusInput = nullptr;
	UPROPERTY()
	float radius;
	virtual float Evaluate(const FVector3f& pos) override;
};


UCLASS()
class GEOFLOWCORE_API UGFN_E_PrimitiveBox : public UGFN_E_BasePrimitive {
	GENERATED_BODY()
public:
	virtual EGeoFlowNodeType NodeType() const override { return EGeoFlowNodeType::PrimitiveBox; }
	virtual FText GetNodeTitle(ENodeTitleType::Type titleType) const override { return FText::FromString("Box Primitive"); }

	virtual UGFN_R_Base* CreateRuntimeNode(UGeoFlowRuntimeGraph* runtimeGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap < FGuid, UGeoFlowRuntimePin*>& idToPinMap) override;
	virtual TArray<UEdGraphPin*> CreateInputPins(UEdGraphPin* fromPin) override;
	virtual void PinDefaultValueChanged(UEdGraphPin* Pin) override {
		if (Pin == RadiusInput) {
			radius = GetVectorDefaultValue(Pin);
		}
		Super::PinDefaultValueChanged(Pin);
	}
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override {
		if (PropertyChangedEvent.GetMemberPropertyName() == GET_MEMBER_NAME_CHECKED(UGFN_E_PrimitiveBox, radius)) {
			SetVectorDefaultValue(RadiusInput, radius);
		}
		Super::PostEditChangeProperty(PropertyChangedEvent);
	}
	virtual void DrawInViewport(UWorld* InWorld) override;
	UEdGraphPin* RadiusInput = nullptr;
	UPROPERTY(EditAnywhere,Category="Node Properties", DisplayName="Extents")
	FVector3f radius;
};
UCLASS()
class GEOFLOWCORE_API UGFN_R_PrimitiveBox : public UGFN_R_BasePrimitive {
	GENERATED_BODY()
public:
	virtual EGeoFlowNodeType NodeType() const override { return EGeoFlowNodeType::PrimitiveBox; }

	virtual UGFN_E_Base* CreateEditorNode(UEdGraph* _workingGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap < FGuid, UEdGraphPin*>& idToPinMap) override;

	UPROPERTY()
	UGeoFlowRuntimePin* RadiusInput = nullptr;
	UPROPERTY()
	FVector3f radius;
	virtual float Evaluate(const FVector3f& pos) override;
};


UCLASS()
class GEOFLOWCORE_API UGFN_E_PrimitiveEllipsoid : public UGFN_E_BasePrimitive {
	GENERATED_BODY()
public:
	virtual EGeoFlowNodeType NodeType() const override { return EGeoFlowNodeType::PrimitiveEllipsoid; }
	virtual FText GetNodeTitle(ENodeTitleType::Type titleType) const override { return FText::FromString("Ellipsoid Primitive"); }

	virtual UGFN_R_Base* CreateRuntimeNode(UGeoFlowRuntimeGraph* runtimeGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap < FGuid, UGeoFlowRuntimePin*>& idToPinMap) override;
	virtual TArray<UEdGraphPin*> CreateInputPins(UEdGraphPin* fromPin) override;
	virtual void PinDefaultValueChanged(UEdGraphPin* Pin) override {
		if (Pin == RadiusInput) {
			radius = GetVectorDefaultValue(Pin);
		}
		Super::PinDefaultValueChanged(Pin);
	}
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override {
		if (PropertyChangedEvent.GetMemberPropertyName() == GET_MEMBER_NAME_CHECKED(UGFN_E_PrimitiveEllipsoid, radius)) {
			SetVectorDefaultValue(RadiusInput, radius);
		}
		Super::PostEditChangeProperty(PropertyChangedEvent);
	}
	virtual void DrawInViewport(UWorld* InWorld) override;
	UEdGraphPin* RadiusInput = nullptr;
	UPROPERTY(EditAnywhere,Category="Node Properties", DisplayName="Extents")
	FVector3f radius;
};
UCLASS()
class GEOFLOWCORE_API UGFN_R_PrimitiveEllipsoid : public UGFN_R_BasePrimitive {
	GENERATED_BODY()
public:
	virtual EGeoFlowNodeType NodeType() const override { return EGeoFlowNodeType::PrimitiveEllipsoid; }

	virtual UGFN_E_Base* CreateEditorNode(UEdGraph* _workingGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap < FGuid, UEdGraphPin*>& idToPinMap) override;

	UPROPERTY()
	UGeoFlowRuntimePin* RadiusInput = nullptr;
	UPROPERTY()
	FVector3f radius;
	virtual float Evaluate(const FVector3f& pos) override;
};

UCLASS()
class GEOFLOWCORE_API UGFN_E_PrimitiveCone : public UGFN_E_BasePrimitive {
	GENERATED_BODY()
public:
	virtual EGeoFlowNodeType NodeType() const override { return EGeoFlowNodeType::PrimitiveCone; }
	virtual FText GetNodeTitle(ENodeTitleType::Type titleType) const override { return FText::FromString("Cone Primitive"); }

	virtual UGFN_R_Base* CreateRuntimeNode(UGeoFlowRuntimeGraph* runtimeGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap < FGuid, UGeoFlowRuntimePin*>& idToPinMap) override;
	virtual TArray<UEdGraphPin*> CreateInputPins(UEdGraphPin* fromPin) override;

	virtual void PinDefaultValueChanged(UEdGraphPin* Pin) override {
		if (Pin == AngleInput) {
			angle = GetFloatDefaultValue(Pin);
		}
		else if (Pin == HeightInput) {
			height = GetFloatDefaultValue(Pin);
		}
		Super::PinDefaultValueChanged(Pin);
	}
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override {
		if (PropertyChangedEvent.GetMemberPropertyName() == GET_MEMBER_NAME_CHECKED(UGFN_E_PrimitiveCone, angle)) {
			SetFloatDefaultValue(AngleInput, angle);
		}
		else if (PropertyChangedEvent.GetMemberPropertyName() == GET_MEMBER_NAME_CHECKED(UGFN_E_PrimitiveCone, height)) {
			SetFloatDefaultValue(HeightInput, height);
		}
		Super::PostEditChangeProperty(PropertyChangedEvent);
	}
	virtual void DrawInViewport(UWorld* InWorld) override;
	UEdGraphPin* AngleInput = nullptr;
	UEdGraphPin* HeightInput = nullptr;
	UPROPERTY(EditAnywhere,Category="Node Properties")
	float angle;
	UPROPERTY(EditAnywhere,Category="Node Properties")
	float height;
};
UCLASS()
class GEOFLOWCORE_API UGFN_R_PrimitiveCone : public UGFN_R_BasePrimitive {
	GENERATED_BODY()
public:
	virtual EGeoFlowNodeType NodeType() const override { return EGeoFlowNodeType::PrimitiveCone; }

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
class GEOFLOWCORE_API UGFN_E_PrimitiveCylinder : public UGFN_E_BasePrimitive {
	GENERATED_BODY()
public:
	virtual EGeoFlowNodeType NodeType() const override { return EGeoFlowNodeType::PrimitiveCylinder; }
	virtual FText GetNodeTitle(ENodeTitleType::Type titleType) const override { return FText::FromString("Cylinder Primitive"); }

	virtual UGFN_R_Base* CreateRuntimeNode(UGeoFlowRuntimeGraph* runtimeGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap < FGuid, UGeoFlowRuntimePin*>& idToPinMap) override;
	virtual TArray<UEdGraphPin*> CreateInputPins(UEdGraphPin* fromPin) override;
	virtual void PinDefaultValueChanged(UEdGraphPin* Pin) override {
		if (Pin == RadiusInput) {
			radius = GetFloatDefaultValue(Pin);
		}
		else if (Pin == HeightInput) {
			height = GetFloatDefaultValue(Pin);
		}
		Super::PinDefaultValueChanged(Pin);
	}
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override {
		if (PropertyChangedEvent.GetMemberPropertyName() == GET_MEMBER_NAME_CHECKED(UGFN_E_PrimitiveCylinder, radius)) {
			SetFloatDefaultValue(RadiusInput, radius);
		}
		else if (PropertyChangedEvent.GetMemberPropertyName() == GET_MEMBER_NAME_CHECKED(UGFN_E_PrimitiveCylinder, height)) {
			SetFloatDefaultValue(HeightInput, height);
		}
		Super::PostEditChangeProperty(PropertyChangedEvent);
	}
	virtual void DrawInViewport(UWorld* InWorld) override;
	UEdGraphPin* RadiusInput = nullptr;
	UEdGraphPin* HeightInput = nullptr;
	UPROPERTY(EditAnywhere,Category="Node Properties")
	float radius;
	UPROPERTY(EditAnywhere,Category="Node Properties")
	float height;
};
UCLASS()
class GEOFLOWCORE_API UGFN_R_PrimitiveCylinder : public UGFN_R_BasePrimitive {
	GENERATED_BODY()
public:
	virtual EGeoFlowNodeType NodeType() const override { return EGeoFlowNodeType::PrimitiveCylinder; }

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