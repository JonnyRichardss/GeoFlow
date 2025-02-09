#pragma once
#include "CoreMinimal.h"
#include "UObject/NameTypes.h"
#include "GeoFlowNodeEnum.h"
#include "GeoFlowRuntimeGraph.generated.h"
class UGFN_R_Base;
UCLASS()
class GEOFLOWCORE_API UGeoFlowRuntimeGraph : public UObject {
	GENERATED_BODY()
public:
	UPROPERTY()
	TArray<UGFN_R_Base*> Nodes;
};
UCLASS()
class GEOFLOWCORE_API UGeoFlowRuntimePin : public UObject {
	GENERATED_BODY()
public:
	UPROPERTY()
	FName PinName;
	UPROPERTY()
	FGuid PinId;
	UPROPERTY()
	UGeoFlowRuntimePin* Connection = nullptr;
	UPROPERTY()
	UGFN_R_Base* OwningNode = nullptr;
	UPROPERTY()
	TEnumAsByte<enum EEdGraphPinDirection> Direction;
	UPROPERTY()
	EGeoFlowReturnType ReturnType;

};
