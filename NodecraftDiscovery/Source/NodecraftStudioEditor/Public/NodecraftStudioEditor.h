// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once


#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "Engine/Engine.h"
#include "Modules/ModuleInterface.h"

DECLARE_LOG_CATEGORY_EXTERN(NodecraftStudioEditor, All, All)

class FNodecraftStudioEditorModule: public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

};