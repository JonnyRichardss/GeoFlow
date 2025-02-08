#include "GeoFlowNode.h"

#include "SGraphNode.h"

#include "BlueprintEditorSettings.h"
#include "BoneDragDropOp.h"
#include "Containers/EnumAsByte.h"
#include "Delegates/Delegate.h"
#include "DragAndDrop/ActorDragDropGraphEdOp.h"
#include "DragAndDrop/ActorDragDropOp.h"
#include "DragAndDrop/AssetDragDropOp.h"
#include "EdGraph/EdGraph.h"
#include "EdGraph/EdGraphNode.h"
#include "EdGraph/EdGraphPin.h"
#include "EdGraph/EdGraphSchema.h"
#include "EdGraphSchema_K2.h"
#include "Framework/Application/SlateApplication.h"
#include "Framework/Text/TextLayout.h"
#include "GameFramework/Actor.h"
#include "GenericPlatform/GenericApplication.h"
#include "GenericPlatform/ICursor.h"
#include "GraphEditorDragDropAction.h"
#include "GraphEditorSettings.h"
#include "HAL/PlatformCrt.h"
#include "IDocumentation.h"
#include "Input/DragAndDrop.h"
#include "Input/Events.h"
#include "InputCoreTypes.h"
#include "Internationalization/Internationalization.h"
#include "K2Node_Literal.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "Layout/ArrangedChildren.h"
#include "Layout/ArrangedWidget.h"
#include "Layout/Children.h"
#include "Layout/Geometry.h"
#include "Layout/Margin.h"
#include "Logging/TokenizedMessage.h"
#include "Math/UnrealMathSSE.h"
#include "Misc/AssertionMacros.h"
#include "Misc/Guid.h"
#include "Misc/Optional.h"
#include "NodeFactory.h"
#include "SCommentBubble.h"
#include "SGraphPanel.h"
#include "SGraphPin.h"
#include "SlateOptMacros.h"
#include "SlotBase.h"
#include "Templates/Casts.h"
#include "Templates/TypeHash.h"
#include "Textures/SlateIcon.h"
#include "TutorialMetaData.h"
#include "UObject/Class.h"
#include "UObject/Object.h"
#include "UObject/ObjectMacros.h"
#include "UObject/ObjectPtr.h"
#include "UObject/Package.h"
#include "UObject/UObjectGlobals.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Input/SCheckBox.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Layout/SSpacer.h"
#include "Widgets/Notifications/SErrorText.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/SNullWidget.h"
#include "Widgets/SToolTip.h"
#include "Widgets/SWidget.h"
#include "Widgets/Text/SInlineEditableTextBlock.h"
#include "Widgets/Text/STextBlock.h"

#include "Framework/Commands/UIAction.h"
#include "ToolMenu.h"
#include "SGraphNode.h"


//SGeoFlowNode -- flow direction stuff
void SGeoFlowNode::Construct(const FArguments& inArgs, UEdGraphNode* inGraphNodeObj)
{
	GraphNode = inGraphNodeObj;
	UpdateGraphNode();
}

TSharedRef<SWidget> SGeoFlowNode::CreateNodeContentArea()
{
	if (bDoHorizontal) {
		return CreateNodeContentAreaHorizontal();
	}
	else {
		return CreateNodeContentAreaVertical();
	}
}

TSharedRef<SWidget> SGeoFlowNode::CreateNodeContentAreaVertical()
{
	// NODE CONTENT AREA
	return SNew(SBorder)
		.BorderImage(FAppStyle::GetBrush("NoBorder"))
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		.Padding(FMargin(0, 3))
		[
			SNew(SVerticalBox)
				+ SVerticalBox::Slot()
				.VAlign(VAlign_Top)
				.FillHeight(1.0f)
				[
					// LEFT
					SAssignNew(TopNodeBox, SHorizontalBox)
				]
				+ SVerticalBox::Slot()
				.AutoHeight()
				.VAlign(VAlign_Bottom)
				[
					// RIGHT
					SAssignNew(BottomNodeBox, SHorizontalBox)
				]
		];
}

TSharedRef<SWidget> SGeoFlowNode::CreateNodeContentAreaHorizontal()
{
	// NODE CONTENT AREA
	return SNew(SBorder)
		.BorderImage(FAppStyle::GetBrush("NoBorder"))
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		.Padding(FMargin(0, 3))
		[
			SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.HAlign(HAlign_Left)
				.FillWidth(1.0f)
				[
					// LEFT
					SAssignNew(LeftNodeBox, SVerticalBox)
				]
				+ SHorizontalBox::Slot()
				.AutoWidth()
				.HAlign(HAlign_Right)
				[
					// RIGHT
					SAssignNew(RightNodeBox, SVerticalBox)
				]
		];
}

/**
 * Update this GraphNode to match the data that it is observing
 */
BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SGeoFlowNode::UpdateGraphNode() {
	if (bDoHorizontal) {
		SGraphNode::UpdateGraphNode();
	}
	else {
		UpdateGraphNodeVertical();
	}
}
void SGeoFlowNode::AddPin(const TSharedRef<SGraphPin>& PinToAdd) {
	if (bDoHorizontal) {
		SGraphNode::AddPin(PinToAdd);
	}
	else {
		AddPinVertical(PinToAdd);
	}
}

void SGeoFlowNode::UpdateGraphNodeVertical()
{
	//TODO change this to be uppy downy not lefty righty
	InputPins.Empty();
	OutputPins.Empty();

	// Reset variables that are going to be exposed, in case we are refreshing an already setup node.
	BottomNodeBox.Reset();
	TopNodeBox.Reset();

	//
	//             ______________________
	//            |      TITLE AREA      |
	//            +-------+------+-------+
	//            | (>) L |      | R (>) |
	//            | (>) E |      | I (>) |
	//            | (>) F |      | G (>) |
	//            | (>) T |      | H (>) |
	//            |       |      | T (>) |
	//            |_______|______|_______|
	//
	TSharedPtr<SHorizontalBox> MainHorizontalBox;
	SetupErrorReporting();

	TSharedPtr<SNodeTitle> NodeTitle = SNew(SNodeTitle, GraphNode);

	// Get node icon
	IconColor = FLinearColor::White;
	const FSlateBrush* IconBrush = nullptr;
	if (GraphNode != NULL && GraphNode->ShowPaletteIconOnNode())
	{
		IconBrush = GraphNode->GetIconAndTint(IconColor).GetOptionalIcon();
	}

	TSharedRef<SOverlay> DefaultTitleAreaWidget =
		SNew(SOverlay)
		+ SOverlay::Slot()
		[
			SNew(SImage)
				.Image(FAppStyle::GetBrush("Graph.Node.TitleGloss"))
				.ColorAndOpacity(this, &SGraphNode::GetNodeTitleIconColor)
		]
		+ SOverlay::Slot()
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Fill)
		[
			SNew(SVerticalBox)
				+ SVerticalBox::Slot()
				.VAlign(VAlign_Fill)
				[
					SNew(SBorder)
						.BorderImage(FAppStyle::GetBrush("Graph.Node.ColorSpill"))
						.Padding(TitleBorderMargin)
						.BorderBackgroundColor(this, &SGraphNode::GetNodeTitleColor)
						[
							SNew(SVerticalBox)
								+ SVerticalBox::Slot()
								.HAlign(HAlign_Left)
								.Padding(FMargin(0.f, 0.f, 4.f, 0.f))
								.AutoHeight()
								[
									SNew(SImage)
										.Image(IconBrush)
										.ColorAndOpacity(this, &SGraphNode::GetNodeTitleIconColor)
								]
								+ SVerticalBox::Slot()
								[
									SNew(SHorizontalBox)
										+ SHorizontalBox::Slot()
										.AutoWidth()
										[
											CreateTitleWidget(NodeTitle)
										]
										+ SHorizontalBox::Slot()
										.AutoWidth()
										[
											NodeTitle.ToSharedRef()
										]
								]
						]
				]
				+ SVerticalBox::Slot()
				.HAlign(HAlign_Center)
				.VAlign(VAlign_Bottom)
				.Padding(0, 0, 5, 0)
				.AutoHeight()
				[
					CreateTitleRightWidget()
				]
		]
		+ SOverlay::Slot()
		.HAlign(HAlign_Left)
		[
			SNew(SBorder)
				.Visibility(EVisibility::HitTestInvisible)
				.BorderImage(FAppStyle::GetBrush("Graph.Node.TitleHighlight"))
				.BorderBackgroundColor(this, &SGraphNode::GetNodeTitleIconColor)
				[
					SNew(SSpacer)
						.Size(FVector2D(20, 20))
				]
		];

	SetDefaultTitleAreaWidget(DefaultTitleAreaWidget);


	if (!SWidget::GetToolTip().IsValid())
	{
		TSharedRef<SToolTip> DefaultToolTip = IDocumentation::Get()->CreateToolTip(TAttribute< FText >(this, &SGraphNode::GetNodeTooltip), NULL, GraphNode->GetDocumentationLink(), GraphNode->GetDocumentationExcerptName());
		SetToolTip(DefaultToolTip);
	}

	// Setup a meta tag for this node
	FGraphNodeMetaData TagMeta(TEXT("Graphnode"));
	PopulateMetaTag(&TagMeta);

	TSharedPtr<SHorizontalBox> InnerHorizontalBox;
	this->ContentScale.Bind(this, &SGraphNode::GetContentScale);


	InnerHorizontalBox = SNew(SHorizontalBox)
		+ SHorizontalBox::Slot()
		.AutoWidth()
		.HAlign(HAlign_Left)
		.VAlign(VAlign_Fill)
		.Padding(Settings->GetNonPinNodeBodyPadding())
		+ SHorizontalBox::Slot()
		.AutoWidth()
		.HAlign(HAlign_Left)
		.VAlign(VAlign_Fill)
		[
			CreateNodeContentArea()
		];

	TSharedPtr<SWidget> EnabledStateWidget = GetEnabledStateWidget();
	if (EnabledStateWidget.IsValid())
	{
		InnerHorizontalBox->AddSlot()
			.AutoWidth()
			.HAlign(HAlign_Left)
			.VAlign(VAlign_Fill)
			.Padding(FMargin(2, 0))
			[
				EnabledStateWidget.ToSharedRef()
			];
	}

	InnerHorizontalBox->AddSlot()
		.AutoWidth()
		.Padding(Settings->GetNonPinNodeBodyPadding())
		[
			ErrorReporting->AsWidget()
		];

	InnerHorizontalBox->AddSlot()
		.AutoWidth()
		.Padding(Settings->GetNonPinNodeBodyPadding())
		[
			VisualWarningReporting->AsWidget()
		];


	this->GetOrAddSlot(ENodeZone::Center)
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		[
			SAssignNew(MainHorizontalBox, SHorizontalBox)
				+ SHorizontalBox::Slot()
				.AutoWidth()
				[
					SNew(SOverlay)
						.AddMetaData<FGraphNodeMetaData>(TagMeta)
						+ SOverlay::Slot()
						.Padding(Settings->GetNonPinNodeBodyPadding())
						[
							SNew(SImage)
								.Image(GetNodeBodyBrush())
								.ColorAndOpacity(this, &SGraphNode::GetNodeBodyColor)
						]
						+ SOverlay::Slot()
						[
							InnerHorizontalBox.ToSharedRef()
						]
				]
		];

	bool SupportsBubble = true;
	if (GraphNode != nullptr)
	{
		SupportsBubble = GraphNode->SupportsCommentBubble();
	}

	if (SupportsBubble)
	{
		// Create comment bubble
		TSharedPtr<SCommentBubble> CommentBubble;
		const FSlateColor CommentColor = GetDefault<UGraphEditorSettings>()->DefaultCommentNodeTitleColor;

		SAssignNew(CommentBubble, SCommentBubble)
			.GraphNode(GraphNode)
			.Text(this, &SGraphNode::GetNodeComment)
			.OnTextCommitted(this, &SGraphNode::OnCommentTextCommitted)
			.OnToggled(this, &SGraphNode::OnCommentBubbleToggled)
			.ColorAndOpacity(CommentColor)
			.AllowPinning(true)
			.EnableTitleBarBubble(true)
			.EnableBubbleCtrls(true)
			.GraphLOD(this, &SGraphNode::GetCurrentLOD)
			.IsGraphNodeHovered(this, &SGraphNode::IsHovered);

		GetOrAddSlot(ENodeZone::TopCenter)
			.SlotOffset(TAttribute<FVector2D>(CommentBubble.Get(), &SCommentBubble::GetOffset))
			.SlotSize(TAttribute<FVector2D>(CommentBubble.Get(), &SCommentBubble::GetSize))
			.AllowScaling(TAttribute<bool>(CommentBubble.Get(), &SCommentBubble::IsScalingAllowed))
			.VAlign(VAlign_Top)
			[
				CommentBubble.ToSharedRef()
			];
	}

	//CreateBelowWidgetControlsVert(MainHorizontalBox);
	CreatePinWidgets();
	//CreateInputSideAddButtonVert(TopNodeBox);
	//CreateOutputSideAddButtonVert(BottomNodeBox);
	//CreateBelowPinControlsVert(InnerVerticalBox);
	//CreateAdvancedViewArrowVert(InnerVerticalBox);
}
void SGeoFlowNode::UpdateGraphNodeHorizontal()
{
	InputPins.Empty();
	OutputPins.Empty();

	// Reset variables that are going to be exposed, in case we are refreshing an already setup node.
	RightNodeBox.Reset();
	LeftNodeBox.Reset();

	//
	//             ______________________
	//            |      TITLE AREA      |
	//            +-------+------+-------+
	//            | (>) L |      | R (>) |
	//            | (>) E |      | I (>) |
	//            | (>) F |      | G (>) |
	//            | (>) T |      | H (>) |
	//            |       |      | T (>) |
	//            |_______|______|_______|
	//
	TSharedPtr<SVerticalBox> MainVerticalBox;
	SetupErrorReporting();

	TSharedPtr<SNodeTitle> NodeTitle = SNew(SNodeTitle, GraphNode);

	// Get node icon
	IconColor = FLinearColor::White;
	const FSlateBrush* IconBrush = nullptr;
	if (GraphNode != NULL && GraphNode->ShowPaletteIconOnNode())
	{
		IconBrush = GraphNode->GetIconAndTint(IconColor).GetOptionalIcon();
	}

	TSharedRef<SOverlay> DefaultTitleAreaWidget =
		SNew(SOverlay)
		+ SOverlay::Slot()
		[
			SNew(SImage)
				.Image(FAppStyle::GetBrush("Graph.Node.TitleGloss"))
				.ColorAndOpacity(this, &SGraphNode::GetNodeTitleIconColor)
		]
		+ SOverlay::Slot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Center)
		[
			SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.HAlign(HAlign_Fill)
				[
					SNew(SBorder)
						.BorderImage(FAppStyle::GetBrush("Graph.Node.ColorSpill"))
						.Padding(TitleBorderMargin)
						.BorderBackgroundColor(this, &SGraphNode::GetNodeTitleColor)
						[
							SNew(SHorizontalBox)
								+ SHorizontalBox::Slot()
								.VAlign(VAlign_Top)
								.Padding(FMargin(0.f, 0.f, 4.f, 0.f))
								.AutoWidth()
								[
									SNew(SImage)
										.Image(IconBrush)
										.ColorAndOpacity(this, &SGraphNode::GetNodeTitleIconColor)
								]
								+ SHorizontalBox::Slot()
								[
									SNew(SVerticalBox)
										+ SVerticalBox::Slot()
										.AutoHeight()
										[
											CreateTitleWidget(NodeTitle)
										]
										+ SVerticalBox::Slot()
										.AutoHeight()
										[
											NodeTitle.ToSharedRef()
										]
								]
						]
				]
				+ SHorizontalBox::Slot()
				.HAlign(HAlign_Right)
				.VAlign(VAlign_Center)
				.Padding(0, 0, 5, 0)
				.AutoWidth()
				[
					CreateTitleRightWidget()
				]
		]
		+ SOverlay::Slot()
		.VAlign(VAlign_Top)
		[
			SNew(SBorder)
				.Visibility(EVisibility::HitTestInvisible)
				.BorderImage(FAppStyle::GetBrush("Graph.Node.TitleHighlight"))
				.BorderBackgroundColor(this, &SGraphNode::GetNodeTitleIconColor)
				[
					SNew(SSpacer)
						.Size(FVector2D(20, 20))
				]
		];

	SetDefaultTitleAreaWidget(DefaultTitleAreaWidget);


	if (!SWidget::GetToolTip().IsValid())
	{
		TSharedRef<SToolTip> DefaultToolTip = IDocumentation::Get()->CreateToolTip(TAttribute< FText >(this, &SGraphNode::GetNodeTooltip), NULL, GraphNode->GetDocumentationLink(), GraphNode->GetDocumentationExcerptName());
		SetToolTip(DefaultToolTip);
	}

	// Setup a meta tag for this node
	FGraphNodeMetaData TagMeta(TEXT("Graphnode"));
	PopulateMetaTag(&TagMeta);

	TSharedPtr<SVerticalBox> InnerVerticalBox;
	this->ContentScale.Bind(this, &SGraphNode::GetContentScale);


	InnerVerticalBox = SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.AutoHeight()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Top)
		.Padding(Settings->GetNonPinNodeBodyPadding())
		+ SVerticalBox::Slot()
		.AutoHeight()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Top)
		[
			CreateNodeContentArea()
		];

	TSharedPtr<SWidget> EnabledStateWidget = GetEnabledStateWidget();
	if (EnabledStateWidget.IsValid())
	{
		InnerVerticalBox->AddSlot()
			.AutoHeight()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Top)
			.Padding(FMargin(2, 0))
			[
				EnabledStateWidget.ToSharedRef()
			];
	}

	InnerVerticalBox->AddSlot()
		.AutoHeight()
		.Padding(Settings->GetNonPinNodeBodyPadding())
		[
			ErrorReporting->AsWidget()
		];

	InnerVerticalBox->AddSlot()
		.AutoHeight()
		.Padding(Settings->GetNonPinNodeBodyPadding())
		[
			VisualWarningReporting->AsWidget()
		];


	this->GetOrAddSlot(ENodeZone::Center)
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		[
			SAssignNew(MainVerticalBox, SVerticalBox)
				+ SVerticalBox::Slot()
				.AutoHeight()
				[
					SNew(SOverlay)
						.AddMetaData<FGraphNodeMetaData>(TagMeta)
						+ SOverlay::Slot()
						.Padding(Settings->GetNonPinNodeBodyPadding())
						[
							SNew(SImage)
								.Image(GetNodeBodyBrush())
								.ColorAndOpacity(this, &SGraphNode::GetNodeBodyColor)
						]
						+ SOverlay::Slot()
						[
							InnerVerticalBox.ToSharedRef()
						]
				]
		];

	bool SupportsBubble = true;
	if (GraphNode != nullptr)
	{
		SupportsBubble = GraphNode->SupportsCommentBubble();
	}

	if (SupportsBubble)
	{
		// Create comment bubble
		TSharedPtr<SCommentBubble> CommentBubble;
		const FSlateColor CommentColor = GetDefault<UGraphEditorSettings>()->DefaultCommentNodeTitleColor;

		SAssignNew(CommentBubble, SCommentBubble)
			.GraphNode(GraphNode)
			.Text(this, &SGraphNode::GetNodeComment)
			.OnTextCommitted(this, &SGraphNode::OnCommentTextCommitted)
			.OnToggled(this, &SGraphNode::OnCommentBubbleToggled)
			.ColorAndOpacity(CommentColor)
			.AllowPinning(true)
			.EnableTitleBarBubble(true)
			.EnableBubbleCtrls(true)
			.GraphLOD(this, &SGraphNode::GetCurrentLOD)
			.IsGraphNodeHovered(this, &SGraphNode::IsHovered);

		GetOrAddSlot(ENodeZone::TopCenter)
			.SlotOffset(TAttribute<FVector2D>(CommentBubble.Get(), &SCommentBubble::GetOffset))
			.SlotSize(TAttribute<FVector2D>(CommentBubble.Get(), &SCommentBubble::GetSize))
			.AllowScaling(TAttribute<bool>(CommentBubble.Get(), &SCommentBubble::IsScalingAllowed))
			.VAlign(VAlign_Top)
			[
				CommentBubble.ToSharedRef()
			];
	}

	//CreateBelowWidgetControls(MainVerticalBox);
	CreatePinWidgets();
	//CreateInputSideAddButton(LeftNodeBox);
	//CreateOutputSideAddButton(RightNodeBox);
	//CreateBelowPinControls(InnerVerticalBox);
	//CreateAdvancedViewArrow(InnerVerticalBox);
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SGeoFlowNode::SetOwner(const TSharedRef<SGraphPanel>& OwnerPanel)
{
	check(!OwnerGraphPanelPtr.IsValid());
	SetParentPanel(OwnerPanel);
	OwnerGraphPanelPtr = OwnerPanel;
	GraphNode->DEPRECATED_NodeWidget = SharedThis(this);
	if (bDoHorizontal) {
		/*Once we have an owner, and if hide Unused pins is enabled, we need to remake our pins to drop the hidden ones*/
		if (OwnerGraphPanelPtr.Pin()->GetPinVisibility() != SGraphEditor::Pin_Show
			&& LeftNodeBox.IsValid()
			&& RightNodeBox.IsValid())
		{
			this->LeftNodeBox->ClearChildren();
			this->RightNodeBox->ClearChildren();
			CreatePinWidgets();
		}
	}
	else {
	/*Once we have an owner, and if hide Unused pins is enabled, we need to remake our pins to drop the hidden ones*/
	if (OwnerGraphPanelPtr.Pin()->GetPinVisibility() != SGraphEditor::Pin_Show
			&& TopNodeBox.IsValid()
			&& BottomNodeBox.IsValid())
		{
			this->TopNodeBox->ClearChildren();
			this->BottomNodeBox->ClearChildren();
			CreatePinWidgets();
		}


	}

	for (TSharedRef<SGraphPin> Pin : InputPins)
	{
		Pin->RefreshLOD();
	}
	for (TSharedRef<SGraphPin> Pin : OutputPins)
	{
		Pin->RefreshLOD();
	}

}
FSlateColor SGeoFlowNode::GetCommentColor() const { return FLinearColor::Red; }

TSharedPtr<SGraphNode> FGeoFlowNodeFactory::CreateNode(UEdGraphNode* node) const
{
	if (true) {
		return SNew(SGeoFlowNode, node);
	}
	else {
		return nullptr;
	}
}

void SGeoFlowNode::AddPinVertical(const TSharedRef<SGraphPin>& PinToAdd)
{
	PinToAdd->SetOwner(SharedThis(this));

	const UEdGraphPin* PinObj = PinToAdd->GetPinObj();
	const bool bAdvancedParameter = (PinObj != nullptr) && PinObj->bAdvancedView;
	if (bAdvancedParameter)
	{
		PinToAdd->SetVisibility(TAttribute<EVisibility>(PinToAdd, &SGraphPin::IsPinVisibleAsAdvanced));
	}

	if (PinToAdd->GetDirection() == EEdGraphPinDirection::EGPD_Input)
	{
		TopNodeBox->AddSlot()
			.AutoWidth()
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Top)
			.Padding(Settings->GetInputPinPadding())
			[
				PinToAdd
			];
		InputPins.Add(PinToAdd);
	}
	else // Direction == EEdGraphPinDirection::EGPD_Output
	{
		BottomNodeBox->AddSlot()
			.AutoWidth()
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Bottom)
			.Padding(Settings->GetOutputPinPadding())
			[
				PinToAdd
			];
		OutputPins.Add(PinToAdd);
	}
}

void SGeoFlowNode::AddPinHorizontal(const TSharedRef<SGraphPin>& PinToAdd)
{
	PinToAdd->SetOwner(SharedThis(this));

	const UEdGraphPin* PinObj = PinToAdd->GetPinObj();
	const bool bAdvancedParameter = (PinObj != nullptr) && PinObj->bAdvancedView;
	if (bAdvancedParameter)
	{
		PinToAdd->SetVisibility(TAttribute<EVisibility>(PinToAdd, &SGraphPin::IsPinVisibleAsAdvanced));
	}

	if (PinToAdd->GetDirection() == EEdGraphPinDirection::EGPD_Input)
	{
		LeftNodeBox->AddSlot()
			.AutoHeight()
			.HAlign(HAlign_Left)
			.VAlign(VAlign_Center)
			.Padding(Settings->GetInputPinPadding())
			[
				PinToAdd
			];
		InputPins.Add(PinToAdd);
	}
	else // Direction == EEdGraphPinDirection::EGPD_Output
	{
		RightNodeBox->AddSlot()
			.AutoHeight()
			.HAlign(HAlign_Right)
			.VAlign(VAlign_Center)
			.Padding(Settings->GetOutputPinPadding())
			[
				PinToAdd
			];
		OutputPins.Add(PinToAdd);
	}
}

