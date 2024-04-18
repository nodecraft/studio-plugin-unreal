// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "NodecraftCommon/NodecraftDefines.h"
#include "NodecraftGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class NODECRAFTGAME_API ANodecraftGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
#if NC_USE_NODECRAFT_ON_SERVER

protected:
	virtual void OnPostLogin(AController* NewPlayer) override;

public:
	virtual APlayerController* Login(UPlayer* NewPlayer, ENetRole InRemoteRole, const FString& Portal, const FString& Options, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;
	virtual void PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;

	virtual void BeginPlay() override;
	virtual void ProcessServerTravel(const FString& URL, bool bAbsolute) override;

	virtual void StartToLeaveMap() override;

private:
	// While this is true, any player leave events will be marked as "reconnecting", which will
	// prevent their session token from being automatically invalidated.
	bool bPlayersLeavingServerCanReconnect;
#endif // NC_USE_NODECRAFT_ON_SERVER

};
