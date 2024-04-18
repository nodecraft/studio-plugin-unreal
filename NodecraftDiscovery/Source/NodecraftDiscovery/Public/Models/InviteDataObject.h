// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "FriendDataObject.h"
#include "UObject/Object.h"
#include "InviteDataObject.generated.h"

class UPlayerDataObject;
class UServerDataObject;
enum class ENodecraftInviteStatus : uint8;
/**
 * 
 */
UCLASS()
class NODECRAFTDISCOVERY_API UInviteDataObject : public UObject
{
	GENERATED_BODY()

public:
	static UInviteDataObject* FromJson(const TSharedRef<FJsonObject>& Json);

protected:
	FString ID;
	FDateTime DateCreated;
	FDateTime DateExpires;
	ENodecraftInviteStatus Status;

	UPROPERTY()
	UServerDataObject* Server;

	UPROPERTY()
	UPlayerDataObject* Player;
};


