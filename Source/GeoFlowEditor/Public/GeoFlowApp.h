#pragma once
#include "CoreMinimal.h"
#include "SGraphPanel.h"
#include "WorkflowOrientedApp/WorkflowCentricApplication.h"
#include "WorkflowOrientedApp/ApplicationMode.h"
#include "WorkflowOrientedApp/WorkflowTabManager.h"

class GeoFlowEditorApp : public FWorkflowCentricApplication, public FEditorUndoClient, public FNotifyHook {
public:
	virtual void RegisterTabSpawners(const TSharedRef<class FTabManager>& tabManager) override;
	void InitEditor(const EToolkitMode::Type mode, const TSharedPtr<class IToolkitHost>& initToolkitHost, UObject* inObject);

	class UGeoFlowAsset* GetWorkingAsset() { return _workingAsset; }
	class UEdGraph* GetWorkingGraph() { return _workingGraph; }
	void SetWorkingGraphUi(TSharedPtr<SGraphEditor> workingGraphUI) { _workingGraphUI = workingGraphUI; }
	void SetSelectedNodeDetailView(TSharedPtr<class IDetailsView> detailsView);
	void OnGraphSelectionChanged(const FGraphPanelSelectionSet& selection);

public: //FAssetEditorToolkit interface
	virtual FName GetToolkitFName() const override { return FName(TEXT("GeoFlowEditorApp")); }
	virtual FText GetBaseToolkitName() const override { return FText::FromString(TEXT("GeoFlowEditorApp")); }
	virtual FString GetWorldCentricTabPrefix() const override { return TEXT("GeoFlowEditorApp"); }
	virtual FLinearColor GetWorldCentricTabColorScale() const override { return FLinearColor(0.1f, 0.4f, 0.5f, 0.5f); }
	virtual FString GetDocumentationLink() const override { return TEXT("TODO"); }
	virtual void OnToolkitHostingStarted(const TSharedRef<class IToolkit>& toolkit) override {}
	virtual void OnToolkitHostingFinished(const TSharedRef<class IToolkit>& toolkit) override {}

	virtual void OnClose() override;
	void OnNodeDetailViewPropertiesUpdate(const FPropertyChangedEvent& event);
    void OnGraphChanged(const FEdGraphEditAction& editAction);

	TSharedPtr<FUICommandList> GetGraphEditorCommands();
	void DeleteSelectedNodes();
	void DuplicateSelectedNodes();
protected:
	TSharedPtr<FUICommandList> GraphEditorCommands;
	void UpdateWorkingAssetFromGraph();
	void UpdateEditorGraphFromWorkingAsset();
private:
	UPROPERTY()
	class UGeoFlowAsset* _workingAsset = nullptr;
	UPROPERTY()
	class UEdGraph* _workingGraph = nullptr;
	FGraphPanelSelectionSet _selection;
	TSharedPtr<SGraphEditor> _workingGraphUI = nullptr;
	TSharedPtr<class IDetailsView> _selectedNodeDetailsView = nullptr;
	
	FDelegateHandle _graphChangeListenerHandle;
};


class GeoFlowAppMode : public FApplicationMode {
public:
	GeoFlowAppMode(TSharedPtr<class GeoFlowEditorApp> app);

	virtual void RegisterTabFactories(TSharedPtr<class FTabManager> inTabManager) override;
	virtual void PreDeactivateMode() override;
	virtual void PostActivateMode() override;
private:
	TWeakPtr<class GeoFlowEditorApp> _app;
	FWorkflowAllowedTabSet _tabs;
};