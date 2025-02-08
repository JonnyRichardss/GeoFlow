#pragma once
double GetDoubleDefaultValue(UEdGraphPin* GraphPinObj);
void SetDoubleDefaultValue(UEdGraphPin* GraphPinObj,double inDouble);

FVector3d GetVectorDefaultValue(UEdGraphPin* GraphPinObj);
void SetVectorDefaultValue(UEdGraphPin* GraphPinObj, FVector3d inVec);

//TODO Getters and setters for bool and int