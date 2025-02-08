#include "GeoFlowPinDefaultValueOps.h"


double GetDoubleDefaultValue(UEdGraphPin* GraphPinObj)
{
	double Num = double();
	LexFromString(Num, *GraphPinObj->GetDefaultAsString());
	return Num;
}
void SetDoubleDefaultValue(UEdGraphPin* GraphPinObj, double inDouble)
{
	GraphPinObj->DefaultValue = LexToString(inDouble);
}


void SetVectorDefaultValue(UEdGraphPin* GraphPinObj, FVector3d inVec)
{
	GraphPinObj->DefaultValue = FString::Format(TEXT("{0},{1},{2}"), { inVec.X, inVec.Y, inVec.Z });
}
FVector3d GetVectorDefaultValue(UEdGraphPin* GraphPinObj)
{
	TArray<FString> VecComponentStrings;
	FString DefaultString = GraphPinObj->GetDefaultAsString();
	// Parse string to split its contents separated by ','
	DefaultString.TrimStartInline();
	DefaultString.TrimEndInline();
	DefaultString.ParseIntoArray(VecComponentStrings, TEXT(","), true);
	// Construct the vector from the string parts.
	FVector3d Vec = FVector3d::ZeroVector;
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