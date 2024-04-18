// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "CreateServerPlaceholder.generated.h"

/**
 * This is used to indicate that we want to show a create server button at this index
 * when we're showing lists of servers or animated placeholders
 */
UCLASS(BlueprintType)
class NODECRAFTDISCOVERY_API UCreateServerPlaceholder : public UObject
{
	GENERATED_BODY()
};
