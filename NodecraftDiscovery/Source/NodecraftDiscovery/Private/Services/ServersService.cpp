// Fill out your copyright notice in the Description page of Project Settings.


#include "Services/ServersService.h"

#include "JsonObjectWrapper.h"
#include "API/DiscoveryAPI.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "Models/ModerationLogEntryDataObject.h"
#include "Models/PlayerListDataObject.h"
#include "Subsystems/MenuManagerSubsystem.h"

DEFINE_LOG_CATEGORY_STATIC(LogServersService, Log, All);

bool UServersService::GetPopularServers(FGetServersListDelegate& OnComplete)
{
	FHttpRequestCompleteDelegate ReqCallback;
	CreateServerListDelegate(OnComplete, ReqCallback);
	return UDiscoveryAPI::ListPopularServers(this, ReqCallback)->ProcessRequest();
}

bool UServersService::GetRecommendedServers(FGetServersListDelegate& OnComplete)
{
	FHttpRequestCompleteDelegate ReqCallback;
	CreateServerListDelegate(OnComplete, ReqCallback);
	return UDiscoveryAPI::ListRecommendedServers(this, ReqCallback)->ProcessRequest();
}

bool UServersService::GetOwnedServers(FGetServersListDelegate& OnComplete)
{
	FHttpRequestCompleteDelegate ReqCallback;
	CreateServerListDelegate(OnComplete, ReqCallback);
	return UDiscoveryAPI::ListPlayerOwnedServers(this, ReqCallback)->ProcessRequest();
}

bool UServersService::GetFavoriteServers(FGetServersListDelegate& OnComplete)
{
	FHttpRequestCompleteDelegate ReqCallback;
	CreateServerListDelegate(OnComplete, ReqCallback);
	return UDiscoveryAPI::ListFavoriteServers(this, ReqCallback)->ProcessRequest();
}

bool UServersService::GetOnlinePlayers(const FString& ServerId, const FGetPlayersListDelegate& OnComplete)
{
	FHttpRequestCompleteDelegate ReqCallback;
	CreatePlayerListDelegate(OnComplete, ReqCallback);
	return UDiscoveryAPI::GetOnlinePlayers(this, ServerId, ReqCallback)->ProcessRequest();
}

bool UServersService::GetRecentPLayers(const FString& ServerId, const FGetPlayersListDelegate& OnComplete)
{
	FHttpRequestCompleteDelegate ReqCallback;
	CreatePlayerListDelegate(OnComplete, ReqCallback);
	return UDiscoveryAPI::GetRecentPlayers(this, ServerId, ReqCallback)->ProcessRequest();
}

bool UServersService::FavoriteServer(const FString& ServerId, FSimpleServiceResponseDelegate& OnComplete)
{
	FHttpRequestCompleteDelegate ReqCallback;
	CreateSimpleServiceResponseDelegate(OnComplete, ReqCallback);
	return UDiscoveryAPI::FavoriteServer(this, ServerId, ReqCallback)->ProcessRequest();
}

bool UServersService::UnfavoriteServer(const FString& ServerId, FSimpleServiceResponseDelegate& OnComplete)
{
	FHttpRequestCompleteDelegate ReqCallback;
	CreateSimpleServiceResponseDelegate(OnComplete, ReqCallback);
	return UDiscoveryAPI::UnfavoriteServer(this, ServerId, ReqCallback)->ProcessRequest();
}

bool UServersService::ListPublicServerModeration(const FString& ServerId, FListPublicServerModerationDelegate& OnComplete)
{
	FHttpRequestCompleteDelegate ReqCallback;
	CreateListPublicServerModerationDelegate(OnComplete, ReqCallback);
	return UDiscoveryAPI::ListPublicServerModeration(this, ServerId, ReqCallback)->ProcessRequest();
}

void UServersService::JoinServer(UServerDataObject* ServerDataObject, const UWorld* World)
{
	if (ServerDataObject)
	{
		UMenuManagerSubsystem::Get().ShowServerDetails(ServerDataObject);
		if (ServerDataObject->HasPassword())
		{
			UMenuManagerSubsystem::Get().ShowServerPasswordModal(ServerDataObject);
		}
		else
		{
			UMenuManagerSubsystem::Get().ShowJoiningServerQueue(World, ServerDataObject);
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
				if (const TArray<TSharedPtr<FJsonValue>>& Data = ResJson.JsonObject->GetArrayField("data"); Data.Num() > 0)
				{
					TArray<UServerDataObject*> Servers;
					for (TSharedPtr<FJsonValue, ESPMode::ThreadSafe> JsonValue : Data)
					{
						UServerDataObject* Server = UServerDataObject::FromJson(JsonValue.Get()->AsObject().ToSharedRef());
						Servers.Add(Server);
					}
					OnComplete.ExecuteIfBound(Servers, true, TOptional<FText>());
				}
				else
				{
					// TODO: Do some kind of error handling or default fallback here
				}
			}
			else
			{
				OnComplete.ExecuteIfBound({}, false, FText::FromString("Response wasn't okay I guess"));
			}
		}
		else
		{
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
						OnComplete.ExecuteIfBound(Server, true, TOptional<FText>());
					}
					else
					{
						TOptional<FText> Error = TOptional<FText>();
						Error.Emplace( FText::FromString("Could not retrieve server details from data object"));
						OnComplete.ExecuteIfBound(nullptr, false, Error);
					}
				}
				else
				{
					TOptional<FText> Error = TOptional<FText>();
					Error.Emplace( FText::FromString("Could not retrieve data object when retrieving server details"));
					OnComplete.ExecuteIfBound(nullptr, false, Error);
				}
			}
			else
			{
				OnComplete.ExecuteIfBound({}, false, FText::FromString("Received non-200 response"));
			}
		}
		else
		{
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
					const FString ErrorText = FString::Printf(TEXT("UServersService::CreateSimpleServiceResponseDelegate: Error code: %d. Message: %ls"), Res.Get()->GetResponseCode(), *Message);
					OnComplete.ExecuteIfBound(false, TOptional<FText>(FText::FromString(ErrorText)));
				}
				else
				{
					const FString ErrorText = FString::Printf(TEXT("UServersService::CreateSimpleServiceResponseDelegate: Received %d response w/out error message"), Res.Get()->GetResponseCode());
					OnComplete.ExecuteIfBound(false, TOptional<FText>(FText::FromString(ErrorText)));
				}
			}
		}
		else
		{
			OnComplete.ExecuteIfBound(false, FText::FromString("UServersService::CreateSimpleServiceResponseDelegate: Failed to connect to server"));
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
					OnComplete.ExecuteIfBound({}, true, FText::FromString("ServersService::CreateListPublicServerModerationDelegate: Error while grabbing data from response"));
				}
			}
			else
			{
				FJsonObjectWrapper ResJson;
				ResJson.JsonObjectFromString(Res.Get()->GetContentAsString());
				if (const FString Message = ResJson.JsonObject->GetStringField("message"); Message.IsEmpty() == false)
				{
					const FString ErrorText = FString::Printf(TEXT("UServersService::CreateListPublicServerModerationDelegate: Error code: %d. Message: %ls"), Res.Get()->GetResponseCode(), *Message);
					OnComplete.ExecuteIfBound({}, false, TOptional<FText>(FText::FromString(ErrorText)));
				}
				else
				{
					const FString ErrorText = FString::Printf(TEXT("UServersService::CreateListPublicServerModerationDelegate: Received %d response w/out error message"), Res.Get()->GetResponseCode());
					OnComplete.ExecuteIfBound({}, false, TOptional<FText>(FText::FromString(ErrorText)));
				}
			}
		}
		else
		{
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
				if (const TSharedPtr<FJsonObject>& Data = ResJson.JsonObject->GetObjectField("data"); Data.IsValid())
				{
					UPlayerListDataObject* PlayerListDataObject;
					
					if (Data->HasField("players"))
					{
						PlayerListDataObject = UPlayerListDataObject::FromJson(Data.ToSharedRef(), "players");
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
		}
		else
		{
			OnComplete.ExecuteIfBound({}, false, FText::FromString("Failed to connect to server"));
		}
	});
}

bool UServersService::GetServerDetails(FGetServerDetailsDelegate& OnComplete, const FString& ServerId)
{
	FHttpRequestCompleteDelegate ReqCallback;
	CreateServerDetailsDelegate(OnComplete, ReqCallback);
	return UDiscoveryAPI::GetServerDetails(this, ServerId, ReqCallback)->ProcessRequest();
}
