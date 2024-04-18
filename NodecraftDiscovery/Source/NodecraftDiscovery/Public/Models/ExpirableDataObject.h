// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "ExpirableDataObject.generated.h"

/**
 * 
 */
USTRUCT()
struct NODECRAFTDISCOVERY_API FExpirableDataObject
{
	GENERATED_BODY()

private:
	FString Id;
	FDateTime DateExpires;
	bool bExpires = false;

public:
	FString GetId() const;
	FDateTime GetDateExpires() const;
	bool Expires() const;

	static FExpirableDataObject FromJson(const TSharedRef<FJsonObject>& Json);

};

typedef FExpirableDataObject FServerAllowDataObject;
typedef FExpirableDataObject FServerInviteDataObject;
