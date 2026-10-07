#include "Projet_Morphee_Editor.h"

#include "DialogueDetailsCustomization.h"
#include "GameLogic/Dialogue/FlowGraph/Nodes/PlayDialog.h"

#define LOCTEXT_NAMESPACE "FProjet_Morphee_EditorModule"

void FProjet_Morphee_EditorModule::StartupModule()
{
	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");

	PropertyModule.RegisterCustomClassLayout(
		UPlayDialog::StaticClass()->GetFName(),
		FOnGetDetailCustomizationInstance::CreateStatic(&FDialogueDetailsCustomization::MakeInstance)
	);

	PropertyModule.NotifyCustomizationModuleChanged();
}

void FProjet_Morphee_EditorModule::ShutdownModule()
{
	if (FModuleManager::Get().IsModuleLoaded("PropertyEditor"))
	{
		FPropertyEditorModule& PropertyModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
		PropertyModule.UnregisterCustomClassLayout(UPlayDialog::StaticClass()->GetFName());
	}
}

#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FProjet_Morphee_EditorModule, Projet_Morphee_Editor)