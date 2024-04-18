// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "InviteStatus.generated.h"

UENUM()
enum class ENodecraftInviteStatus : uint8
{
	Pending,
	Accepted,
	Declined,
	Revoked
};
