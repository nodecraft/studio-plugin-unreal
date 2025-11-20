// Nodecraft, Inc. © 2012-2024, All Rights Reserved.


#include "Services/ServersService.h"

#include "JsonObjectWrapper.h"
#include "TimerManager.h"
#include "NodecraftLogCategories.h"
#include "API/NodecraftStudioApi.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "Models/ModerationLogEntryDataObject.h"
#include "Models/PaginationDataObject.h"
#include "Models/PlayerListDataObject.h"
#include "Services/ServerQueueService.h"
#include "Stores/SearchStore.h"
#include "Stores/ServersStore.h"
#include "Subsystems/MenuManagerSubsystem.h"
#include "Subsystems/MessageRouterSubsystem.h"

bool UServersService::GetPopularServers(FGetServersListDelegate& OnComplete, int32 PageNumber)
{
	FHttpRequestCompleteDelegate ReqCallback;
	CreateServerListDelegate(EServerListType::Popular, OnComplete, ReqCallback);
	return UNodecraftStudioApi::ListPopularServers(this, PageNumber, ReqCallback)->ProcessRequest();
}

bool UServersService::GetRecommendedServers(FGetServersListDelegate& OnComplete, int32 PageNumber)
{
	FHttpRequestCompleteDelegate ReqCallback;
	CreateServerListDelegate(EServerListType::Recommended, OnComplete, ReqCallback);
	return UNodecraftStudioApi::ListRecommendedServers(this, PageNumber, ReqCallback)->ProcessRequest();
}

bool UServersService::GetOwnedServers(FGetServersListDelegate& OnComplete, int32 PageNumber)
{
	FHttpRequestCompleteDelegate ReqCallback;
	CreateServerListDelegate(EServerListType::Owned, OnComplete, ReqCallback);
	return UNodecraftStudioApi::ListPlayerOwnedServers(this, PageNumber, ReqCallback)->ProcessRequest();
}

bool UServersService::GetFavoriteServers(FGetServersListDelegate& OnComplete, int32 PageNumber)
{
	FHttpRequestCompleteDelegate ReqCallback;
	CreateServerListDelegate(EServerListType::Favorite, OnComplete, ReqCallback);
	return UNodecraftStudioApi::ListFavoriteServers(this, PageNumber, ReqCallback)->ProcessRequest();
}

bool UServersService::GetOnlinePlayers(const FString& ServerId, const FGetPlayersListDelegate& OnComplete)
{
	FHttpRequestCompleteDelegate ReqCallback;
	CreatePlayerListDelegate(OnComplete, ReqCallback);
	return UNodecraftStudioApi::GetOnlinePlayers(this, ServerId, ReqCallback)->ProcessRequest();
}

bool UServersService::GetRecentPLayers(const FString& ServerId, const FGetPlayersListDelegate& OnComplete)
{
	FHttpRequestCompleteDelegate ReqCallback;
	CreatePlayerListDelegate(OnComplete, ReqCallback);
	return UNodecraftStudioApi::GetRecentPlayers(this, ServerId, ReqCallback)->ProcessRequest();
}

bool UServersService::FavoriteServer(UServerDataObject* Server, FSimpleServiceResponseDelegate& OnComplete)
{
	if (Server == nullptr)
	{
		UE_LOG(LogNodecraft_Servers, Error, TEXT("FavoriteServer: Server is nullptr"));
		return false;
	}
	FHttpRequestCompleteDelegate ReqCallback;
	FSimpleServiceResponseDelegate OnFavoriteComplete = FSimpleServiceResponseDelegate::CreateWeakLambda(this, [Server, OnComplete](bool bSuccess, TOptional<FText> Error)
	{
		OnComplete.ExecuteIfBound(bSuccess, Error);
		if (bSuccess)
		{
			UServersStore::Get().AddFavoriteServer(Server);
		}
	});
	CreateSimpleServiceResponseDelegate(OnFavoriteComplete, ReqCallback);
	
	return UNodecraftStudioApi::FavoriteServer(this, Server->GetId(), ReqCallback)->ProcessRequest();
}

bool UServersService::UnfavoriteServer(UServerDataObject* Server, FSimpleServiceResponseDelegate& OnComplete)
{
	if (Server == nullptr)
	{
		UE_LOG(LogNodecraft_Servers, Error, TEXT("UnfavoriteServer: Server is nullptr"));
		return false;
	}
	FSimpleServiceResponseDelegate OnUnfavoriteComplete = FSimpleServiceResponseDelegate::CreateWeakLambda(this, [Server, OnComplete](bool bSuccess, TOptional<FText> Error)
	{
		if (bSuccess)
		{
			UServersStore::Get().RemoveFavoriteServer(Server);
		}
		OnComplete.ExecuteIfBound(bSuccess, Error);
	});
	FHttpRequestCompleteDelegate ReqCallback;
	CreateSimpleServiceResponseDelegate(OnUnfavoriteComplete, ReqCallback);
	return UNodecraftStudioApi::UnfavoriteServer(this, Server->GetId(), ReqCallback)->ProcessRequest();
}

bool UServersService::ListPublicServerModeration(const FString& ServerId, FListPublicServerModerationDelegate& OnComplete)
{
	FHttpRequestCompleteDelegate ReqCallback;
	CreateListPublicServerModerationDelegate(OnComplete, ReqCallback);
	return UNodecraftStudioApi::ListPublicServerModeration(this, ServerId, ReqCallback)->ProcessRequest();
}

void UServersService::SearchServers(const UWorld* World, const FString& SearchQuery, TOptional<uint32> Page, TOptional<bool> bShouldDebounce)
{
	if (World == nullptr)
	{
		return;
	}
	
	USearchStore::Get().SetSearchQuery(SearchQuery);
	if (SearchQuery.Len() < 3)
    {
        return;
    }
	
	if (bShouldDebounce.Get(true))
	{
		// If a search is already scheduled, unschedule it.
        // We want to clear the timer even if the search query is < 3 characters
        // because user stated intention to NOT perform the query as stated.
        if (SearchDebounceTimer.IsValid())
        {
        	World->GetTimerManager().ClearTimer(SearchDebounceTimer);
        }
		
        // Schedule the search to happen 1 second from now
        constexpr float SearchDebounceTime = 1.0f;
        World->GetTimerManager().SetTimer(SearchDebounceTimer, [this, SearchQuery, Page]()
        {
        	SearchServers_Internal(SearchQuery, Page);
        }, SearchDebounceTime, false);
	}
	else
	{
		SearchServers_Internal(SearchQuery, Page);
	}
}

bool UServersService::GetServersForList(EServerListType List, FGetServersListDelegate& OnComplete, int32 PageNumber)
{
	FHttpRequestCompleteDelegate ReqCallback;
	CreateServerListDelegate(List, OnComplete, ReqCallback);

	switch (List)
	{
	case EServerListType::Popular:
		return UNodecraftStudioApi::ListPopularServers(this, PageNumber, ReqCallback)->ProcessRequest();
	case EServerListType::Favorite:
		return UNodecraftStudioApi::ListFavoriteServers(this, PageNumber, ReqCallback)->ProcessRequest();
	case EServerListType::Recommended:
		return UNodecraftStudioApi::ListRecommendedServers(this, PageNumber, ReqCallback)->ProcessRequest();
	case EServerListType::Owned:
		return UNodecraftStudioApi::ListPlayerOwnedServers(this, PageNumber, ReqCallback)->ProcessRequest();
	default:
		unimplemented()
		return false;
	}
}

bool UServersService::SearchServers_Internal(const FString& SearchQuery, TOptional<uint32> Page)
{
	FHttpRequestCompleteDelegate ReqCallback;
	ReqCallback.BindWeakLambda(this, [](FHttpRequestPtr Req, FHttpResponsePtr Res, bool bConnectedSuccessfully)
	{
		if (bConnectedSuccessfully)
		{
			if (EHttpResponseCodes::IsOk(Res.Get()->GetResponseCode()))
			{
				FJsonObjectWrapper ResJson;
				ResJson.JsonObjectFromString(Res.Get()->GetContentAsString());
				USearchStore& SearchStore = USearchStore::Get();
				TArray<UServerDataObject*> Servers;
				if (const TArray<TSharedPtr<FJsonValue>>& Data = ResJson.JsonObject->GetArrayField(TEXT("data")); Data.Num() > 0)
				{
					for (TSharedPtr<FJsonValue, ESPMode::ThreadSafe> JsonValue : Data)
					{
						UServerDataObject* Server = UServerDataObject::FromJson(JsonValue.Get()->AsObject().ToSharedRef());
						Servers.Add(Server);
					}
				}
				if (const TSharedPtr<FJsonObject>& Pagination = ResJson.JsonObject->GetObjectField(TEXT("pagination")); Pagination.IsValid())
				{
					UPaginationDataObject* PaginationDataObject = UPaginationDataObject::FromJson(Pagination.ToSharedRef());
					SearchStore.SetCurrentPageNumber(PaginationDataObject->GetPage());
					SearchStore.SetMaxPages(PaginationDataObject->GetPagesMax());
				}
				SearchStore.SetSearchResults(Servers);
			}
			else
			{
				UE_LOG(LogNodecraft_Servers, Error, TEXT("Bad response when trying to search for servers. Response code: %d. Log message router for more details."), Res.Get()->GetResponseCode());
				USearchStore::Get().SetSearchResults({});
			}
			UMessageRouterSubsystem::Get().RouteHTTPResult(Res, __FUNCTION__);
		}
		else
		{
			UE_LOG(LogNodecraft_Servers, Error, TEXT("Failed to connect to server"));
			UMessageRouterSubsystem::Get().RouteFailureToConnect(__FUNCTION__);
			USearchStore::Get().SetSearchResults({});
		}
	});
	
	OnSearchStarted.Broadcast();

	// ideally, this would get called on search store initialization,
	// but we can't do that because of the timing of initialization of search store and this as engine subsystems.
	USearchStore::Get().ShowSearchResults();
    return UNodecraftStudioApi::SearchServers(this, SearchQuery, Page, ReqCallback)->ProcessRequest();
}

void UServersService::JoinServer(UServerDataObject* ServerDataObject, const UWorld* World)
{
	if (ServerDataObject)
	{
		UE_LOG(LogNodecraft_Servers, Log, TEXT("JoinServer: Joining server %s"), *ServerDataObject->GetId());
		if (URulesDataObject* Rules = ServerDataObject->GetRules())
		{
			if (Rules->GetConsentStatus() == EConsentStatus::Outdated)
			{
				UE_LOG(LogNodecraft_Servers, Log, TEXT("JoinServer: Server consent is outdated"));
				TMap<FString, FText> Argument;
				Argument.Add("RulesError", FText::FromString("Consent for server is outdated and must be signed"));
				UMenuManagerSubsystem::Get().ShowServerDetails(ServerDataObject, Argument);
				return;
			}
			else if (Rules->GetConsentStatus() == EConsentStatus::NotSigned)
			{
				UE_LOG(LogNodecraft_Servers, Log, TEXT("JoinServer: Server consent is not signed"));
				TMap<FString, FText> Argument;
				Argument.Add("RulesError", FText::FromString("Consent for server is missing and must be signed"));
				UMenuManagerSubsystem::Get().ShowServerDetails(ServerDataObject, Argument);
				return;
			}
		}
		else
		{
			// the server data object is incomplete. we need to finish loading it.
			FGetServerDetailsDelegate OnGetFullServerDetails;
			OnGetFullServerDetails.BindWeakLambda(this, [this, World](UServerDataObject* Server, bool bSuccess, TOptional<FText> Error)
			{
				if (bSuccess)
				{
					JoinServer(Server, World);
				}
			});
			Get().GetServerDetails(OnGetFullServerDetails, ServerDataObject->GetId());
			return;
		}

		if (ServerDataObject->HasPassword())
		{
			UE_LOG(LogNodecraft_Servers, Log, TEXT("JoinServer: Server has password"));
			UMenuManagerSubsystem::Get().ShowServerPasswordModal(ServerDataObject);
		}
		else
		{
			UE_LOG(LogNodecraft_Servers, Log, TEXT("JoinServer: Server does not have password. Moving into server queue."));
			
			UServerQueueService* ServerQueueService = UServerQueueService::Get(World);
			ServerQueueService->OnStartedPollingServerQueue.RemoveAll(this);
			ServerQueueService->OnStartedPollingServerQueue.AddWeakLambda(this, [this]()
			{
				UMenuManagerSubsystem::Get().ShowJoiningServerQueue();
			});
			ServerQueueService->JoinServer(ServerDataObject);
		}
	}
}

void UServersService::CreateServerListDelegate(const EServerListType List, const FGetServersListDelegate& OnComplete, FHttpRequestCompleteDelegate& ReqCallbackOut)
{
	ReqCallbackOut.BindWeakLambda(this, [OnComplete, List](FHttpRequestPtr Req, FHttpResponsePtr Res, bool bConnectedSuccessfully)
	{
		if (bConnectedSuccessfully)
		{
			if (EHttpResponseCodes::IsOk(Res.Get()->GetResponseCode()))
			{
				FJsonObjectWrapper ResJson;
				ResJson.JsonObjectFromString(Res.Get()->GetContentAsString());
				TArray<UServerDataObject*> Servers;
				UPaginationDataObject* PaginationDataObject = nullptr;
				if (const TArray<TSharedPtr<FJsonValue>>& Data = ResJson.JsonObject->GetArrayField(TEXT("data")); Data.Num() > 0)
				{
					for (TSharedPtr<FJsonValue, ESPMode::ThreadSafe> JsonValue : Data)
					{
						UServerDataObject* Server = UServerDataObject::FromJson(JsonValue.Get()->AsObject().ToSharedRef());
						Servers.Add(Server);
					}
				}
				if (const TSharedPtr<FJsonObject>& Pagination = ResJson.JsonObject->GetObjectField(TEXT("pagination")); Pagination.IsValid())
				{
					PaginationDataObject = UPaginationDataObject::FromJson(Pagination.ToSharedRef());
				}

				UServersStore::Get().AddServers(List, Servers, PaginationDataObject);

				
				OnComplete.ExecuteIfBound(Servers, PaginationDataObject, true, TOptional<FText>());
			}
			else
			{
				UE_LOG(LogNodecraft_Servers, Error, TEXT("Bad response when trying to get list of servers. Response code: %d. Log message router for more details."), Res.Get()->GetResponseCode());
				OnComplete.ExecuteIfBound({}, nullptr, false, FText::FromString("Response wasn't okay I guess"));
			}
			UMessageRouterSubsystem::Get().RouteHTTPResult(Res, __FUNCTION__);
		}
		else
		{
			UE_LOG(LogNodecraft_Servers, Error, TEXT("Failed to connect to server"));
			UMessageRouterSubsystem::Get().RouteFailureToConnect(__FUNCTION__);
			OnComplete.ExecuteIfBound({}, nullptr, false, FText::FromString("Failed to connect to server"));
		}
	});
}

void UServersService::CreateServerDetailsDelegate(const FGetServerDetailsDelegate& OnComplete, 	FHttpRequestCompleteDelegate& ReqCallbackOut)
{
	ReqCallbackOut.BindWeakLambda(this, [OnComplete](FHttpRequestPtr Req, FHttpResponsePtr Res, bool bConnectedSuccessfully)
	{
		if (bConnectedSuccessfully)
		{
			if (EHttpResponseCodes::IsOk(Res.Get()->GetResponseCode()))
			{
				FJsonObjectWrapper ResJson;
				ResJson.JsonObjectFromString(Res.Get()->GetContentAsString());
				if (const TSharedPtr<FJsonObject>& Data = ResJson.JsonObject->GetObjectField(TEXT("data")); Data.IsValid())
				{
					if (const TSharedPtr<FJsonObject>& ServerData = Data->GetObjectField(TEXT("server")); ServerData.IsValid())
					{
						UServerDataObject* Server = UServerDataObject::FromJson(ServerData.ToSharedRef());
						if (const TSharedPtr<FJsonObject>& RulesData = Data->GetObjectField(TEXT("rules")); RulesData.IsValid())
						{
							URulesDataObject* Rules = URulesDataObject::FromJson(RulesData.ToSharedRef());
							Server->SetRules(Rules);
						}
						UE_LOG(LogNodecraft_Servers, Log, TEXT("Server details retrieved successfully"));
						OnComplete.ExecuteIfBound(Server, true, TOptional<FText>());
					}
					else
					{
						TOptional<FText> Error = TOptional<FText>();
						Error.Emplace( FText::FromString("Could not retrieve server details from data object"));
						UE_LOG(LogNodecraft_Servers, Error, TEXT("Could not retrieve server details from data object"));
						OnComplete.ExecuteIfBound(nullptr, false, Error);
					}
				}
				else
				{
					TOptional<FText> Error = TOptional<FText>();
					Error.Emplace( FText::FromString("Could not retrieve data object when retrieving server details"));
					UE_LOG(LogNodecraft_Servers, Error, TEXT("Could not retrieve data object when retrieving server details"));
					OnComplete.ExecuteIfBound(nullptr, false, Error);
				}
			}
			else
			{
				UE_LOG(LogNodecraft_Servers, Error, TEXT("Bad response when trying to retrieve server details. Response code: %d. Log message router for more details."), Res.Get()->GetResponseCode());
				OnComplete.ExecuteIfBound({}, false, FText::FromString("Received non-200 response"));
			}
			UMessageRouterSubsystem::Get().RouteHTTPResult(Res, __FUNCTION__);
		}
		else
		{
			UE_LOG(LogNodecraft_Servers, Error, TEXT("Failed to connect to server"));
			UMessageRouterSubsystem::Get().RouteFailureToConnect(__FUNCTION__);
			OnComplete.ExecuteIfBound({}, false, FText::FromString("Failed to connect to server"));
		}
	});
}

void UServersService::CreateSimpleServiceResponseDelegate(const FSimpleServiceResponseDelegate& OnComplete,
	FHttpRequestCompleteDelegate& ReqCallbackOut)
{
	ReqCallbackOut.BindWeakLambda(this, [OnComplete](FHttpRequestPtr Req, FHttpResponsePtr Res, bool bConnectedSuccessfully)
	{
		if (bConnectedSuccessfully)
		{
			if (EHttpResponseCodes::IsOk(Res.Get()->GetResponseCode()))
			{
				OnComplete.ExecuteIfBound(true, TOptional<FText>());
			}
			else
			{
				FJsonObjectWrapper ResJson;
				ResJson.JsonObjectFromString(Res.Get()->GetContentAsString());
				if (const FString Message = ResJson.JsonObject->GetStringField(TEXT("message")); Message.IsEmpty() == false)
				{
					const FString ErrorText = FString::Printf(TEXT("Error code: %d. Message: %ls"), Res.Get()->GetResponseCode(), *Message);
					UE_LOG(LogNodecraft_Servers, Error, TEXT("%s"), *ErrorText);
					OnComplete.ExecuteIfBound(false, TOptional<FText>(FText::FromString(ErrorText)));
				}
				else
				{
					const FString ErrorText = FString::Printf(TEXT("Received %d response w/out error message"), Res.Get()->GetResponseCode());
					UE_LOG(LogNodecraft_Servers, Error, TEXT("%s"), *ErrorText);
					OnComplete.ExecuteIfBound(false, TOptional<FText>(FText::FromString(ErrorText)));
				}
			}
			UMessageRouterSubsystem::Get().RouteHTTPResult(Res, __FUNCTION__);
		}
		else
		{
			UMessageRouterSubsystem::Get().RouteFailureToConnect(__FUNCTION__);
			UE_LOG(LogNodecraft_Servers, Error, TEXT("Failed to connect to server"));
			OnComplete.ExecuteIfBound(false, FText::FromString("Failed to connect to server"));
		}
	});
}

void UServersService::CreateListPublicServerModerationDelegate(const FListPublicServerModerationDelegate& OnComplete, FHttpRequestCompleteDelegate& ReqCallbackOut)
{
	ReqCallbackOut.BindWeakLambda(this, [OnComplete](FHttpRequestPtr Req, FHttpResponsePtr Res, bool bConnectedSuccessfully)
	{
		if (bConnectedSuccessfully)
		{
			if (EHttpResponseCodes::IsOk(Res.Get()->GetResponseCode()))
			{
				FJsonObjectWrapper ResJson;
				ResJson.JsonObjectFromString(Res.Get()->GetContentAsString());
				if (const TArray<TSharedPtr<FJsonValue>>& Data = ResJson.JsonObject->GetArrayField(TEXT("data")); Data.Num() > 0)
				{
					TArray<UModerationLogEntryDataObject*> ModerationLogEntryDataObjects;
					for (TSharedPtr<FJsonValue, ESPMode::ThreadSafe> JsonValue : Data)
					{
						UModerationLogEntryDataObject* ModerationLogEntryDataObject = UModerationLogEntryDataObject::FromJson(JsonValue.Get()->AsObject().ToSharedRef());
						ModerationLogEntryDataObjects.Add(ModerationLogEntryDataObject);
					}
					OnComplete.ExecuteIfBound(ModerationLogEntryDataObjects, true, TOptional<FText>());
				}
				else
				{
					OnComplete.ExecuteIfBound({}, true, TOptional<FText>());
				}
			}
			else
			{
				FJsonObjectWrapper ResJson;
				ResJson.JsonObjectFromString(Res.Get()->GetContentAsString());
				if (const FString Message = ResJson.JsonObject->GetStringField(TEXT("message")); Message.IsEmpty() == false)
				{
					const FString ErrorText = FString::Printf(TEXT("UServersService::CreateListPublicServerModerationDelegate: Error code: %d. Message: %ls"), Res.Get()->GetResponseCode(), *Message);
					UE_LOG(LogNodecraft_Servers, Error, TEXT("%s"), *ErrorText);
					OnComplete.ExecuteIfBound({}, false, TOptional<FText>(FText::FromString(ErrorText)));
				}
				else
				{
					const FString ErrorText = FString::Printf(TEXT("UServersService::CreateListPublicServerModerationDelegate: Received %d response w/out error message"), Res.Get()->GetResponseCode());
					UE_LOG(LogNodecraft_Servers, Error, TEXT("%s"), *ErrorText);
					OnComplete.ExecuteIfBound({}, false, TOptional<FText>(FText::FromString(ErrorText)));
				}
			}
			UMessageRouterSubsystem::Get().RouteHTTPResult(Res, __FUNCTION__);
		}
		else
		{
			UMessageRouterSubsystem::Get().RouteFailureToConnect(__FUNCTION__);
			UE_LOG(LogNodecraft_Servers, Error, TEXT("UServersService::CreateListPublicServerModerationDelegate: Failed to connect to server"));
			OnComplete.ExecuteIfBound({}, false, TOptional<FText>(FText::FromString("UServersService::CreateListPublicServerModerationDelegate: Failed to connect tot server")));
		}
	});
}

void UServersService::CreatePlayerListDelegate(const FGetPlayersListDelegate& OnComplete, FHttpRequestCompleteDelegate& ReqCallbackOut)
{
	ReqCallbackOut.BindWeakLambda(this, [OnComplete](FHttpRequestPtr Req, FHttpResponsePtr Res, bool bConnectedSuccessfully)
	{
		if (bConnectedSuccessfully)
		{
			if (EHttpResponseCodes::IsOk(Res.Get()->GetResponseCode()))
			{
				FJsonObjectWrapper ResJson;
				ResJson.JsonObjectFromString(Res.Get()->GetContentAsString());
				if (const TSharedPtr<FJsonObject>* Data; ResJson.JsonObject->TryGetObjectField(TEXT("data"), Data))
				{
					UPlayerListDataObject* PlayerListDataObject;
					
					if (Data->Get()->HasField(TEXT("players")))
					{
						PlayerListDataObject = UPlayerListDataObject::FromJson(Data->ToSharedRef(), "players");
					}
					else
					{
						PlayerListDataObject = UPlayerListDataObject::FromJson(ResJson.JsonObject.ToSharedRef(), "data");
					}
					TArray<UPlayerDataObject*> Players;
					for (UPlayerDataObject* PlayerDataObject : PlayerListDataObject->GetPlayerDataObjects())
					{
						Players.Add(PlayerDataObject);
					}
					PlayerListDataObject->SetPlayerDataObjects(Players);
					UE_LOG(LogNodecraft_Servers, Log, TEXT("Player list retrieved successfully"));
					OnComplete.ExecuteIfBound(PlayerListDataObject, true, TOptional<FText>());
				}
				else
				{
					OnComplete.ExecuteIfBound({}, false, FText::FromString("ServersService::CreatePlayerListDelegate: Error while grabbing data from response"));
				}
			}
			else
			{
				OnComplete.ExecuteIfBound({}, false, FText::FromString("Response wasn't okay I guess"));
			}
			UMessageRouterSubsystem::Get().RouteHTTPResult(Res, __FUNCTION__);
		}
		else
		{
			UMessageRouterSubsystem::Get().RouteFailureToConnect(__FUNCTION__);
			OnComplete.ExecuteIfBound({}, false, FText::FromString("Failed to connect to server"));
		}
	});
}

bool UServersService::GetServerDetails(FGetServerDetailsDelegate& OnComplete, const FString& ServerId)
{
	FHttpRequestCompleteDelegate ReqCallback;
	CreateServerDetailsDelegate(OnComplete, ReqCallback);
	return UNodecraftStudioApi::GetServerDetails(this, ServerId, ReqCallback)->ProcessRequest();
}

FDelegateHandle UServersService::AddSearchStartedListener(const FSimpleMulticastDelegate::FDelegate& Delegate)
{
	Delegate.ExecuteIfBound();
	return OnSearchStarted.Add(Delegate);
}

bool UServersService::RemoveSearchStartedListener(const FDelegateHandle& Handle)
{
	return OnSearchStarted.Remove(Handle);
}