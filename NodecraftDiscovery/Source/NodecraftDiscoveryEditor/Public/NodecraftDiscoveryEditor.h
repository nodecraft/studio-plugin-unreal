#pragma once


#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "Engine.h"
#include "Modules/ModuleInterface.h"
#include "UnrealEd.h"

#define DISCOVERY_API_DEMO_AUTH_CODE "DEV01234"
#define DISCOVERY_API_DEMO_ACCESS_TOKEN_GAME "public_123-this-is-a-test"

DECLARE_LOG_CATEGORY_EXTERN(NodecraftDiscoveryEditor, All, All)

class FNodecraftDiscoveryEditorModule: public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

};