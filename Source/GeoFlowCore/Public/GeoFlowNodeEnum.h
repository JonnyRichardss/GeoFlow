#pragma once
#include "GeoFlowNodeEnum.generated.h"
//TODO? namespace some of this

UENUM()
enum class EGeoFlowReturnType {
	None,
	Bool,
	Int,
	Float,
	Vector
};
GEOFLOWCORE_API FName  PinNameFromEnum(EGeoFlowReturnType type);
GEOFLOWCORE_API const TArray<FName>& AllEnumPinNames();
GEOFLOWCORE_API const TMap<EGeoFlowReturnType, FName>& EnumToNameMap();
GEOFLOWCORE_API const TMap<FName, EGeoFlowReturnType>& NameToEnumMap();
UENUM()
enum class EGeoFlowNodeType : int {
	
	ConstBool,
	ConstInt,
	ConstDouble,
	ConstVector,

	PrimitiveSphere,
	PrimitiveEllipsoid,
	PrimitiveCube,
	PrimitiveBox,
	PrimitiveCone,
	PrimitiveCylinder,


	Smin,
	Smax,
	Min,
	Max,


	//non-creatable types are at end for iterating purposes
	Output,
	Base,
	BaseBool,
	BaseInt,
	BaseFloat,
	BaseVector,
	BasePrimitive,
};
struct AddNodeArgs {
	UClass* TemplateClass;
	FString Category;
	FString Name;
};
GEOFLOWCORE_API AddNodeArgs EnumToAddNodeArgs(EGeoFlowNodeType type);
