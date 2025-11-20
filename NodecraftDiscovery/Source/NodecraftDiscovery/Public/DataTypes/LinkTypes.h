// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "LinkTypes.generated.h"

UENUM()
enum class ELinkType : uint8
{
	Undefined,
	Internal,
	External,
	COUNT UMETA(Hidden)
};

