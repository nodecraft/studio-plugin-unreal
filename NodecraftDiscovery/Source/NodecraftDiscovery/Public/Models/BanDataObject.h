// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "PlayerDataObject.h"
#include "UObject/Object.h"
#include "Dom/JsonObject.h"
#include "BanDataObject.generated.h"

UENUM()
enum class EBanScope : uint8
{
	Server,
	Community,
	Game,
	Global
};

/**
 * 
 */
UCLASS()
class NODECRAFTDISCOVERY_API UBanDataObject : public UObject
{
	GENERATED_BODY()

public:
	FString GetId() const;
	EBanScope GetScope() const;
	FDateTime GetDateExpires() const;
	FDateTime GetDateCreated() const;
	UPlayerDataObject* GetAuthor() const;
	FText GetPublicReason() const;
	FText GetReason() const;

	static UBanDataObject* FromJson(const TSharedRef<FJsonObject> Json);

private:
	FString Id;
	EBanScope Scope = EBanScope::Server;
	FDateTime DateExpires;
	FDateTime DateCreated;

	// Public moderation reason category.
	FText PublicReason;

	// Only visible to the server owner & moderators. Documented reason for the kick or ban.
	FText Reason;

	UPROPERTY()
	UPlayerDataObject* Author;
};
