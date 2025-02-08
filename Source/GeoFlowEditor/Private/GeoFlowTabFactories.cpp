#include "GeoFlowTabFactories.h"
#include "GeoFlowApp.h"
#include "GeoFlowAsset.h"
#include "IDetailsView.h"
#include "PropertyEditorModule.h"
#include "GraphEditor.h"
#include "Editor/UnrealEd/Public/Kismet2/BlueprintEditorUtils.h"
#include "Kismet2/KismetEditorUtilities.h"


GeoFlowPrimaryTabFactory::GeoFlowPrimaryTabFactory(TSharedPtr<class GeoFlowEditorApp> app) : FWorkflowTabFactory(FName("GeoFlowPrimaryTab"), app)
{
	_app = app;
	TabLabel = FText::FromString(TEXT("Main"));
	ViewMenuDescription = FText::FromString(TEXT("Jonny wuz here"));
	ViewMenuTooltip = FText::FromString(TEXT("Jonny wuz here too"));
}

TSharedRef<SWidget> GeoFlowPrimaryTabFactory::CreateTabBody(const FWorkflowTabSpawnInfo& info) const
{
	TSharedPtr<GeoFlowEditorApp> app = _app.Pin();

	SGraphEditor::FGraphEditorEvents events;
	events.OnSelectionChanged.BindRaw(app.Get(), &GeoFlowEditorApp::OnGraphSelectionChanged);

	TSharedPtr<SGraphEditor> graphEditor =
		SNew(SGraphEditor)
		.AdditionalCommands(app->GetGraphEditorCommands())
		.IsEditable(true)
		.GraphEvents(events)
		.GraphToEdit(app->GetWorkingGraph());
	app->SetWorkingGraphUi(graphEditor);
	return SNew(SVerticalBox) + SVerticalBox::Slot()
		.FillHeight(1.0f)
		.HAlign(HAlign_Fill)
		[
			graphEditor.ToSharedRef()
		];

}

FText GeoFlowPrimaryTabFactory::GetTabToolTipText(const FWorkflowTabSpawnInfo& info) const
{
	return FText::FromString(TEXT("Jonny wuz ere mk4 (the best one)"));
}

GeoFlowPropertiesTabFactory::GeoFlowPropertiesTabFactory(TSharedPtr<class GeoFlowEditorApp> app) : FWorkflowTabFactory(FName("GeoFlowPropertiesTab"), app)
{
	_app = app;
	TabLabel = FText::FromString(TEXT("Graph Properties"));
	ViewMenuDescription = FText::FromString(TEXT("Tab for global graph data or node properties"));
	ViewMenuTooltip = FText::FromString(TEXT("Jonny wuz here"));
}

TSharedRef<SWidget> GeoFlowPropertiesTabFactory::CreateTabBody(const FWorkflowTabSpawnInfo& info) const
{
	TSharedPtr<GeoFlowEditorApp> app = _app.Pin();
	FPropertyEditorModule& propertyEditorModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>(TEXT("PropertyEditor"));

	FDetailsViewArgs args;
	{
		args.bAllowSearch = false;
		args.bHideSelectionTip = true;
		args.bLockable = false;
		args.bSearchInitialKeyFocus = true;
		args.bUpdatesFromSelection = false;
		args.NotifyHook = nullptr;
		args.bShowOptions = true;
		args.bShowModifiedPropertiesOption = false;
		args.bShowScrollBar = false;
	}
	TSharedPtr<IDetailsView> graphDetailsView = propertyEditorModule.CreateDetailView(args);
	graphDetailsView->SetObject(app->GetWorkingAsset());
	
	TSharedPtr<IDetailsView> nodeDetailsView = propertyEditorModule.CreateDetailView(args);
	nodeDetailsView->SetObject(nullptr);
	app->SetSelectedNodeDetailView(nodeDetailsView);

	return SNew(SVerticalBox) 
		+ SVerticalBox::Slot()
		.FillHeight(0.25f)
		.HAlign(HAlign_Fill)
		[
			nodeDetailsView.ToSharedRef()
		]
		+ SVerticalBox::Slot()
		.FillHeight(0.75f)
		.HAlign(HAlign_Fill)
		[
			graphDetailsView.ToSharedRef()
		]
		;
}

FText GeoFlowPropertiesTabFactory::GetTabToolTipText(const FWorkflowTabSpawnInfo& info) const
{
	return FText::FromString(TEXT("Jonny wuz here"));
}
