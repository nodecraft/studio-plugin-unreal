#pragma once


#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "Engine.h"
#include "Modules/ModuleInterface.h"
#include "UnrealEd.h"

DECLARE_LOG_CATEGORY_EXTERN(NodecraftStudioEditor, All, All)

class FNodecraftStudioEditorModule: public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

};