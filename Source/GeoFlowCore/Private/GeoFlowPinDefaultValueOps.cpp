#include "GeoFlowPinDefaultValueOps.h"


float GetFloatDefaultValue(UEdGraphPin* GraphPinObj)
{
	float Num = float();
	LexFromString(Num, *GraphPinObj->GetDefaultAsString());
	return Num;
}
void SetFloatDefaultValue(UEdGraphPin* GraphPinObj, float inFloat)
{
	GraphPinObj->DefaultValue = LexToString(inFloat);
}

FVector3f GetVectorDefaultValue(UEdGraphPin* GraphPinObj)
{
	TArray<FString> VecComponentStrings;
	FString DefaultString = GraphPinObj->GetDefaultAsString();
	// Parse string to split its contents separated by ','
	DefaultString.TrimStartInline();
	DefaultString.TrimEndInline();
	DefaultString.ParseIntoArray(VecComponentStrings, TEXT(","), true);
	// Construct the vector from the string parts.
	FVector3f Vec = FVector3f::ZeroVector;
	TDefaultNumericTypeInterface<double> NumericTypeInterface{};

	// If default string value contained a fully specified 3D vector, set the vector components, otherwise leave it zero'ed.
	if (VecComponentStrings.Num() == 3)
	{
		Vec.X = NumericTypeInterface.FromString(VecComponentStrings[0], 0).Get(0);
		Vec.Y = NumericTypeInterface.FromString(VecComponentStrings[1], 0).Get(0);
		Vec.Z = NumericTypeInterface.FromString(VecComponentStrings[2], 0).Get(0);
	}

	return Vec;
}
void SetVectorDefaultValue(UEdGraphPin* GraphPinObj, FVector3f inVec)
{
	GraphPinObj->DefaultValue = FString::Format(TEXT("{0},{1},{2}"), { inVec.X, inVec.Y, inVec.Z });
}
FGeoFlowIntVector GetIntVectorDefaultValue(UEdGraphPin* GraphPinObj)
{
	TArray<FString> VecComponentStrings;
	FString DefaultString = GraphPinObj->GetDefaultAsString();
	// Parse string to split its contents separated by ','
	DefaultString.TrimStartInline();
	DefaultString.TrimEndInline();
	DefaultString.ParseIntoArray(VecComponentStrings, TEXT(","), true);
	// Construct the vector from the string parts.
	FGeoFlowIntVector Vec = { 0,0,0 };
	TDefaultNumericTypeInterface<double> NumericTypeInterface{};

	// If default string value contained a fully specified 3D vector, set the vector components, otherwise leave it zero'ed.
	if (VecComponentStrings.Num() == 3)
	{
		Vec.X = NumericTypeInterface.FromString(VecComponentStrings[0], 0).Get(0);
		Vec.Y = NumericTypeInterface.FromString(VecComponentStrings[1], 0).Get(0);
		Vec.Z = NumericTypeInterface.FromString(VecComponentStrings[2], 0).Get(0);
	}

	return Vec;
}
void SetIntVectorDefaultValue(UEdGraphPin* GraphPinObj, FGeoFlowIntVector inVec)
{
	GraphPinObj->DefaultValue = FString::Format(TEXT("{0},{1},{2}"), { inVec.X, inVec.Y, inVec.Z });
}
bool GetBoolDefaultValue(UEdGraphPin* GraphPinObj)
{
	return GraphPinObj->GetDefaultAsString().ToBool();
}
void SetBoolDefaultValue(UEdGraphPin* GraphPinObj, bool inBool)
{
	GraphPinObj->DefaultValue = LexToString(inBool);
}
int GetIntDefaultValue(UEdGraphPin* GraphPinObj)
{
	int Num = int();
	LexFromString(Num, *GraphPinObj->GetDefaultAsString());
	return Num;
}
void SetIntDefaultValue(UEdGraphPin* GraphPinObj, int inValue)
{
	GraphPinObj->DefaultValue = LexToString(inValue);
}
