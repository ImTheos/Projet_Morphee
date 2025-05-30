// Copyright https://github.com/MothCocoon/FlowGraph/graphs/contributors

#pragma once

#include "GraphEditor.h"
#include "Widgets/DeclarativeSyntaxSupport.h"

#include "FlowGraph.h"

class FFlowAssetEditor;
class IDetailsView;
class UFlowDebuggerSubsystem;

/**
 *
 */
class FLOWEDITOR_API SFlowGraphEditor : public SGraphEditor
{
public:
	SLATE_BEGIN_ARGS(SFlowGraphEditor)
		{
		}

		SLATE_ARGUMENT(FGraphEditorEvents, GraphEvents)
		SLATE_ARGUMENT(TSharedPtr<IDetailsView>, DetailsView)
	SLATE_END_ARGS()

protected:
	TWeakObjectPtr<UFlowAsset> FlowAsset;

	TWeakPtr<FFlowAssetEditor> FlowAssetEditor;
	TSharedPtr<IDetailsView> DetailsView;
	TSharedPtr<FUICommandList> CommandList;

	TWeakObjectPtr<UFlowDebuggerSubsystem> DebuggerSubsystem;

public:
	void Construct(const FArguments& InArgs, const TSharedPtr<FFlowAssetEditor> InAssetEditor);

	virtual void BindGraphCommands();

	virtual FGraphAppearanceInfo GetGraphAppearanceInfo() const;
	virtual FText GetCornerText() const;

private:
	static void UndoGraphAction();
	static void RedoGraphAction();

	static FReply OnSpawnGraphNodeByShortcut(FInputChord InChord, const FVector2D& InPosition, UEdGraph* InGraph);
	void OnCreateComment() const;

public:
	virtual bool IsTabFocused() const;
	
	static bool CanEdit();
	static bool IsPIE();
	static bool IsPlaySessionPaused();

	virtual void SelectSingleNode(UEdGraphNode* Node);

protected:
	virtual void OnSelectedNodesChanged(const TSet<UObject*>& Nodes);

public:
	FOnSelectionChanged OnSelectionChangedEvent;

	TSet<UFlowGraphNode*> GetSelectedFlowNodes() const;

protected:
	virtual bool CanSelectAllNodes() const { return true; }

	static void ReconnectExecPins(const UFlowGraphNode* Node);
	virtual void DeleteSelectedNodes();
	virtual void DeleteSelectedDuplicableNodes();
	virtual bool CanDeleteNodes() const;

	virtual void CopySelectedNodes() const;
	static void PrepareFlowGraphNodeForCopy(UFlowGraphNode& FlowGraphNode, const int32 ParentEdNodeIndex, FGraphPanelSelectionSet& NewSelectedNodes);
	virtual bool CanCopyNodes() const;

	virtual void CutSelectedNodes();
	virtual bool CanCutNodes() const;

	virtual void PasteNodes();

	static bool CanPasteNodesAsSubNodes(const TSet<UEdGraphNode*>& NodesToPaste, const UFlowGraphNode& PasteTargetNode);
	static TSet<UEdGraphNode*> ImportNodesToPasteFromClipboard(UFlowGraph& FlowGraph, FString& OutTextToImport);
	TArray<UFlowGraphNode*> DerivePasteTargetNodesFromSelectedNodes() const;

public:
	virtual void PasteNodesHere(const FVector2D& Location);
	virtual bool CanPasteNodes() const;

protected:
	virtual void DuplicateNodes();
	virtual bool CanDuplicateNodes() const;

	virtual void OnNodeDoubleClicked(class UEdGraphNode* Node) const;
	virtual void OnNodeTitleCommitted(const FText& NewText, ETextCommit::Type CommitInfo, UEdGraphNode* NodeBeingChanged);

	virtual void ReconstructNode() const;
	virtual bool CanReconstructNode() const;

private:
	void AddInput() const;
	bool CanAddInput() const;

	void AddOutput() const;
	bool CanAddOutput() const;

	void RemovePin();
	bool CanRemovePin();

	void OnAddBreakpoint() const;
	void OnAddPinBreakpoint();

	bool CanAddBreakpoint() const;
	bool CanAddPinBreakpoint();

	void OnRemoveBreakpoint() const;
	void OnRemovePinBreakpoint();

	bool CanRemoveBreakpoint() const;
	bool CanRemovePinBreakpoint();

	void OnEnableBreakpoint() const;
	void OnEnablePinBreakpoint();

	bool CanEnableBreakpoint() const;
	bool CanEnablePinBreakpoint();

	void OnDisableBreakpoint() const;
	void OnDisablePinBreakpoint();

	bool CanDisableBreakpoint() const;
	bool CanDisablePinBreakpoint();

	void OnToggleBreakpoint() const;
	void OnTogglePinBreakpoint();

	bool CanToggleBreakpoint() const;
	bool CanTogglePinBreakpoint();

	void SetSignalMode(const EFlowSignalMode Mode) const;
	bool CanSetSignalMode(const EFlowSignalMode Mode) const;

	void OnForcePinActivation();

	void FocusViewport() const;
	bool CanFocusViewport() const;

	void JumpToNodeDefinition() const;
	bool CanJumpToNodeDefinition() const;
};