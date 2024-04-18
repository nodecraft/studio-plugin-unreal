// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#include "NodecraftAuthCommon.h"

#define LOCTEXT_NAMESPACE "FNodecraftAuthCommonModule"

void FNodecraftAuthCommonModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FNodecraftAuthCommonModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FNodecraftAuthCommonModule, NodecraftAuthCommon)