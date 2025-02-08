#pragma once
#include "CoreMinimal.h"
#include "EdGraphUtilities.h"
#include "KismetPins/SGraphPinColor.h"
#include "EdGraph/EdGraphPin.h"
//SGraphPin overrides are in CPP file just because
struct FGeoFlowPinFactory : public FGraphPanelPinFactory {
public:
	virtual ~FGeoFlowPinFactory();
	virtual TSharedPtr<SGraphPin> CreatePin(UEdGraphPin* pin) const override;
};
