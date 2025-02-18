#include "GeoFlowPin.h"
#include "GeoFlowNodeEnum.h"
#include "KismetPins/SGraphPinBool.h"
#include "KismetPins/SGraphPinInteger.h"
#include "KismetPins/SGraphPinNum.h"
#include "KismetPins/SGraphPinVector.h"
class SGeoFlowPinBool : public SGraphPinBool {
public:
	SLATE_BEGIN_ARGS(SGeoFlowPinBool) {}
	SLATE_END_ARGS()
	void Construct(const FArguments& inArgs, UEdGraphPin* inGraphPinObj) { SGraphPinBool::Construct(SGraphPinBool::FArguments(), inGraphPinObj); }
protected:
	virtual FSlateColor GetPinColor() const override { return FSlateColor(FLinearColor::Red); }
};
class SGeoFlowPinInteger : public SGraphPinInteger {
public:
	SLATE_BEGIN_ARGS(SGeoFlowPinInteger) {}
	SLATE_END_ARGS()
	void Construct(const FArguments& inArgs, UEdGraphPin* inGraphPinObj) { SGraphPinInteger::Construct(SGraphPinInteger::FArguments(), inGraphPinObj); }
protected:
	virtual FSlateColor GetPinColor() const override { return FSlateColor(FLinearColor::Blue); }
};
class SGeoFlowPinDouble : public SGraphPinNum<double> {
public:
	SLATE_BEGIN_ARGS(SGeoFlowPinDouble) {}
	SLATE_END_ARGS()
	void Construct(const FArguments& inArgs, UEdGraphPin* inGraphPinObj) { SGraphPinNum::Construct(SGraphPinNum::FArguments(), inGraphPinObj); }
protected:
	virtual FSlateColor GetPinColor() const override { return FSlateColor(FLinearColor::Green); }
};
class SGeoFlowPinVector : public SGraphPinVector<double> {
public:
	SLATE_BEGIN_ARGS(SGeoFlowPinVector) {}
	SLATE_END_ARGS()
	void Construct(const FArguments& inArgs, UEdGraphPin* inGraphPinObj) { SGraphPinVector::Construct(SGraphPinVector::FArguments(), inGraphPinObj); }
protected:
	virtual FSlateColor GetPinColor() const override { return FSlateColor(FLinearColor::Yellow); }
};
FGeoFlowPinFactory::~FGeoFlowPinFactory() {}

TSharedPtr<SGraphPin> FGeoFlowPinFactory::CreatePin(UEdGraphPin* pin) const {
	FName subCategory = pin->PinType.PinSubCategory;
	if (AllEnumPinNames().Contains(subCategory)) {//if its one of our pins
		EGeoFlowReturnType rtype = *NameToEnumMap().Find(subCategory);
		switch (rtype) {
		case EGeoFlowReturnType::None:
			//shouldnt happen ever
			return SNew(SGraphPin, pin);
		case EGeoFlowReturnType::Bool:
			return SNew(SGeoFlowPinBool, pin);
		case EGeoFlowReturnType::Int:
			return SNew(SGeoFlowPinInteger, pin);
		case EGeoFlowReturnType::Float:
			return SNew(SGeoFlowPinDouble, pin);
		case EGeoFlowReturnType::Vector:
			return SNew(SGeoFlowPinVector, pin);
		default:
			//unreachable
			return SNew(SGraphPin, pin);
		}
	}
	else {
		return nullptr;
	}
}
