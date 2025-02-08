#include "GeoFlowNodeEnum.h"

#include "GeoFlowNodeTypes.h"
#include "GeoFlowConstantNodes.h"
#include "GeoFlowPrimitiveNodes.h"
#include "GeoFlowProcessNodes.h"
#include "GeoFlowMiscNodes.h"




FName PinNameFromEnum(EGeoFlowReturnType type)
{
	switch (type) {
	case EGeoFlowReturnType::None:
		return TEXT("GeoFlowPinNone");
	case EGeoFlowReturnType::Bool:
		return TEXT("GeoFlowPinBool");
	case EGeoFlowReturnType::Int:
		return TEXT("GeoFlowPinInt");
	case EGeoFlowReturnType::Double:
		return TEXT("GeoFlowPinDouble");
	case EGeoFlowReturnType::Vector:
		return TEXT("GeoFlowPinVector");
	default:
		//unreachable
		return TEXT("");
	}
}
static TMap<EGeoFlowReturnType, FName> allTypesNames;
static TMap<FName, EGeoFlowReturnType> allNamesTypes;
static const TArray<FName> names =				{TEXT("GeoFlowPinNone")  ,TEXT("GeoFlowPinBool")  ,TEXT("GeoFlowPinInt")  ,TEXT("GeoFlowPinDouble")  ,TEXT("GeoFlowPinVector")  };
static const TArray<EGeoFlowReturnType> types =	{EGeoFlowReturnType::None,EGeoFlowReturnType::Bool,EGeoFlowReturnType::Int,EGeoFlowReturnType::Double,EGeoFlowReturnType::Vector};
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
			UGFN_E_ConstantDouble::StaticClass(),
			TEXT("Constant"),
			TEXT("Float")
		};
	case EGeoFlowNodeType::ConstVector:
		return {
			UGFN_E_ConstantVector::StaticClass(),
			TEXT("Constant"),
			TEXT("Vector")
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
	case EGeoFlowNodeType::PrimitiveBox:
		return {
			UGFN_E_PrimitiveBox::StaticClass(),
			TEXT("Primitive"),
			TEXT("Box")
		};
	case EGeoFlowNodeType::PrimitivePlane:
		return {
			UGFN_E_PrimitivePlane::StaticClass(),
			TEXT("Primitive"),
			TEXT("Plane")
		};
		
	case EGeoFlowNodeType::Smin:
		return {
			UGFN_E_Smin::StaticClass(),
			TEXT("Process"),
			TEXT("Smoothed Union")
		};
	case EGeoFlowNodeType::Smax:
		return {
			UGFN_E_Smax::StaticClass(),
			TEXT("Process"),
			TEXT("Smoothed Subtraction")
		};
	case EGeoFlowNodeType::Min:
		return {
			UGFN_E_Min::StaticClass(),
			TEXT("Process"),
			TEXT("Union")
		};
	case EGeoFlowNodeType::Max:
		return {
			UGFN_E_Max::StaticClass(),
			TEXT("Process"),
			TEXT("Subtraction")
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
	case EGeoFlowNodeType::BaseDouble:
		return {
			UGFN_E_BaseDouble::StaticClass(),
			TEXT("Base"),
			TEXT("Float")
		};
	case EGeoFlowNodeType::BaseVector:
		return {
			UGFN_E_BaseVector::StaticClass(),
			TEXT("Base"),
			TEXT("Vector")
		};
	default:
		check(false);
		return{ nullptr,TEXT(""),TEXT("") };
	}
}
