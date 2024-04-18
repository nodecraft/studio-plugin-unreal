// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "NodecraftPlayer.generated.h"

// This class does not need to be modified.
UINTERFACE()
class NODECRAFTCOMMON_API UNodecraftPlayer : public UInterface
{
	GENERATED_BODY()
};

/**
 * Add this interface to any APlayerController that remote players use when connecting to and while playing
 * on your game’s servers. To avoid reproducing code across multiple player controllers,
 * it is recommended that you use some kind of BasePlayerController which implements this interface.
 */
class NODECRAFTCOMMON_API INodecraftPlayer
{
	GENERATED_BODY()

public:
	UFUNCTION()
	virtual FString GetJoinToken() = 0;
	
	UFUNCTION()
	virtual FString GetSessionId() = 0;

	UFUNCTION()
	virtual FString GetAnalyticsSessionId() = 0;

	UFUNCTION()
	virtual FString GetPlayerID() = 0;

	UFUNCTION()
	virtual FString GetIpAddress() = 0;

	UFUNCTION()
	virtual void SetIpAddress(FString InIP) = 0;

	// This is called when the player logs into the server and pre-validation
	UFUNCTION()
	virtual void SetJoinToken(FString String) = 0;
	
	// These will get called by the server when the player joins the server and is validated
	UFUNCTION()
	virtual void SetSessionId(FString String) = 0;
	UFUNCTION()
	virtual void SetAnalyticsSessionId(FString String) = 0;
	UFUNCTION()
	virtual void SetPlayerID(const FString& String) = 0;
	// End of server calls
};
