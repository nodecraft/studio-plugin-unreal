// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ServiceDelegates.h"
#include "Models/ServerDataObject.h"
#include "Interfaces/IHttpRequest.h"
#include "ServersService.generated.h"


class UModerationLogEntryDataObject;
class UPlayerListDataObject;

DECLARE_DELEGATE_ThreeParams(FGetServerDetailsDelegate, UServerDataObject* /*Server*/, bool /*bSuccess*/,
                             TOptional<FText> /*Error*/);
DECLARE_DELEGATE_ThreeParams(FGetServersListDelegate, TArray<UServerDataObject*> /*Servers*/, bool /*bSuccess*/, TOptional<FText> /*Error*/);
DECLARE_DELEGATE_ThreeParams(FGetPlayersListDelegate, UPlayerListDataObject* /*ServerAndPlayersInfo*/, bool /*bSuccess*/, TOptional<FText> /*Error*/);
DECLARE_DELEGATE_ThreeParams(FListPublicServerModerationDelegate, TArray<UModerationLogEntryDataObject*> /*ModerationLogEntries*/, bool /*bSuccess*/, TOptional<FText> /*Error*/);

UCLASS()
class NODECRAFTDISCOVERY_API UServersService : public UEngineSubsystem
{
	GENERATED_BODY()

public:
	static UServersService& Get() { return *GEngine->GetEngineSubsystem<UServersService>(); }

	void JoinServer(UServerDataObject* ServerDataObject, const UWorld* World);

	void CreateServerListDelegate(const FGetServersListDelegate& Delegate, FHttpRequestCompleteDelegate& ReqCallbackOut);
	void CreatePlayerListDelegate(const FGetPlayersListDelegate& OnComplete, FHttpRequestCompleteDelegate& ReqCallbackOut);
	void CreateServerDetailsDelegate(const FGetServerDetailsDelegate& OnComplete, 	FHttpRequestCompleteDelegate& ReqCallbackOut);
	void CreateSimpleServiceResponseDelegate(const FSimpleServiceResponseDelegate& OnComplete, FHttpRequestCompleteDelegate& ReqCallbackOut);
	void CreateListPublicServerModerationDelegate(const FListPublicServerModerationDelegate& OnComplete, FHttpRequestCompleteDelegate& ReqCallbackOut);

	bool GetServerDetails(FGetServerDetailsDelegate& OnComplete, const FString& ServerId);
	bool GetPopularServers(FGetServersListDelegate& OnComplete);
	bool GetRecommendedServers(FGetServersListDelegate& OnComplete);
	bool GetOwnedServers(FGetServersListDelegate& OnComplete);
	bool GetFavoriteServers(FGetServersListDelegate& OnComplete);
	bool GetOnlinePlayers(const FString& ServerId, const FGetPlayersListDelegate& OnComplete);
	bool GetRecentPLayers(const FString& ServerId, const FGetPlayersListDelegate& OnComplete);

	bool FavoriteServer(const UServerDataObject* Server, FSimpleServiceResponseDelegate& OnComplete);
	bool UnfavoriteServer(const UServerDataObject* Server, FSimpleServiceResponseDelegate& OnComplete);

	bool ListPublicServerModeration(const FString& ServerId, FListPublicServerModerationDelegate& OnComplete);
};
