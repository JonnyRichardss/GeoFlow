#pragma once
#include "GeoFlowNodeTypes.h"
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

	UPROPERTY(EditAnywhere)
	int Value = 0;
};
UCLASS()
class GEOFLOWCORE_API UGFN_E_ConstantDouble : public UGFN_E_BaseDouble {
	GENERATED_BODY()
public:
	virtual EGeoFlowNodeType NodeType() const override { return EGeoFlowNodeType::ConstDouble; }

	virtual FText GetNodeTitle(ENodeTitleType::Type titleType) const override { return FText::FromString("Constant double"); }

	virtual UGFN_R_Base* CreateRuntimeNode(UGeoFlowRuntimeGraph* runtimeGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap < FGuid, UGeoFlowRuntimePin*>& idToPinMap) override;

	UPROPERTY(EditAnywhere)
	double Value = 0.0;
};
UCLASS()
class GEOFLOWCORE_API UGFN_E_ConstantVector : public UGFN_E_BaseVector {
	GENERATED_BODY()
public:
	virtual EGeoFlowNodeType NodeType() const override { return EGeoFlowNodeType::ConstVector; }

	virtual FText GetNodeTitle(ENodeTitleType::Type titleType) const override { return FText::FromString("Constant vector"); }

	virtual UGFN_R_Base* CreateRuntimeNode(UGeoFlowRuntimeGraph* runtimeGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap < FGuid, UGeoFlowRuntimePin*>& idToPinMap) override;

	UPROPERTY(EditAnywhere)
	FVector3d Value = FVector3d::ZeroVector;
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
	virtual bool Evaluate(const FVector3d& pos) override { return Value; }
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
	virtual int Evaluate(const FVector3d& pos) override { return Value; }
};
UCLASS()
class GEOFLOWCORE_API UGFN_R_ConstantDouble : public UGFN_R_BaseDouble {
	GENERATED_BODY()
public:
	virtual EGeoFlowReturnType NodeReturnType() const override { return EGeoFlowReturnType::Double; }
	virtual EGeoFlowNodeType NodeType() const override { return EGeoFlowNodeType::ConstDouble; }

	virtual UGFN_E_Base* CreateEditorNode(UEdGraph* _workingGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap < FGuid, UEdGraphPin*>& idToPinMap) override;
	UPROPERTY()
	double Value;
	virtual double Evaluate(const FVector3d& pos) override { return Value; }
};
UCLASS()
class GEOFLOWCORE_API UGFN_R_ConstantVector : public UGFN_R_BaseVector {
	GENERATED_BODY()
public:
	virtual EGeoFlowReturnType NodeReturnType() const override { return EGeoFlowReturnType::Vector; }
	virtual EGeoFlowNodeType NodeType() const override { return EGeoFlowNodeType::ConstVector; }

	virtual UGFN_E_Base* CreateEditorNode(UEdGraph* _workingGraph, TArray<std::pair<FGuid, FGuid>>& connections, TMap < FGuid, UEdGraphPin*>& idToPinMap) override;
	UPROPERTY()
	FVector3d Value;
	virtual FVector3d Evaluate(const FVector3d& pos) override { return Value; }

};
