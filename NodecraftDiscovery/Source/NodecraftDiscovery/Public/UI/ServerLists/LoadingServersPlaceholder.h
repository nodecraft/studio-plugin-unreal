// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "LoadingServersPlaceholder.generated.h"

/**
 * This is used as a placeholder to indicate that we're loading servers.
 * For each of these that is in our list, we will show a glowing placeholder.
 */
UCLASS(BlueprintType)
class NODECRAFTDISCOVERY_API ULoadingServersPlaceholder : public UObject
{
	GENERATED_BODY()
};
