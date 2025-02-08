// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "Styling/SlateStyle.h"
class FGeoFlowEditorModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
private:
	TSharedPtr<FSlateStyleSet> _styleSet = nullptr;
	TSharedPtr<struct FGeoFlowPinFactory> _pinFactory = nullptr;
	TSharedPtr<struct FGeoFlowNodeFactory> _nodeFactory = nullptr;
};
