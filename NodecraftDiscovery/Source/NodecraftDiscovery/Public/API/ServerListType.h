// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ServerListType.generated.h"

/**
 * 
 */
UENUM()
enum class EServerListType : uint8
{
	Popular,
	Favorite,
	Recommended,
	Owned
};
