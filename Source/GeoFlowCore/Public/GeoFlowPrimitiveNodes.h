#pragma once

#include "EdGraph/EdGraphPin.h"
#include "GeoFlowNodeTypes.h"
#include "GeoFlowPrimitiveNodes.generated.h"

UCLASS()
class GEOFLOWCORE_API UGFN_E_PrimitiveSphere : public UGFN_E_BaseDouble {
	GENERATED_BODY()
public:
	virtual EGeoFlowNodeType NodeType() const override { return EGeoFlowNodeType::PrimitiveSphere; }
	virtual FText GetNodeTitle(ENodeTitleType::Type titleType) const override { return FText::FromString("Sphere Primitive"); }

	virtual UGFN_R_Base* CreateRuntimeNode(UGeoFlowRuntimeGraph* runtimeGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap < FGuid, UGeoFlowRuntimePin*>& idToPinMap) override;
	virtual TArray<UEdGraphPin*> CreateInputPins(UEdGraphPin* fromPin) override;


	UEdGraphPin* CentreInput = nullptr;
	UEdGraphPin* RadiusInput = nullptr;
};
UCLASS()
class GEOFLOWCORE_API UGFN_R_PrimitiveSphere : public UGFN_R_BaseDouble {
	GENERATED_BODY()
public:
	virtual EGeoFlowNodeType NodeType() const override { return EGeoFlowNodeType::PrimitiveSphere; }

	virtual UGFN_E_Base* CreateEditorNode(UEdGraph* _workingGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap < FGuid, UEdGraphPin*>& idToPinMap) override;

	UPROPERTY()
	UGeoFlowRuntimePin* CentreInput = nullptr;
	UPROPERTY()
	UGeoFlowRuntimePin* RadiusInput = nullptr;
	UPROPERTY()
	FVector3d centre;
	UPROPERTY()
	double radius;

	virtual double Evaluate(const FVector3d& pos) override;
};


UCLASS()
class GEOFLOWCORE_API UGFN_E_PrimitiveBox : public UGFN_E_BaseDouble {
	GENERATED_BODY()
public:
	virtual EGeoFlowNodeType NodeType() const override { return EGeoFlowNodeType::PrimitiveBox; }
	virtual FText GetNodeTitle(ENodeTitleType::Type titleType) const override { return FText::FromString("Box Primitive"); }

	virtual UGFN_R_Base* CreateRuntimeNode(UGeoFlowRuntimeGraph* runtimeGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap < FGuid, UGeoFlowRuntimePin*>& idToPinMap) override;
	virtual TArray<UEdGraphPin*> CreateInputPins(UEdGraphPin* fromPin) override;


	UEdGraphPin* CentreInput = nullptr;
	UEdGraphPin* RadiusInput = nullptr;
};
UCLASS()
class GEOFLOWCORE_API UGFN_R_PrimitiveBox : public UGFN_R_BaseDouble {
	GENERATED_BODY()
public:
	virtual EGeoFlowNodeType NodeType() const override { return EGeoFlowNodeType::PrimitiveBox; }

	virtual UGFN_E_Base* CreateEditorNode(UEdGraph* _workingGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap < FGuid, UEdGraphPin*>& idToPinMap) override;

	UPROPERTY()
	UGeoFlowRuntimePin* CentreInput = nullptr;
	UPROPERTY()
	UGeoFlowRuntimePin* RadiusInput = nullptr;
	UPROPERTY()
	FVector3d centre;
	UPROPERTY()
	FVector3d radius;

	virtual double Evaluate(const FVector3d& pos) override;
};



UCLASS()
class GEOFLOWCORE_API UGFN_E_PrimitivePlane : public UGFN_E_BaseDouble {
	GENERATED_BODY()
public:
	virtual EGeoFlowNodeType NodeType() const override { return EGeoFlowNodeType::PrimitivePlane; }
	virtual FText GetNodeTitle(ENodeTitleType::Type titleType) const override { return FText::FromString("Plane Primitive"); }

	virtual UGFN_R_Base* CreateRuntimeNode(UGeoFlowRuntimeGraph* runtimeGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap < FGuid, UGeoFlowRuntimePin*>& idToPinMap) override;
	virtual TArray<UEdGraphPin*> CreateInputPins(UEdGraphPin* fromPin) override;


	//since i got lazy copying these have the wrong names -- should be normal vector and offset
	UEdGraphPin* NormalInput = nullptr;
	UEdGraphPin* OffsetInput = nullptr;
};
UCLASS()
class GEOFLOWCORE_API UGFN_R_PrimitivePlane : public UGFN_R_BaseDouble {
	GENERATED_BODY()
public:
	virtual EGeoFlowNodeType NodeType() const override { return EGeoFlowNodeType::PrimitivePlane; }

	virtual UGFN_E_Base* CreateEditorNode(UEdGraph* _workingGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap < FGuid, UEdGraphPin*>& idToPinMap) override;

	UPROPERTY()
	UGeoFlowRuntimePin* NormalInput = nullptr;
	UPROPERTY()
	UGeoFlowRuntimePin* OffsetInput = nullptr;
	UPROPERTY()
	FVector3d normal;
	UPROPERTY()
	double offset;

	virtual double Evaluate(const FVector3d& pos) override;
};





UCLASS()
class GEOFLOWCORE_API UGFN_E_PrimitiveEllipsoid : public UGFN_E_BaseDouble {
	GENERATED_BODY()
public:
	virtual EGeoFlowNodeType NodeType() const override { return EGeoFlowNodeType::PrimitiveEllipsoid; }
	virtual FText GetNodeTitle(ENodeTitleType::Type titleType) const override { return FText::FromString("Ellipsoid Primitive"); }

	virtual UGFN_R_Base* CreateRuntimeNode(UGeoFlowRuntimeGraph* runtimeGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap < FGuid, UGeoFlowRuntimePin*>& idToPinMap) override;
	virtual TArray<UEdGraphPin*> CreateInputPins(UEdGraphPin* fromPin) override;


	UEdGraphPin* CentreInput = nullptr;
	UEdGraphPin* RadiusInput = nullptr;
};
UCLASS()
class GEOFLOWCORE_API UGFN_R_PrimitiveEllipsoid : public UGFN_R_BaseDouble {
	GENERATED_BODY()
public:
	virtual EGeoFlowNodeType NodeType() const override { return EGeoFlowNodeType::PrimitiveEllipsoid; }

	virtual UGFN_E_Base* CreateEditorNode(UEdGraph* _workingGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap < FGuid, UEdGraphPin*>& idToPinMap) override;

	UPROPERTY()
	UGeoFlowRuntimePin* CentreInput = nullptr;
	UPROPERTY()
	UGeoFlowRuntimePin* RadiusInput = nullptr;
	UPROPERTY()
	FVector3d centre;
	UPROPERTY()
	FVector3d radius;

	virtual double Evaluate(const FVector3d& pos) override;
};
