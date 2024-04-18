// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Services/IdentService.h"
#include "NodecraftStudioSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class NODECRAFTDISCOVERY_API UNodecraftStudioSaveGame : public USaveGame
{
	GENERATED_BODY()

	UPROPERTY()
	FString GameAccessToken;

public:
	FString GetGameAccessToken() const;
	void SetGameAccessToken(const FString& InGameAccessToken);
	FString GetPlayerAccessToken() const;
	void SetPlayerAccessToken(const FString& InPlayerAccessToken);
	FPlayerSession GetPlayerSession() const;
	void SetPlayerSession(const FPlayerSession& InPlayerSession);

private:
	UPROPERTY()
	FString PlayerAccessToken;

	UPROPERTY()
	FPlayerSession PlayerSession = FPlayerSession();
};
