// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "PlayerDataObject.generated.h"

enum class EIdentityType : uint8;
/**
 * 
 */
UCLASS()
class NODECRAFTDISCOVERY_API UPlayerDataObject : public UObject
{
	GENERATED_BODY()

public:
	FString GetId() const;
	FString GetUsername() const;
	FString GetIdent() const;
	EIdentityType GetIdentType() const;
	FString GetIdentTypeRaw() const;
	FString GetImageAvatarUrl() const;
	FString GetImageAvatarFilenameForCache() const;
	bool IsOnline() const;
	FDateTime GetDateStart() const;
	FDateTime GetDateEnd() const;
	FDateTime GetDateFirstJoined() const;
	// Return the total playtime in hours
	float GetPlaytime() const;

public:
	static UPlayerDataObject* FromJson(const TSharedRef<FJsonObject> Json);
	
private:
	FString Id;
	FString Username;
	
	// Platform Player ID used to authenticate the player. Differs by platform.
	FString Ident;
	
	// Platform used to authenticate the player
	FString IdentType;
	
	FString ImageUrl;
	
	// Used in context of user on server. will not be set simultaneously.
	// DateStart is for online players, DateEnd is for recent players.
	FString DateStart;
	FString DateEnd;

	// Used in the context of getting Server Player Details
	// Total playtime in hours
	float Playtime;

	FString DateFirstJoined;
	// End of Server Player Details
};
