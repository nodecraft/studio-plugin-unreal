// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "AllowStatus.generated.h"

UENUM()
enum class EAllowStatus : uint8
{
	Pending,
	Accepted,
	Declined,
	Revoked
};
