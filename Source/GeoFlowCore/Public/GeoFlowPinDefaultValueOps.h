#pragma once
float GetFloatDefaultValue(UEdGraphPin* GraphPinObj);
void SetFloatDefaultValue(UEdGraphPin* GraphPinObj,float inFloat);

FVector3f GetVectorDefaultValue(UEdGraphPin* GraphPinObj);
void SetVectorDefaultValue(UEdGraphPin* GraphPinObj, FVector3f inVec);

//TODO Getters and setters for bool and int