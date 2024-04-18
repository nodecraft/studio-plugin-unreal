// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "PlayerDataObject.h"
#include "UObject/Object.h"
#include "KickDataObject.generated.h"

USTRUCT(BlueprintType)
struct NODECRAFTDISCOVERY_API FPlayerKickDataObject
{
	GENERATED_BODY()

	// UUID of the kick
	UPROPERTY(BlueprintReadOnly)
	FString Id;

	// Date when the kick was created.
	// Max not exist
	UPROPERTY(BlueprintReadOnly)
	FDateTime DateCreated = FDateTime();

	// Only visible to the server owner & moderators. Documented reason for the kick or ban.
	// May not exist
	UPROPERTY(BlueprintReadOnly)
	FText Reason;

	// The player who was kicked
	UPROPERTY(BlueprintReadOnly)
	UPlayerDataObject* Player = nullptr;

	// Only visible to the server owner & moderators.
	// Owner or Moderator who performed the kick or ban.
	// When null, it is a system or game issued ban.
	UPROPERTY(BlueprintReadOnly)
	UPlayerDataObject* Author = nullptr;

	FString GetKickedPlayerId() const;
	
	static FPlayerKickDataObject FromJson(const TSharedRef<FJsonObject>& Json);
};
