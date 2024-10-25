// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ServiceDelegates.h"
#include "Models/ServerDataObject.h"
#include "Interfaces/IHttpRequest.h"
#include "Models/PaginationDataObject.h"
#include "Stores/ServersStore.h"
#include "Engine/Engine.h"
#include "ServersService.generated.h"


class UModerationLogEntryDataObject;
class UPlayerListDataObject;

DECLARE_DELEGATE_ThreeParams(FGetServerDetailsDelegate, UServerDataObject* /*Server*/, bool /*bSuccess*/,
                             TOptional<FText> /*Error*/);
DECLARE_DELEGATE_FourParams(FGetServersListDelegate, TArray<UServerDataObject*> /*Servers*/, UPaginationDataObject* /*PaginationDetails*/, bool /*bSuccess*/, TOptional<FText> /*Error*/);
DECLARE_DELEGATE_ThreeParams(FGetPlayersListDelegate, UPlayerListDataObject* /*ServerAndPlayersInfo*/, bool /*bSuccess*/, TOptional<FText> /*Error*/);
DECLARE_DELEGATE_ThreeParams(FListPublicServerModerationDelegate, TArray<UModerationLogEntryDataObject*> /*ModerationLogEntries*/, bool /*bSuccess*/, TOptional<FText> /*Error*/);

UCLASS()
class NODECRAFTDISCOVERY_API UServersService : public UEngineSubsystem
{
	GENERATED_BODY()

public:
	static UServersService& Get() { return *GEngine->GetEngineSubsystem<UServersService>(); }

	void JoinServer(UServerDataObject* ServerDataObject, const UWorld* World);

	bool GetServerDetails(FGetServerDetailsDelegate& OnComplete, const FString& ServerId);
	FDelegateHandle AddSearchStartedListener(const FSimpleMulticastDelegate::FDelegate& Delegate);
	bool RemoveSearchStartedListener(const FDelegateHandle& Handle);
	bool GetPopularServers(FGetServersListDelegate& OnComplete, int32 PageNumber = 1);
	bool GetRecommendedServers(FGetServersListDelegate& OnComplete, int32 PageNumber = 1);
	bool GetOwnedServers(FGetServersListDelegate& OnComplete, int32 PageNumber = 1);
	bool GetFavoriteServers(FGetServersListDelegate& OnComplete, int32 PageNumber = 1);
	bool GetOnlinePlayers(const FString& ServerId, const FGetPlayersListDelegate& OnComplete);
	bool GetRecentPLayers(const FString& ServerId, const FGetPlayersListDelegate& OnComplete);

	bool FavoriteServer(UServerDataObject* Server, FSimpleServiceResponseDelegate& OnComplete);
	bool UnfavoriteServer(UServerDataObject* Server, FSimpleServiceResponseDelegate& OnComplete);

	bool ListPublicServerModeration(const FString& ServerId, FListPublicServerModerationDelegate& OnComplete);

	void SearchServers(const UWorld* World, const FString& SearchQuery, TOptional<uint32> Page = 1, TOptional<bool> bShouldDebounce = true);
	bool GetServersForList(EServerListType List, FGetServersListDelegate& OnComplete, int32 PageNumber = 1);

private:
	void CreateServerListDelegate(const EServerListType List, const FGetServersListDelegate& Delegate, FHttpRequestCompleteDelegate& ReqCallbackOut);
	void CreatePlayerListDelegate(const FGetPlayersListDelegate& OnComplete, FHttpRequestCompleteDelegate& ReqCallbackOut);
	void CreateServerDetailsDelegate(const FGetServerDetailsDelegate& OnComplete, 	FHttpRequestCompleteDelegate& ReqCallbackOut);
	void CreateSimpleServiceResponseDelegate(const FSimpleServiceResponseDelegate& OnComplete, FHttpRequestCompleteDelegate& ReqCallbackOut);
	void CreateListPublicServerModerationDelegate(const FListPublicServerModerationDelegate& OnComplete, FHttpRequestCompleteDelegate& ReqCallbackOut);
	
	bool SearchServers_Internal(const FString& SearchQuery, TOptional<uint32> Page = 1);
	
	FTimerHandle SearchDebounceTimer;
	FSimpleMulticastDelegate OnSearchStarted;
};
