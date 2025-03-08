#include "GeoFlowPin.h"
#include "GeoFlowNodeEnum.h"
#include "KismetPins/SGraphPinBool.h"
#include "KismetPins/SGraphPinInteger.h"
#include "KismetPins/SGraphPinNum.h"
#include "KismetPins/SGraphPinVector.h"
#include "Nodes/GeoFlowConstantNodes.h"
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
class SGeoFlowPinFloat : public SGraphPinNum<float> {
public:
	SLATE_BEGIN_ARGS(SGeoFlowPinFloat) {}
	SLATE_END_ARGS()
	void Construct(const FArguments& inArgs, UEdGraphPin* inGraphPinObj) { SGraphPinNum::Construct(SGraphPinNum::FArguments(), inGraphPinObj); }
protected:
	virtual FSlateColor GetPinColor() const override { return FSlateColor(FLinearColor::Green); }
};
class SGeoFlowPinVector : public SGraphPinVector<float> {
public:
	SLATE_BEGIN_ARGS(SGeoFlowPinVector) {}
	SLATE_END_ARGS()
	void Construct(const FArguments& inArgs, UEdGraphPin* inGraphPinObj) { SGraphPinVector::Construct(SGraphPinVector::FArguments(), inGraphPinObj); }
protected:
	virtual FSlateColor GetPinColor() const override { return FSlateColor(FLinearColor::Yellow); }
};
class SGeoFlowPinArray : public SGraphPin {
public:
	SLATE_BEGIN_ARGS(SGeoFlowPinArray) {}
	SLATE_END_ARGS()
	void Construct(const FArguments& inArgs, UEdGraphPin* inGraphPinObj) { SGraphPin::Construct(SGraphPin::FArguments(), inGraphPinObj); }
protected:
	virtual FSlateColor GetPinColor() const override { return FSlateColor(FColor::FromHex(TEXT("#0F52BA"))); }
};
FGeoFlowPinFactory::~FGeoFlowPinFactory() {}

TSharedPtr<SGraphPin> FGeoFlowPinFactory::CreatePin(UEdGraphPin* pin) const {
	FName subCategory = pin->PinType.PinSubCategory;
	if (AllEnumPinNames().Contains(subCategory)) {//if its one of our pins
		EGeoFlowReturnType rtype = *NameToEnumMap().Find(subCategory);
		TSharedPtr<SGraphPin> p;
		switch (rtype) {
		case EGeoFlowReturnType::None:
			//shouldnt happen ever
			p = SNew(SGraphPin, pin);
			break;
		case EGeoFlowReturnType::Bool:
			p = SNew(SGeoFlowPinBool, pin);
			break;
		case EGeoFlowReturnType::Int:
			p = SNew(SGeoFlowPinInteger, pin);
			break;
		case EGeoFlowReturnType::Float:
			p = SNew(SGeoFlowPinFloat, pin);
			break;
		case EGeoFlowReturnType::Vector:
			p = SNew(SGeoFlowPinVector, pin);
			break;
		case EGeoFlowReturnType::Array:
			p = SNew(SGeoFlowPinArray, pin);
			break;
		default:
			//unreachable
			return SNew(SGraphPin, pin);
		}
		if (pin->bNotConnectable) {
			const FSlateBrush* brush = FSlateIcon(TEXT("GeoFlowEditorStyle"), TEXT("GeoFlowEditor.HiddenPinIcon")).GetIcon();
			p->SetCustomPinIcon(brush, brush);
		}
		return p;
	}
	else {
		return nullptr;
	}
}
