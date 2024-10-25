// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DataTypes/LinkTypes.h"
#include "DataTypes/PlayerConnectionSubjects.h"
#include "Subsystems/EngineSubsystem.h"
#include "Engine/Engine.h"
#include "LinksService.generated.h"

class UPlayerConnectionDataObject;

DECLARE_DELEGATE_ThreeParams(FOnPlayerConnection, UPlayerConnectionDataObject* /*PlayerConnection*/, bool /*bSuccess*/, TOptional<FText> /*Error*/);
/**
 * 
 */
UCLASS()
class NODECRAFTDISCOVERY_API ULinksService : public UEngineSubsystem
{
	GENERATED_BODY()

public:
	static ULinksService& Get() { return *GEngine->GetEngineSubsystem<ULinksService>(); }
	
	void OpenURLInBrowser(ELinkType LinkType, FString URL);

	bool CreatePlayerConnection(EPlayerConnectionSubject Subject, FOnPlayerConnection OnComplete);
	
	bool CreatePlayerConnectionForServer(EPlayerConnectionSubject Subject, const FString& ServerId,
	                                     FOnPlayerConnection OnComplete);

	void StartPollingForPlayerConnectionStatus(UWorld* World);
	void StopPollingForPlayerConnectionStatus(UWorld* World);

	bool GetPlayerConnection(const FString& ConnectionId, FOnPlayerConnection OnComplete);
	bool ConsumeChallengeResponse(const FString& Code);

private:
	bool CreatePlayerConnection_Internal(const FString& SubjectStr, FOnPlayerConnection OnComplete);

	FString CurrentConnectionId;
	FTimerHandle PollingTimerHandle;
};
