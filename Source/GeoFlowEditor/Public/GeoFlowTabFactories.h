#pragma once
#include "CoreMinimal.h"
#include "WorkflowOrientedApp/WorkflowTabFactory.h"
#include "GeoFlowViewport.h"
class GeoFlowPrimaryTabFactory : public FWorkflowTabFactory {
public:
	GeoFlowPrimaryTabFactory(TSharedPtr<class GeoFlowEditorApp> app);

	virtual TSharedRef<SWidget> CreateTabBody(const FWorkflowTabSpawnInfo& info) const override;
	virtual FText GetTabToolTipText(const FWorkflowTabSpawnInfo& info) const override;


private:
	TWeakPtr<class GeoFlowEditorApp> _app;
};

class GeoFlowPropertiesTabFactory : public FWorkflowTabFactory {
public:
	GeoFlowPropertiesTabFactory(TSharedPtr<class GeoFlowEditorApp> app);

	virtual TSharedRef<SWidget> CreateTabBody(const FWorkflowTabSpawnInfo& info) const override;
	virtual FText GetTabToolTipText(const FWorkflowTabSpawnInfo& info) const override;
private:
	TWeakPtr<class GeoFlowEditorApp> _app;
};
class GeoFlowPreviewTabFactory : public FWorkflowTabFactory {
public:
	GeoFlowPreviewTabFactory(TSharedPtr<class GeoFlowEditorApp> app);

	virtual TSharedRef<SWidget> CreateTabBody(const FWorkflowTabSpawnInfo& info) const override;
	virtual FText GetTabToolTipText(const FWorkflowTabSpawnInfo& info) const override;
private:
	TWeakPtr<class GeoFlowEditorApp> _app;
};
