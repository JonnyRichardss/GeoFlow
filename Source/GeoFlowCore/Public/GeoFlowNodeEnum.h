#pragma once
#include "GeoFlowNodeEnum.generated.h"
//TODO? namespace some of this

UENUM()
enum class EGeoFlowReturnType {
	None,
	Bool,
	Int,
	Float,
	Vector,
	Array
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
	ConstArray,

	PrimitiveSphere,
	PrimitiveEllipsoid,
	PrimitiveCube,
	PrimitiveBox,
	PrimitiveCone,
	PrimitiveCylinder,

	ArrayPrimitiveSphere,
	ArrayPrimitiveEllipsoid,
	ArrayPrimitiveCube,
	ArrayPrimitiveBox,
	ArrayPrimitiveCone,
	ArrayPrimitiveCylinder,

	Mirror,
	Grid,

	Smin,
	Smax,
	Min,
	Max,

	
	FloatAdd,
	FloatMultiply,
	FloatSubtract,
	FloatDivide,
	
	VectorAdd,
	VectorMultiply,
	VectorSubtract,
	VectorDivide,

	MakeVector,


	//non-creatable types are at end for iterating purposes
	Output,
	Base,
	BaseBool,
	BaseInt,
	BaseFloat,
	BaseVector,
	BaseArray,
	BasePrimitive,
	BaseArrayPrimitive,
	FloatMath,
	VectorMath,
};
struct AddNodeArgs {
	UClass* TemplateClass;
	FString Category;
	FString Name;
};
GEOFLOWCORE_API AddNodeArgs EnumToAddNodeArgs(EGeoFlowNodeType type);
