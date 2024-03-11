#include "NodecraftDiscoveryEditor.h"

#include "API/DiscoverySessionManagerSubsystem.h"
#include "DeveloperSettings/DiscoveryAPISettings.h"
#include "Modules/ModuleManager.h"
#include "Modules/ModuleInterface.h"

IMPLEMENT_MODULE(FDefaultModuleImpl, NodecraftDiscoveryEditor);

DEFINE_LOG_CATEGORY(NodecraftDiscoveryEditor)

#define LOCTEXT_NAMESPACE "NodecraftDiscoveryEditor"

void FNodecraftDiscoveryEditorModule::StartupModule()
{
	// TODO: Set info on session manager subsystem here
	UDiscoverySessionManager& Session = UDiscoverySessionManager::Get();
	UE_LOG(NodecraftDiscoveryEditor, Warning, TEXT("NodecraftDiscoveryEditor: Log Started"));
}

void FNodecraftDiscoveryEditorModule::ShutdownModule()
{
	UE_LOG(NodecraftDiscoveryEditor, Warning, TEXT("NodecraftDiscoveryEditor: Log Ended"));
}

#undef LOCTEXT_NAMESPACE