// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#include "NodecraftSteamAuth.h"

#define LOCTEXT_NAMESPACE "FNodecraftSteamAuthModule"

void FNodecraftSteamAuthModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FNodecraftSteamAuthModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FNodecraftSteamAuthModule, SteamAuth)