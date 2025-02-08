#pragma once
#include "SGraphNode.h"
#include "EdGraphUtilities.h"
#include "EdGraph/EdGraphNode.h"

class SGeoFlowNode : public SGraphNode {
public:
	SLATE_BEGIN_ARGS(SGeoFlowNode) {}
	SLATE_END_ARGS()
	bool bDoHorizontal = true;
	void Construct(const FArguments& inArgs, UEdGraphNode* inGraphNodeObj);
	virtual TSharedRef<SWidget> CreateNodeContentArea() override;
	TSharedRef<SWidget> CreateNodeContentAreaVertical();
	TSharedRef<SWidget> CreateNodeContentAreaHorizontal();
	virtual void UpdateGraphNode() override;
	void UpdateGraphNodeVertical();
	void UpdateGraphNodeHorizontal();
	virtual void AddPin(const TSharedRef<SGraphPin>& PinToAdd) override;
	void AddPinHorizontal(const TSharedRef<SGraphPin>& PinToAdd);
	void AddPinVertical(const TSharedRef<SGraphPin>& PinToAdd);
	virtual void SetOwner(const TSharedRef<SGraphPanel>& OwnerPanel) override;
protected:

	/** The area where input pins reside */
	TSharedPtr<SHorizontalBox> TopNodeBox;
	/** The area where output pins reside */
	TSharedPtr<SHorizontalBox> BottomNodeBox;
	virtual FSlateColor GetCommentColor() const override;
};

struct FGeoFlowNodeFactory : public FGraphPanelNodeFactory {
public:
	virtual ~FGeoFlowNodeFactory() {}
	virtual TSharedPtr<SGraphNode> CreateNode(UEdGraphNode* node) const override;
};