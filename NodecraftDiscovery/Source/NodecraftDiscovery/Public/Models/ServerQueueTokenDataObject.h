// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Dom/JsonObject.h"
#include "ServerQueueTokenDataObject.generated.h"

/**
 * 
 */
UCLASS()
class NODECRAFTDISCOVERY_API UServerQueueTokenDataObject : public UObject
{
	GENERATED_BODY()

public:
	static UServerQueueTokenDataObject* FromJson(const TSharedRef<FJsonObject>& Json);

	FString GetToken() const;
	FDateTime GetQueueExpires() const;
	FDateTime GetQueueRenews() const;

protected:
	FString Token;
	FDateTime QueueExpires;
	FDateTime QueueRenews;
};
