#include "GeoFlowApp.h"
#include "GeoFlowTabFactories.h"
#include "GeoFlowAssetActions.h"
#include "AdvancedPreviewSceneModule.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "GeoFlowGraphSchema.h"
#include "GeoFlowRuntimeGraph.h"
#include "Nodes/GeoFlowNodeTypes.h"
#include "GeoFlowAsset.h"
#include "Framework/Commands/GenericCommands.h"
#include "GeoFlowViewport.h"


void GeoFlowEditorApp::RegisterTabSpawners(const TSharedRef<class FTabManager>& tabManager)
{
	FWorkflowCentricApplication::RegisterTabSpawners(tabManager);
}

void GeoFlowEditorApp::InitEditor(const EToolkitMode::Type mode, const TSharedPtr<class IToolkitHost>& initToolkitHost, UObject* inObject)
{
	TArray<UObject*> objects;
	objects.Add(inObject);

	_workingAsset = Cast<UGeoFlowAsset>(inObject);

	_workingGraph = FBlueprintEditorUtils::CreateNewGraph(
		_workingAsset,
		NAME_None,
		UEdGraph::StaticClass(),
		UGeoFlowGraphSchema::StaticClass()
	);


	InitAssetEditor(
		mode,
		initToolkitHost,
		TEXT("GeoFlowEditor"),
		FTabManager::FLayout::NullLayout,
		true, //createStandaloneMenu
		true, //createDefaultToolbar
		objects
	);

	AddApplicationMode(TEXT("GeoFlowEditorAppMode"), MakeShareable(new GeoFlowAppMode(SharedThis(this))));
	SetCurrentMode(TEXT("GeoFlowEditorAppMode"));

	UpdateEditorGraphFromWorkingAsset();
	_graphChangeListenerHandle = _workingGraph->AddOnGraphChangedHandler(FOnGraphChanged::FDelegate::CreateSP(this,&GeoFlowEditorApp::OnGraphChanged));
	GenerateDelegate = FOnClicked::CreateSP(this, &GeoFlowEditorApp::OnGenerateClicked);
	FCoreDelegates::OnPreExit.AddLambda([this]()
		{
			if (!ViewportWidget.IsValid())
				return;

			ViewportWidget->PreviewScene.Reset();
			ViewportWidget->ViewportClient.Reset();
			ViewportWidget.Reset();
		});
	
	FAdvancedPreviewSceneModule& AdvancedPreviewSceneModule = FModuleManager::LoadModuleChecked<FAdvancedPreviewSceneModule>("AdvancedPreviewScene");
	TSharedRef<SWidget> PreviewDetails = AdvancedPreviewSceneModule.CreateAdvancedPreviewSceneSettingsWidget(ViewportWidget->PreviewScene.ToSharedRef());

	ViewportClient = ViewportWidget->GetViewportClient().ToSharedRef();
	ViewportClient->SetAsset(_workingAsset);
}

void GeoFlowEditorApp::SetSelectedNodeDetailView(TSharedPtr<class IDetailsView> detailsView)
{
	_selectedNodeDetailsView = detailsView;
	_selectedNodeDetailsView->OnFinishedChangingProperties().AddRaw(this, &GeoFlowEditorApp::OnNodeDetailViewPropertiesUpdate);
}

void GeoFlowEditorApp::OnGraphSelectionChanged(const FGraphPanelSelectionSet& selection)
{
	_selection = selection;
	//FOR NOW do first selection only -- you can do other stuff but this is the simplest
	for (UObject* obj : selection) {
		UGFN_E_Base* node = Cast<UGFN_E_Base>(obj);
		if (node != nullptr) {
			_selectedNodeDetailsView->SetObject(node);
			return;
		}
	}
	_selectedNodeDetailsView->SetObject(nullptr);
}

FReply GeoFlowEditorApp::OnGenerateClicked()
{
	ViewportClient->Generate();
	return FReply::Handled();
}

void GeoFlowEditorApp::OnClose()
{
	UpdateWorkingAssetFromGraph();
	_workingGraph->RemoveOnGraphChangedHandler(_graphChangeListenerHandle);
	_workingAsset->lastWorkingGraph = nullptr;
	FAssetEditorToolkit::OnClose();
}

void GeoFlowEditorApp::OnNodeDetailViewPropertiesUpdate(const FPropertyChangedEvent& event)
{
	if (_workingGraphUI != nullptr) {
		_workingGraphUI->NotifyGraphChanged();
	}
}

void GeoFlowEditorApp::OnGraphChanged(const FEdGraphEditAction& editAction)
{
	UpdateWorkingAssetFromGraph();
	RegisterGraphUpdateDelegates();
}


TSharedPtr<FUICommandList> GeoFlowEditorApp::GetGraphEditorCommands()
{
	if (!GraphEditorCommands.IsValid())
	{
		GraphEditorCommands = MakeShareable(new FUICommandList);
		GraphEditorCommands->MapAction(FGenericCommands::Get().Delete,
			FExecuteAction::CreateSP(this, &GeoFlowEditorApp::DeleteSelectedNodes)
		);
		GraphEditorCommands->MapAction(FGenericCommands::Get().Duplicate,
			FExecuteAction::CreateSP(this, &GeoFlowEditorApp::DuplicateSelectedNodes)
		);

	}
	return GraphEditorCommands;
}

void GeoFlowEditorApp::DeleteSelectedNodes()
{
	for (UObject* obj : _selection) {
		UGFN_E_Base* node = Cast<UGFN_E_Base>(obj);
		if (node == nullptr) continue;
		if (!node->CanUserDeleteNode()) continue;

		node->GetGraph()->RemoveNode(node);
	}
	UpdateWorkingAssetFromGraph();
}

void GeoFlowEditorApp::DuplicateSelectedNodes()
{
	//TODO
}

void GeoFlowEditorApp::UpdateWorkingAssetFromGraph()
{
	if (_workingAsset == nullptr || _workingGraph == nullptr) {
		return;
	}
	_workingAsset->SaveFromEditor(_workingGraph);
}

void GeoFlowEditorApp::UpdateEditorGraphFromWorkingAsset()
{
	if (_workingAsset->Graph == nullptr) {
		_workingGraph->GetSchema()->CreateDefaultNodesForGraph(*_workingGraph);
		_workingAsset->lastWorkingGraph = _workingGraph;
		
	}
	else {
		_workingAsset->LoadToEditor(_workingGraph);
	}
	RegisterGraphUpdateDelegates();
}

void GeoFlowEditorApp::RegisterGraphUpdateDelegates()
{
	TArray<UGFN_E_Base*> nodes;
	_workingGraph->GetNodesOfClass<UGFN_E_Base>(nodes);
	for (UGFN_E_Base* node : nodes) {
		node->regenDelegate.BindRaw(this,&GeoFlowEditorApp::UpdateWorkingAssetFromGraph);
	}
}



GeoFlowAppMode::GeoFlowAppMode(TSharedPtr<class GeoFlowEditorApp> app) : FApplicationMode(TEXT("GeoFlowEditorAppMode"))
{
	_app = app;
	_tabs.RegisterFactory(MakeShareable(new GeoFlowPrimaryTabFactory(app)));
	_tabs.RegisterFactory(MakeShareable(new GeoFlowPropertiesTabFactory(app)));
	_tabs.RegisterFactory(MakeShareable(new GeoFlowPreviewTabFactory(app)));

	TabLayout = FTabManager::NewLayout("GeoFlowLayout_TestVersion_3")
		->AddArea(
			FTabManager::NewPrimaryArea()->SetOrientation(Orient_Vertical)
			->Split(
				FTabManager::NewSplitter()
				->SetOrientation(Orient_Horizontal)
				->Split(
					FTabManager::NewStack()
					->SetSizeCoefficient(0.7)
					->AddTab(FName(TEXT("GeoFlowPrimaryTab")), ETabState::OpenedTab)
				)
				->Split(
					FTabManager::NewSplitter()
					->SetSizeCoefficient(0.3)
					->Split(
						FTabManager::NewStack()
						->SetSizeCoefficient(0.4)
						-> AddTab(FName(TEXT("GeoFlowPreviewTab")), ETabState::OpenedTab)
					)
					->Split(
						FTabManager::NewStack()
						->SetSizeCoefficient(0.6)
						->AddTab(FName(TEXT("GeoFlowPropertiesTab")), ETabState::OpenedTab)
					)

				)
			)
		);
}

void GeoFlowAppMode::RegisterTabFactories(TSharedPtr<class FTabManager> inTabManager)
{
	TSharedPtr<GeoFlowEditorApp> app = _app.Pin();
	app->PushTabFactories(_tabs);
	FApplicationMode::RegisterTabFactories(inTabManager);
}

void GeoFlowAppMode::PreDeactivateMode()
{
	FApplicationMode::PreDeactivateMode();
}

void GeoFlowAppMode::PostActivateMode()
{
	FApplicationMode::PostActivateMode();
}
