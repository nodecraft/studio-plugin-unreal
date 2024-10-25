// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/Engine.h"
#include "ServiceDelegates.h"
#include "Interfaces/IHttpRequest.h"
#include "Models/KickDataObject.h"
#include "Models/ModerationReasonDataObject.h"
#include "Subsystems/EngineSubsystem.h"
#include "Models/PlayerServerDetails.h"
#include "ModerationService.generated.h"

/**
 * 
 */

DECLARE_DELEGATE_ThreeParams(FGetPlayerServerDetails, UPlayerServerDetailsDataObject* /* Player */, bool /* bSuccess */, TOptional<FText> /* Error */);
DECLARE_DELEGATE_ThreeParams(FGetPlayerServerDetailsList, TArray<UPlayerServerDetailsDataObject*> /* Players */, bool /* bSuccess */, TOptional<FText> /* Error */);
DECLARE_DELEGATE_ThreeParams(FGetModActionReasonsDelegate, TArray<UModerationReasonDataObject*> /* Reasons */, bool /* bSuccess */, TOptional<FText> /* Error */);
DECLARE_DELEGATE_ThreeParams(FPlayersKickedDelegate, TArray<FPlayerKickDataObject> /* Kicks */, bool /* bSuccess */, TOptional<FText> /* Error */);
DECLARE_DELEGATE_ThreeParams(FPlayersBannedDelegate, TArray<UBanDataObject*> /* Bans */, bool /* bSuccess */, TOptional<FText> /* Error */);
DECLARE_DELEGATE_ThreeParams(FPlayersModStatusUpdatedDelegate, TArray<UPlayerServerDetailsDataObject*> /* Players */, bool /* bSuccess */, TOptional<FText> /* Error */);

UCLASS()
class NODECRAFTDISCOVERY_API UModerationService : public UEngineSubsystem
{
	GENERATED_BODY()

public:
	//getter
	static UModerationService& Get() { return *GEngine->GetEngineSubsystem<UModerationService>(); }

	void CreatePlayersListModerationDelegate(const FGetPlayerServerDetailsList& OnComplete, FHttpRequestCompleteDelegate& ReqCallbackOut);

	void CreateServerPlayersDetailDelegate(const FGetPlayerServerDetails& OnComplete, FHttpRequestCompleteDelegate& ReqCallbackOut);

	bool GetModerators(const UWorld* World, const FString& ServerId);

	bool GetOnlinePlayers(const UWorld* World, const FString& ServerId);

	bool GetOfflinePlayers(const UWorld* World, const FString& ServerId);

	bool GetBannedPlayers(const UWorld* World, const FString& ServerId);

	bool GetOwner(const UWorld* World, const FString& ServerId);

	TSharedRef<IHttpRequest> GetModeratorsRequest(const UWorld* World, const FString& ServerId);
	TSharedRef<IHttpRequest> GetOnlinePlayersRequest(const UWorld* World, const FString& ServerId);
	TSharedRef<IHttpRequest> GetOfflinePlayersRequest(const UWorld* World, const FString& ServerId);
	TSharedRef<IHttpRequest> GetBannedPlayersRequest(const UWorld* World, const FString& ServerId);
	TSharedRef<IHttpRequest> GetOwnerRequest(const UWorld* World, const FString& ServerId);
	

	bool GetModerationActionReasons(UWorld* World, FGetModActionReasonsDelegate OnComplete);

	bool KickPlayers(const TArray<FString> PlayerIds, const FString& ServerId, const UModerationReasonDataObject* Reason, const FString&
	                 ReasonNotes, FPlayersKickedDelegate OnComplete);

	bool BanPlayers(TArray<FString> PlayerIds, const FString& ServerId, const TOptional<FDateTime>& DateExpires, const UModerationReasonDataObject
	                * ReasonId, const FString& ReasonNotes, FPlayersBannedDelegate OnComplete);

	bool UnbanPlayers(TArray<FString> BanIds, FPlayersBannedDelegate OnComplete);

	bool UpdatePlayerModStatus(UWorld* World, TArray<UPlayerServerDetailsDataObject*> Players, const FString& ServerId, const bool bIsModerator, FSimpleServiceResponseDelegate
	                           OnComplete);
};
