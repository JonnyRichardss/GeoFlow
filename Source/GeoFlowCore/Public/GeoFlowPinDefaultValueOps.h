#pragma once
#include "GeoFlowPinDefaultValueOps.generated.h"
USTRUCT()
struct FGeoFlowIntVector {
	GENERATED_BODY()
	UPROPERTY(EditAnywhere)
	int X;
	UPROPERTY(EditAnywhere)
	int Y;
	UPROPERTY(EditAnywhere)
	int Z;
};
float GetFloatDefaultValue(UEdGraphPin* GraphPinObj);
void SetFloatDefaultValue(UEdGraphPin* GraphPinObj,float inFloat);

FVector3f GetVectorDefaultValue(UEdGraphPin* GraphPinObj);
void SetVectorDefaultValue(UEdGraphPin* GraphPinObj, FVector3f inVec);

FGeoFlowIntVector GetIntVectorDefaultValue(UEdGraphPin* GraphPinObj);
void SetIntVectorDefaultValue(UEdGraphPin* GraphPinObj, FGeoFlowIntVector inVec);

bool GetBoolDefaultValue(UEdGraphPin* GraphPinObj);
void SetBoolDefaultValue(UEdGraphPin* GraphPinObj, bool inBool);

int GetIntDefaultValue(UEdGraphPin* GraphPinObj);
void SetIntDefaultValue(UEdGraphPin* GraphPinObj, int inValue);


//TODO Getters and setters for bool and int