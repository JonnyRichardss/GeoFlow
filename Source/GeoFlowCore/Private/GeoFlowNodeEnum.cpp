#include "GeoFlowNodeEnum.h"

#include "Nodes/GeoFlowNodeTypes.h"
#include "Nodes/GeoFlowConstantNodes.h"
#include "Nodes/GeoFlowPrimitiveNodes.h"
#include "Nodes/GeoFlowProcessNodes.h"
#include "Nodes/GeoFlowMiscNodes.h"
#include "Nodes/GeoFlowArrayPrimitiveNodes.h"
#include "Nodes/GeoFlowPlacementNodes.h"
#include "Nodes/GeoFlowMathNodes.h"




FName PinNameFromEnum(EGeoFlowReturnType type)
{
	switch (type) {
	case EGeoFlowReturnType::None:
		return TEXT("GeoFlowPinNone");
	case EGeoFlowReturnType::Bool:
		return TEXT("GeoFlowPinBool");
	case EGeoFlowReturnType::Int:
		return TEXT("GeoFlowPinInt");
	case EGeoFlowReturnType::Float:
		return TEXT("GeoFlowPinDouble");
	case EGeoFlowReturnType::Vector:
		return TEXT("GeoFlowPinVector");
	case EGeoFlowReturnType::Array:
		return TEXT("GeoFlowPinArray");
	default:
		//unreachable
		return TEXT("");
	}
}
static TMap<EGeoFlowReturnType, FName> allTypesNames;
static TMap<FName, EGeoFlowReturnType> allNamesTypes;
static const TArray<FName> names =				{TEXT("GeoFlowPinNone")  ,TEXT("GeoFlowPinBool")  ,TEXT("GeoFlowPinInt")  ,TEXT("GeoFlowPinDouble")  ,TEXT("GeoFlowPinVector"), TEXT("GeoFlowPinArray")};
static const TArray<EGeoFlowReturnType> types =	{EGeoFlowReturnType::None,EGeoFlowReturnType::Bool,EGeoFlowReturnType::Int,EGeoFlowReturnType::Float,EGeoFlowReturnType::Vector,EGeoFlowReturnType::Array};
//i think lazy loading is the easiest way to do this since i cant just {} a TMap
static void PopulateAllNamesTypes() {
	for (int i = 0; i < names.Num(); i++) {
		allTypesNames.Add(types[i], names[i]);
	}
}
static void PopulateAllTypesNames() {
	for (int i = 0; i < names.Num(); i++) {
		allNamesTypes.Add(names[i], types[i]);
	}
}
const TArray<FName>& AllEnumPinNames()
{
	return names;
}
const TMap<EGeoFlowReturnType, FName>& EnumToNameMap() {
	if (allTypesNames.Num() == 0) PopulateAllNamesTypes();
	return allTypesNames;
}
const TMap<FName,EGeoFlowReturnType>& NameToEnumMap() {
	if (allNamesTypes.Num() == 0) PopulateAllTypesNames();
	return allNamesTypes;
}
AddNodeArgs EnumToAddNodeArgs(EGeoFlowNodeType type)
{
	switch (type) {
	case EGeoFlowNodeType::ConstBool:
		return {
			UGFN_E_ConstantBool::StaticClass(),
			TEXT("Constant"),
			TEXT("Bool")
		};
	case EGeoFlowNodeType::ConstInt:
		return { 
			UGFN_E_ConstantInt::StaticClass(),
			TEXT("Constant"),
			TEXT("Int")
		};
	case EGeoFlowNodeType::ConstDouble:
		return { 
			UGFN_E_ConstantFloat::StaticClass(),
			TEXT("Constant"),
			TEXT("Float")
		};
	case EGeoFlowNodeType::ConstVector:
		return {
			UGFN_E_ConstantVector::StaticClass(),
			TEXT("Constant"),
			TEXT("Vector")
		};
	case EGeoFlowNodeType::ConstArray:
		return {
			UGFN_E_ConstantArray::StaticClass(),
			TEXT("Constant"),
			TEXT("Transform Array")
		};

	case EGeoFlowNodeType::PrimitiveSphere:
		return {
			UGFN_E_PrimitiveSphere::StaticClass(),
			TEXT("Primitive"),
			TEXT("Sphere")
		};
		
	case EGeoFlowNodeType::PrimitiveEllipsoid:
		return {
			UGFN_E_PrimitiveEllipsoid::StaticClass(),
			TEXT("Primitive"),
			TEXT("Ellipsoid")
		};	
	case EGeoFlowNodeType::PrimitiveCube:
		return {
			UGFN_E_PrimitiveCube::StaticClass(),
			TEXT("Primitive"),
			TEXT("Cube")
		};
	case EGeoFlowNodeType::PrimitiveBox:
		return {
			UGFN_E_PrimitiveBox::StaticClass(),
			TEXT("Primitive"),
			TEXT("Box")
		};
	case EGeoFlowNodeType::PrimitiveCone:
		return {
			UGFN_E_PrimitiveCone::StaticClass(),
			TEXT("Primitive"),
			TEXT("Cone")
		};
	case EGeoFlowNodeType::PrimitiveCylinder:
		return {
			UGFN_E_PrimitiveCylinder::StaticClass(),
			TEXT("Primitive"),
			TEXT("Cylinder")
		};

	case EGeoFlowNodeType::ArrayPrimitiveSphere:
		return {
			UGFN_E_ArrayPrimitiveSphere::StaticClass(),
			TEXT("Primitive Array"),
			TEXT("Sphere")
		};

	case EGeoFlowNodeType::ArrayPrimitiveEllipsoid:
		return {
			UGFN_E_ArrayPrimitiveEllipsoid::StaticClass(),
			TEXT("Primitive Array"),
			TEXT("Ellipsoid")
		};
	case EGeoFlowNodeType::ArrayPrimitiveCube:
		return {
			UGFN_E_ArrayPrimitiveCube::StaticClass(),
			TEXT("Primitive Array"),
			TEXT("Cube")
		};
	case EGeoFlowNodeType::ArrayPrimitiveBox:
		return {
			UGFN_E_ArrayPrimitiveBox::StaticClass(),
			TEXT("Primitive Array"),
			TEXT("Box")
		};
	case EGeoFlowNodeType::ArrayPrimitiveCone:
		return {
			UGFN_E_ArrayPrimitiveCone::StaticClass(),
			TEXT("Primitive Array"),
			TEXT("Cone")
		};
	case EGeoFlowNodeType::ArrayPrimitiveCylinder:
		return {
			UGFN_E_ArrayPrimitiveCylinder::StaticClass(),
			TEXT("Primitive Array"),
			TEXT("Cylinder")
		};
	case EGeoFlowNodeType::Mirror:
		return {
			UGFN_E_Mirror::StaticClass(),
			TEXT("Placement"),
			TEXT("Mirror Vector")
		};
	case EGeoFlowNodeType::Grid:
		return {
			UGFN_E_Grid::StaticClass(),
			TEXT("Placement"),
			TEXT("Grid Vector")
		};


	case EGeoFlowNodeType::Smin:
		return {
			UGFN_E_Smin::StaticClass(),
			TEXT("Boolean"),
			TEXT("Smoothed Union")
		};
	case EGeoFlowNodeType::Smax:
		return {
			UGFN_E_Smax::StaticClass(),
			TEXT("Boolean"),
			TEXT("Smoothed Subtraction")
		};
	case EGeoFlowNodeType::Min:
		return {
			UGFN_E_Min::StaticClass(),
			TEXT("Boolean"),
			TEXT("Union")
		};
	case EGeoFlowNodeType::Max:
		return {
			UGFN_E_Max::StaticClass(),
			TEXT("Boolean"),
			TEXT("Subtraction")
		};


	case EGeoFlowNodeType::FloatAdd:
		return {
			UGFN_E_FloatAdd::StaticClass(),
			TEXT("Math"),
			TEXT("Float Add")
		};
	case EGeoFlowNodeType::FloatMultiply:
		return {
			UGFN_E_FloatAdd::StaticClass(),
			TEXT("Math"),
			TEXT("Float Multiply")
		};
	case EGeoFlowNodeType::FloatSubtract:
		return {
			UGFN_E_FloatSubtract::StaticClass(),
			TEXT("Math"),
			TEXT("Float Subtract")
		};
	case EGeoFlowNodeType::FloatDivide:
		return {
			UGFN_E_FloatDivide::StaticClass(),
			TEXT("Math"),
			TEXT("Float Divide")
		};
	case EGeoFlowNodeType::VectorAdd:
		return {
			UGFN_E_VectorAdd::StaticClass(),
			TEXT("Math"),
			TEXT("Vector Add")
		};
	case EGeoFlowNodeType::VectorMultiply:
		return {
			UGFN_E_VectorAdd::StaticClass(),
			TEXT("Math"),
			TEXT("Vector Multiply")
		};
	case EGeoFlowNodeType::VectorSubtract:
		return {
			UGFN_E_VectorSubtract::StaticClass(),
			TEXT("Math"),
			TEXT("Vector Subtract")
		};
	case EGeoFlowNodeType::VectorDivide:
		return {
			UGFN_E_VectorDivide::StaticClass(),
			TEXT("Math"),
			TEXT("Vector Divide")
		};

	case EGeoFlowNodeType::MakeVector:
		return {
			UGFN_E_MakeVector::StaticClass(),
			TEXT("Math"),
			TEXT("Make Vector")
		};
	








	case EGeoFlowNodeType::Output:
		return {
			UGFN_E_Output::StaticClass(),
			TEXT("Base"),
			TEXT("Output")
		};

	case EGeoFlowNodeType::Base:
		return {
			UGFN_E_Base::StaticClass(),
			TEXT("Base"),
			TEXT("Base")
		};
	case EGeoFlowNodeType::BaseBool:
		return {
			UGFN_E_BaseBool::StaticClass(),
			TEXT("Base"),
			TEXT("Bool")
		};
	case EGeoFlowNodeType::BaseInt:
		return {
			UGFN_E_BaseInt::StaticClass(),
			TEXT("Base"),
			TEXT("Int")
		};
	case EGeoFlowNodeType::BaseFloat:
		return {
			UGFN_E_BaseFloat::StaticClass(),
			TEXT("Base"),
			TEXT("Float")
		};
	case EGeoFlowNodeType::BaseVector:
		return {
			UGFN_E_BaseVector::StaticClass(),
			TEXT("Base"),
			TEXT("Vector")
		};
	case EGeoFlowNodeType::BaseArray:
		return {
			UGFN_E_BaseArray::StaticClass(),
			TEXT("Base"),
			TEXT("Array")
		};
	case EGeoFlowNodeType::BasePrimitive:
		return {
			UGFN_E_BasePrimitive::StaticClass(),
			TEXT("Base"),
			TEXT("Primitive")
		};
	case EGeoFlowNodeType::BaseArrayPrimitive:
		return {
			UGFN_E_BaseArrayPrimitive::StaticClass(),
			TEXT("Base"),
			TEXT("Primitive Array")
		};
	case EGeoFlowNodeType::FloatMath:
		return {
			UGFN_E_FloatMathBase::StaticClass(),
			TEXT("Math"),
			TEXT("Base Float")
		};
	case EGeoFlowNodeType::VectorMath:
		return {
			UGFN_E_VectorMathBase::StaticClass(),
			TEXT("Math"),
			TEXT("Base Vector")
		};
	default:
		//this should never happen so crash to make it clear a case is missing
		check(false);
		return{ nullptr,TEXT(""),TEXT("") };
	}
}
