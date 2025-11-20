// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "NoServerPlaceholder.generated.h"

/**
 * This is used as a placeholder to show that there is no server to display.
 * We need this for display purposes to ensure correct sizing of items that are present.
 */
UCLASS(BlueprintType)
class NODECRAFTDISCOVERY_API UNoServerPlaceholder : public UObject
{
	GENERATED_BODY()
};
