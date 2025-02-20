#pragma once

#include "EdGraph/EdGraphPin.h"
#include "GeoFlowNodeTypes.h"
#include "GeoFlowPrimitiveNodes.generated.h"

UCLASS()
class GEOFLOWCORE_API UGFN_E_BasePrimitive : public UGFN_E_BaseFloat {
	GENERATED_BODY()
public:
	virtual EGeoFlowNodeType NodeType() const override { return EGeoFlowNodeType::BasePrimitive; }
	virtual FText GetNodeTitle(ENodeTitleType::Type titleType) const override { return FText::FromString("Default Primitive node"); }
	UEdGraphPin* PositionInput = nullptr;
};
UCLASS()
class GEOFLOWCORE_API UGFN_R_BasePrimitive : public UGFN_R_BaseFloat {
	GENERATED_BODY()
public:
	virtual EGeoFlowNodeType NodeType() const override { return EGeoFlowNodeType::BasePrimitive; }

	UPROPERTY()
	UGeoFlowRuntimePin* PositionInput = nullptr;
	UPROPERTY()
	FVector3f position;

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


	UEdGraphPin* RadiusInput = nullptr;
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
	virtual FString CreateShaderEvalCall(TArray<FString>& PinDeclarations) override;
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


	UEdGraphPin* RadiusInput = nullptr;
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
	virtual FString CreateShaderEvalCall(TArray<FString>& PinDeclarations) override;
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

	UEdGraphPin* RadiusInput = nullptr;
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
	virtual FString CreateShaderEvalCall(TArray<FString>& PinDeclarations) override;
	virtual float Evaluate(const FVector3f& pos) override;
};
