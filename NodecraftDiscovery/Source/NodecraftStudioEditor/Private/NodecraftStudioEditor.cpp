#include "NodecraftStudioEditor.h"

#include "API/NodecraftStudioSessionManagerSubsystem.h"
#include "DeveloperSettings/NodecraftStudioApiSettings.h"
#include "Modules/ModuleManager.h"
#include "Modules/ModuleInterface.h"

IMPLEMENT_MODULE(FDefaultModuleImpl, NodecraftStudioEditor);

DEFINE_LOG_CATEGORY(NodecraftStudioEditor)

#define LOCTEXT_NAMESPACE "NodecraftStudioEditor"

void FNodecraftStudioEditorModule::StartupModule()
{
	// TODO: Set info on session manager subsystem here
	UNodecraftStudioSessionManager& Session = UNodecraftStudioSessionManager::Get();
	UE_LOG(NodecraftStudioEditor, Warning, TEXT("NodecraftStudioEditor: Log Started"));
}

void FNodecraftStudioEditorModule::ShutdownModule()
{
	UE_LOG(NodecraftStudioEditor, Warning, TEXT("NodecraftStudioEditor: Log Ended"));
}

#undef LOCTEXT_NAMESPACE