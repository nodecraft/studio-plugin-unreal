// Nodecraft, Inc. © 2012-2024, All Rights Reserved.


#include "Services/ServersService.h"

#include "JsonObjectWrapper.h"
#include "NodecraftLogCategories.h"
#include "Api/NodecraftStudioApi.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "Models/ModerationLogEntryDataObject.h"
#include "Models/PlayerListDataObject.h"
#include "Services/ServerQueueService.h"
#include "Stores/ServersStore.h"
#include "Subsystems/MenuManagerSubsystem.h"
#include "Subsystems/MessageRouterSubsystem.h"

bool UServersService::GetPopularServers(FGetServersListDelegate& OnComplete)
{
	FHttpRequestCompleteDelegate ReqCallback;
	CreateServerListDelegate(OnComplete, ReqCallback);
	return UNodecraftStudioApi::ListPopularServers(this, ReqCallback)->ProcessRequest();
}

bool UServersService::GetRecommendedServers(FGetServersListDelegate& OnComplete)
{
	FHttpRequestCompleteDelegate ReqCallback;
	CreateServerListDelegate(OnComplete, ReqCallback);
	return UNodecraftStudioApi::ListRecommendedServers(this, ReqCallback)->ProcessRequest();
}

bool UServersService::GetOwnedServers(FGetServersListDelegate& OnComplete)
{
	FHttpRequestCompleteDelegate ReqCallback;
	CreateServerListDelegate(OnComplete, ReqCallback);
	return UNodecraftStudioApi::ListPlayerOwnedServers(this, ReqCallback)->ProcessRequest();
}

bool UServersService::GetFavoriteServers(FGetServersListDelegate& OnComplete)
{
	FHttpRequestCompleteDelegate ReqCallback;
	FGetServersListDelegate OnGetFavoritesComplete = FGetServersListDelegate::CreateWeakLambda(this, [OnComplete](TArray<UServerDataObject*> Servers, bool bSuccess, TOptional<FText> Error)
	{
		if (bSuccess)
		{
			UServersStore::Get().SetFavoriteServers(Servers);
		}
		OnComplete.ExecuteIfBound(Servers, bSuccess, Error);
	});
	CreateServerListDelegate(OnGetFavoritesComplete, ReqCallback);
	return UNodecraftStudioApi::ListFavoriteServers(this, ReqCallback)->ProcessRequest();
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

bool UServersService::FavoriteServer(const UServerDataObject* Server, FSimpleServiceResponseDelegate& OnComplete)
{
	if (Server == nullptr)
	{
		UE_LOG(LogNodecraft_Servers, Error, TEXT("FavoriteServer: Server is nullptr"));
		return false;
	}
	FHttpRequestCompleteDelegate ReqCallback;
	FSimpleServiceResponseDelegate OnFavoriteComplete = FSimpleServiceResponseDelegate::CreateWeakLambda(this, [Server, OnComplete](bool bSuccess, TOptional<FText> Error)
	{
		if (bSuccess)
		{
			UServersStore::Get().AddFavoriteServer(const_cast<UServerDataObject*>(Server));
		}
		OnComplete.ExecuteIfBound(bSuccess, Error);
	});
	CreateSimpleServiceResponseDelegate(OnFavoriteComplete, ReqCallback);
	
	return UNodecraftStudioApi::FavoriteServer(this, Server->GetId(), ReqCallback)->ProcessRequest();
}

bool UServersService::UnfavoriteServer(const UServerDataObject* Server, FSimpleServiceResponseDelegate& OnComplete)
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
			UServersStore::Get().RemoveFavoriteServer(const_cast<UServerDataObject*>(Server));
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
				UMenuManagerSubsystem::Get().ShowServerDetails(ServerDataObject, FText::FromString("Consent for server is outdated and must be signed"));
				return;
			}
			else if (Rules->GetConsentStatus() == EConsentStatus::NotSigned)
			{
				UE_LOG(LogNodecraft_Servers, Log, TEXT("JoinServer: Server consent is not signed"));
				UMenuManagerSubsystem::Get().ShowServerDetails(ServerDataObject, FText::FromString("Consent for server is missing and must be signed"));
				return;
			}
		}

		if (ServerDataObject->HasPassword())
		{
			UE_LOG(LogNodecraft_Servers, Log, TEXT("JoinServer: Server has password"));
			UMenuManagerSubsystem::Get().ShowServerPasswordModal(ServerDataObject);
		}
		else
		{
			UE_LOG(LogNodecraft_Servers, Log, TEXT("JoinServer: Server does not have password. Moving into server queue."));
			UMenuManagerSubsystem::Get().ShowJoiningServerQueue();
			UServerQueueService::Get(World)->JoinServer(ServerDataObject);
		}
	}
}

void UServersService::CreateServerListDelegate(const FGetServersListDelegate& OnComplete, 	FHttpRequestCompleteDelegate& ReqCallbackOut)
{
	ReqCallbackOut.BindWeakLambda(this, [OnComplete](FHttpRequestPtr Req, FHttpResponsePtr Res, bool bConnectedSuccessfully)
	{
		if (bConnectedSuccessfully)
		{
			if (EHttpResponseCodes::IsOk(Res.Get()->GetResponseCode()))
			{
				FJsonObjectWrapper ResJson;
				ResJson.JsonObjectFromString(Res.Get()->GetContentAsString());
				TArray<UServerDataObject*> Servers;
				if (const TArray<TSharedPtr<FJsonValue>>& Data = ResJson.JsonObject->GetArrayField("data"); Data.Num() > 0)
				{
					for (TSharedPtr<FJsonValue, ESPMode::ThreadSafe> JsonValue : Data)
					{
						UServerDataObject* Server = UServerDataObject::FromJson(JsonValue.Get()->AsObject().ToSharedRef());
						Servers.Add(Server);
					}
				}
				OnComplete.ExecuteIfBound(Servers, true, TOptional<FText>());
			}
			else
			{
				UE_LOG(LogNodecraft_Servers, Error, TEXT("Bad response when trying to join server. Response code: %d. Log message router for more details."), Res.Get()->GetResponseCode());
				OnComplete.ExecuteIfBound({}, false, FText::FromString("Response wasn't okay I guess"));
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
				if (const TSharedPtr<FJsonObject>& Data = ResJson.JsonObject->GetObjectField("data"); Data.IsValid())
				{
					if (const TSharedPtr<FJsonObject>& ServerData = Data->GetObjectField("server"); ServerData.IsValid())
					{
						UServerDataObject* Server = UServerDataObject::FromJson(ServerData.ToSharedRef());
						if (const TSharedPtr<FJsonObject>& RulesData = Data->GetObjectField("rules"); RulesData.IsValid())
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
				if (const FString Message = ResJson.JsonObject->GetStringField("message"); Message.IsEmpty() == false)
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
				if (const TArray<TSharedPtr<FJsonValue>>& Data = ResJson.JsonObject->GetArrayField("data"); Data.Num() > 0)
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
					// TODO: should this be true or false?
					UE_LOG(LogNodecraft_Servers, Error, TEXT("ServersService::CreateListPublicServerModerationDelegate: Error while grabbing data from response"));
					OnComplete.ExecuteIfBound({}, true, FText::FromString("Error while grabbing data from response"));
				}
			}
			else
			{
				FJsonObjectWrapper ResJson;
				ResJson.JsonObjectFromString(Res.Get()->GetContentAsString());
				if (const FString Message = ResJson.JsonObject->GetStringField("message"); Message.IsEmpty() == false)
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
				if (const TSharedPtr<FJsonObject>* Data; ResJson.JsonObject->TryGetObjectField("data", Data))
				{
					UPlayerListDataObject* PlayerListDataObject;
					
					if (Data->Get()->HasField("players"))
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
