// Fill out your copyright notice in the Description page of Project Settings.

#include "API/DiscoveryAPI.h"

TSharedRef<IHttpRequest> UDiscoveryAPI::SendIdentToken(const UObject* WeakContext,
                                                       const FJsonObjectWrapper& RequestBody, FHttpRequestCompleteDelegate& OnComplete)
{
	FHttpRequestCompleteDelegate ReqCallback;
	ReqCallback.BindWeakLambda(WeakContext, [OnComplete](FHttpRequestPtr Req, FHttpResponsePtr Res, bool bConnectedSuccessfully) mutable
	{
		// If we connected successfully, and the response code is 200 OK, always set the player session
		// so that we don't have to repeat this logic in every callback the services provide that use this function
		if (bConnectedSuccessfully)
		{
			if (EHttpResponseCodes::IsOk(Res.Get()->GetResponseCode()))
			{
				FJsonObjectWrapper ResJson;
				ResJson.JsonObjectFromString(Res.Get()->GetContentAsString());
				if (const TSharedPtr<FJsonObject>& Data = ResJson.JsonObject->GetObjectField("data"); Data.IsValid())
				{
					UDiscoverySessionManager& SessionManager = UDiscoverySessionManager::Get();
					SessionManager.SetPlayerSession(FPlayerSession::FromJson(Data));
				}
			}
		}
		OnComplete.ExecuteIfBound(Req, Res, bConnectedSuccessfully);
	});
	
	return MakeRequest(WeakContext, UDiscoveryAPISettings::Get().GetApiRoot() + "ident/token", "POST", UDiscoverySessionManager::Get().GetGameAccessToken(), RequestBody, ReqCallback);
}

TSharedRef<IHttpRequest> UDiscoveryAPI::GetPlayerSession(const UObject* WeakContext,
                                                         FHttpRequestCompleteDelegate& OnComplete)
{
	FJsonObjectWrapper JsonObj;
	return MakeRequest(WeakContext, UDiscoveryAPISettings::Get().GetApiRoot() + "ident/session", "GET", UDiscoverySessionManager::Get().GetPlayerAccessToken(), JsonObj, OnComplete);
}

TSharedRef<IHttpRequest> UDiscoveryAPI::GetPlayerSettings(const UObject* WeakContext,
	FHttpRequestCompleteDelegate& OnComplete)
{
	FJsonObjectWrapper JsonObj;
	return MakeRequest(WeakContext, UDiscoveryAPISettings::Get().GetApiRoot() + "player/settings", "GET",
		UDiscoverySessionManager::Get().GetPlayerAccessToken(), JsonObj, OnComplete);
}

TSharedRef<IHttpRequest> UDiscoveryAPI::UpdatePlayerSettings(const UObject* WeakContext,
	TOptional<bool> AnalyticsOptOut, TOptional<FString> DefaultRegionID, FHttpRequestCompleteDelegate& OnComplete)
{
	FJsonObjectWrapper JsonObj;
	if (AnalyticsOptOut.IsSet())
	{
		JsonObj.JsonObject->SetBoolField("analytics_opt_out", AnalyticsOptOut.GetValue());
	}
	if (DefaultRegionID.IsSet())
	{
		JsonObj.JsonObject->SetStringField("default_server_region_id", DefaultRegionID.GetValue());
	}
	return MakeRequest(WeakContext, UDiscoveryAPISettings::Get().GetApiRoot() +
		"player/settings", "POST", UDiscoverySessionManager::Get().GetPlayerAccessToken(), JsonObj, OnComplete);
}

TSharedRef<IHttpRequest> UDiscoveryAPI::UpdateCurrentPlayerRegion(const UObject* WeakContext, const FString& RegionID,
                                                                  FHttpRequestCompleteDelegate& OnComplete)
{
	FJsonObjectWrapper JsonObj;
	JsonObj.JsonObject->SetStringField("server_region_id", RegionID);
	return MakeRequest(WeakContext, UDiscoveryAPISettings::Get().GetApiRoot() +
		"ident/session", "POST", UDiscoverySessionManager::Get().GetPlayerAccessToken(), JsonObj, OnComplete);
}

TSharedRef<IHttpRequest> UDiscoveryAPI::GameDetails(const UObject* WeakContext,
                                                    FHttpRequestCompleteDelegate& OnComplete)
{
	FJsonObjectWrapper JsonObj;
	return MakeRequest(WeakContext, UDiscoveryAPISettings::Get().GetApiRoot() + "game", "GET", UDiscoverySessionManager::Get().GetGameAccessToken(), JsonObj, OnComplete);
}

TSharedRef<IHttpRequest> UDiscoveryAPI::GetGameLegalConsents(const UObject* WeakContext, const FString& IdentType,
                                                             FHttpRequestCompleteDelegate& OnComplete)
{
	FJsonObjectWrapper JsonObj;
	return MakeRequest(WeakContext, UDiscoveryAPISettings::Get().GetApiRoot() + "consents/game/legal?ident_type=" + IdentType, "GET", UDiscoverySessionManager::Get().GetGameAccessToken(), JsonObj, OnComplete);
}

TSharedRef<IHttpRequest> UDiscoveryAPI::ListPopularServers(const UObject* WeakContext,
                                                           FHttpRequestCompleteDelegate& OnComplete)
{
	FJsonObjectWrapper JsonObj;
	return MakeRequest(WeakContext, UDiscoveryAPISettings::Get().GetApiRoot() + "discovery/popular", "GET", UDiscoverySessionManager::Get().GetPlayerAccessToken(), JsonObj, OnComplete);
}

TSharedRef<IHttpRequest> UDiscoveryAPI::ListRecommendedServers(const UObject* WeakContext,
                                                               FHttpRequestCompleteDelegate& OnComplete)
{
	FJsonObjectWrapper JsonObj;
	return MakeRequest(WeakContext, UDiscoveryAPISettings::Get().GetApiRoot() + "discovery/recommended", "GET", UDiscoverySessionManager::Get().GetPlayerAccessToken(), JsonObj, OnComplete);
}

TSharedRef<IHttpRequest> UDiscoveryAPI::ListPlayerOwnedServers(const UObject* WeakContext,
                                                               FHttpRequestCompleteDelegate& OnComplete)
{
	FJsonObjectWrapper JsonObj;
	return MakeRequest(WeakContext, UDiscoveryAPISettings::Get().GetApiRoot() + "discovery/friends", "GET", UDiscoverySessionManager::Get().GetPlayerAccessToken(), JsonObj, OnComplete);
}

TSharedRef<IHttpRequest> UDiscoveryAPI::ListFavoriteServers(const UObject* WeakContext,
                                                            FHttpRequestCompleteDelegate& OnComplete)
{
	FJsonObjectWrapper JsonObj;
	return MakeRequest(WeakContext, UDiscoveryAPISettings::Get().GetApiRoot() + "discovery/favorite", "GET", UDiscoverySessionManager::Get().GetPlayerAccessToken(), JsonObj, OnComplete);
}

TSharedRef<IHttpRequest> UDiscoveryAPI::ListInvitableServers(const UObject* WeakContext, const FString& InviteeID,
                                                             FHttpRequestCompleteDelegate& OnComplete)
{
	FJsonObjectWrapper JsonObj;
	return MakeRequest(WeakContext, UDiscoveryAPISettings::Get().GetApiRoot() + "discovery/friend/" + InviteeID,
		"GET", UDiscoverySessionManager::Get().GetPlayerAccessToken(), JsonObj, OnComplete);
}

TSharedRef<IHttpRequest> UDiscoveryAPI::GetServerQueue(const UObject* WeakContext, const FString& Token,
FHttpRequestCompleteDelegate& OnComplete)
{
	FJsonObjectWrapper JsonObj;
	return MakeRequest(WeakContext, UDiscoveryAPISettings::Get().GetApiRoot() + "queue/" + Token,
		"GET", UDiscoverySessionManager::Get().GetPlayerAccessToken(), JsonObj, OnComplete);
}

TSharedRef<IHttpRequest> UDiscoveryAPI::RenewServerQueue(const UObject* WeakContext, const FString& Token,
	FHttpRequestCompleteDelegate& OnComplete)
{
	FJsonObjectWrapper JsonObj;
	return MakeRequest(WeakContext, UDiscoveryAPISettings::Get().GetApiRoot() + "queue/" + Token,
		"POST", UDiscoverySessionManager::Get().GetPlayerAccessToken(), JsonObj, OnComplete);
}

TSharedRef<IHttpRequest> UDiscoveryAPI::CancelServerQueue(const UObject* WeakContext, const FString& Token,
	FHttpRequestCompleteDelegate& OnComplete)
{
	FJsonObjectWrapper JsonObj;
	return MakeRequest(WeakContext, UDiscoveryAPISettings::Get().GetApiRoot() + "queue/" + Token,
		"DELETE", UDiscoverySessionManager::Get().GetPlayerAccessToken(), JsonObj, OnComplete);
}

TSharedRef<IHttpRequest> UDiscoveryAPI::JoinServer(const UObject* WeakContext, const FString& ServerID, TOptional<FString> ServerPassword,
	FHttpRequestCompleteDelegate& OnComplete)
{
	FJsonObjectWrapper JsonObj;
	if (ServerPassword.IsSet())
	{
		JsonObj.JsonObject->SetStringField("password", ServerPassword.GetValue());
	}
	return MakeRequest(WeakContext, UDiscoveryAPISettings::Get().GetApiRoot() + "server/" + ServerID + "/join",
		"POST", UDiscoverySessionManager::Get().GetPlayerAccessToken(), JsonObj, OnComplete);
}

TSharedRef<IHttpRequest> UDiscoveryAPI::GetRecentPlayers(const UObject* WeakContext, const FString& ServerId,
                                                         FHttpRequestCompleteDelegate& OnComplete)
{
	FJsonObjectWrapper JsonObj;
	return MakeRequest(WeakContext, UDiscoveryAPISettings::Get().GetApiRoot() + "server/" + ServerId + "/players/recent", "GET", UDiscoverySessionManager::Get().GetPlayerAccessToken(), JsonObj, OnComplete);
}

TSharedRef<IHttpRequest> UDiscoveryAPI::GetOnlinePlayers(const UObject* WeakContext, const FString& ServerId,
                                                         FHttpRequestCompleteDelegate& OnComplete)
{
	FJsonObjectWrapper JsonObj;
	return MakeRequest(WeakContext, UDiscoveryAPISettings::Get().GetApiRoot() + "server/" + ServerId + "/players", "GET", UDiscoverySessionManager::Get().GetPlayerAccessToken(), JsonObj, OnComplete);
}

TSharedRef<IHttpRequest> UDiscoveryAPI::GetNotifications(const UObject* WeakContext,
	FHttpRequestCompleteDelegate& OnComplete)
{
	FJsonObjectWrapper JsonObj;
	return MakeRequest(WeakContext, UDiscoveryAPISettings::Get().GetApiRoot() + "notifications", "GET", UDiscoverySessionManager::Get().GetPlayerAccessToken(), JsonObj, OnComplete);
}

TSharedRef<IHttpRequest> UDiscoveryAPI::GetNotificationsHistory(const UObject* WeakContext,
	FHttpRequestCompleteDelegate& OnComplete)
{
	FJsonObjectWrapper JsonObj;
	return MakeRequest(WeakContext, UDiscoveryAPISettings::Get().GetApiRoot() + "notifications/history", "GET", UDiscoverySessionManager::Get().GetPlayerAccessToken(), JsonObj, OnComplete);
}

TSharedRef<IHttpRequest> UDiscoveryAPI::DismissNotification(const UObject* WeakContext, const FString& NotificationId,
	FHttpRequestCompleteDelegate& OnComplete)
{
	FJsonObjectWrapper JsonObj;
	return MakeRequest(WeakContext, UDiscoveryAPISettings::Get().GetApiRoot() + "notifications/" + NotificationId + "/dismiss", "POST", UDiscoverySessionManager::Get().GetPlayerAccessToken(), JsonObj, OnComplete);
}

TSharedRef<IHttpRequest> UDiscoveryAPI::DismissNotifications(const UObject* WeakContext, const TArray<FString> NotificationIds,
	FHttpRequestCompleteDelegate& OnComplete)
{
	FJsonObjectWrapper JsonObj;
	TArray<TSharedPtr<FJsonValue>> NotificationIdsJson;
	for (const FString& NotificationId : NotificationIds)
	{
		NotificationIdsJson.Add(MakeShared<FJsonValueString>(NotificationId));
	}
	JsonObj.JsonObject->SetArrayField("ids", NotificationIdsJson);
	return MakeRequest(WeakContext, UDiscoveryAPISettings::Get().GetApiRoot() + "notifications/dismiss", "POST", UDiscoverySessionManager::Get().GetPlayerAccessToken(), JsonObj, OnComplete);
}

TSharedRef<IHttpRequest> UDiscoveryAPI::ListFriends(const UObject* WeakContext,
	FHttpRequestCompleteDelegate& OnComplete)
{
	FJsonObjectWrapper JsonObj;
	return MakeRequest(WeakContext, UDiscoveryAPISettings::Get().GetApiRoot() + "friends", "GET", UDiscoverySessionManager::Get().GetPlayerAccessToken(), JsonObj, OnComplete);
}

TSharedRef<IHttpRequest> UDiscoveryAPI::UpdateInviteStatus(const UObject* WeakContext, const FString& InviteId,
                                                           const ENodecraftInviteStatus Status, FHttpRequestCompleteDelegate& OnComplete)
{
	FJsonObjectWrapper JsonObj;
	FString StatusString;
	switch (Status)
	{
	case ENodecraftInviteStatus::Accepted:
		StatusString = "accepted";
		break;
	case ENodecraftInviteStatus::Declined:
		StatusString = "declined";
		break;
	case ENodecraftInviteStatus::Pending:
		StatusString = "pending";
		break;
	case ENodecraftInviteStatus::Revoked:
		StatusString = "revoked";
		break;
	}
	return MakeRequest(WeakContext, UDiscoveryAPISettings::Get().GetApiRoot() + "invites/" + InviteId + "/" + StatusString, "PUT", UDiscoverySessionManager::Get().GetPlayerAccessToken(), JsonObj, OnComplete);
}

TSharedRef<IHttpRequest> UDiscoveryAPI::GetOwner(const UObject* WeakContext, const FString& ServerId,
	FHttpRequestCompleteDelegate& OnComplete)
{
	FJsonObjectWrapper JsonObj;
	return MakeRequest(WeakContext, UDiscoveryAPISettings::Get().GetApiRoot() + "server/" + ServerId + "/moderator/owner", "GET", UDiscoverySessionManager::Get().GetPlayerAccessToken(), JsonObj, OnComplete);
}

TSharedRef<IHttpRequest> UDiscoveryAPI::GetModerators(const UObject* WeakContext, const FString& ServerId,
	FHttpRequestCompleteDelegate& OnComplete)
{
	FJsonObjectWrapper JsonObj;
	return MakeRequest(WeakContext, UDiscoveryAPISettings::Get().GetApiRoot() + "server/" + ServerId + "/moderator/staff", "GET", UDiscoverySessionManager::Get().GetPlayerAccessToken(), JsonObj, OnComplete);
}

TSharedRef<IHttpRequest> UDiscoveryAPI::GetOnlinePlayersForModeration(const UObject* WeakContext,
	const FString& ServerId, FHttpRequestCompleteDelegate& OnComplete)
{
	FJsonObjectWrapper JsonObj;
	return MakeRequest(WeakContext, UDiscoveryAPISettings::Get().GetApiRoot() + "server/" + ServerId + "/moderator/online", "GET", UDiscoverySessionManager::Get().GetPlayerAccessToken(), JsonObj, OnComplete);
}

TSharedRef<IHttpRequest> UDiscoveryAPI::GetOfflinePlayersForModeration(const UObject* WeakContext,
	const FString& ServerId, FHttpRequestCompleteDelegate& OnComplete)
{
	FJsonObjectWrapper JsonObj;
	return MakeRequest(WeakContext, UDiscoveryAPISettings::Get().GetApiRoot() + "server/" + ServerId + "/moderator/offline", "GET", UDiscoverySessionManager::Get().GetPlayerAccessToken(), JsonObj, OnComplete);
}

TSharedRef<IHttpRequest> UDiscoveryAPI::GetBannedPlayers(const UObject* WeakContext, const FString& ServerId,
	FHttpRequestCompleteDelegate& OnComplete)
{
	FJsonObjectWrapper JsonObj;
	return MakeRequest(WeakContext, UDiscoveryAPISettings::Get().GetApiRoot() + "server/" + ServerId + "/moderator/banned", "GET", UDiscoverySessionManager::Get().GetPlayerAccessToken(), JsonObj, OnComplete);
}

TSharedRef<IHttpRequest> UDiscoveryAPI::UpdateAllowStatus(const UObject* WeakContext, const FString& AllowId,
                                                          const EAllowStatus Status, FHttpRequestCompleteDelegate& OnComplete)
{
	FJsonObjectWrapper JsonObj;
	FString StatusString;
	switch (Status)
	{
	case EAllowStatus::Accepted:
		StatusString = "accepted";
		break;
	case EAllowStatus::Declined:
		StatusString = "declined";
		break;
	case EAllowStatus::Pending:
		StatusString = "pending";
		break;
	case EAllowStatus::Revoked:
		StatusString = "revoked";
		break;
	}
	return MakeRequest(WeakContext, UDiscoveryAPISettings::Get().GetApiRoot() + "allows/" + AllowId + "/" + StatusString, "PUT", UDiscoverySessionManager::Get().GetPlayerAccessToken(), JsonObj, OnComplete);
}

TSharedRef<IHttpRequest> UDiscoveryAPI::ListPlayerAllows(const UObject* WeakContext,
	FHttpRequestCompleteDelegate& OnComplete)
{
	FJsonObjectWrapper JsonObj;
	return MakeRequest(WeakContext, UDiscoveryAPISettings::Get().GetApiRoot() + "allows",
		"GET", UDiscoverySessionManager::Get().GetPlayerAccessToken(), JsonObj, OnComplete);
}

TSharedRef<IHttpRequest> UDiscoveryAPI::GetServerDetails(const UObject* WeakContext, const FString& ServerId,
                                                         FHttpRequestCompleteDelegate& OnComplete)
{
	FJsonObjectWrapper JsonObj;
	return MakeRequest(WeakContext, UDiscoveryAPISettings::Get().GetApiRoot() + "server/" + ServerId, "GET", UDiscoverySessionManager::Get().GetPlayerAccessToken(), JsonObj, OnComplete);
}

TSharedRef<IHttpRequest> UDiscoveryAPI::FavoriteServer(const UObject* WeakContext, const FString& ServerId,
	FHttpRequestCompleteDelegate& OnComplete)
{
	FJsonObjectWrapper JsonObj;
	return MakeRequest(WeakContext, UDiscoveryAPISettings::Get().GetApiRoot() + "server/" + ServerId + "/favorite",
		"POST", UDiscoverySessionManager::Get().GetPlayerAccessToken(), JsonObj, OnComplete);
}

TSharedRef<IHttpRequest> UDiscoveryAPI::UnfavoriteServer(const UObject* WeakContext, const FString& ServerId,
	FHttpRequestCompleteDelegate& OnComplete)
{
	FJsonObjectWrapper JsonObj;
	return MakeRequest(WeakContext, UDiscoveryAPISettings::Get().GetApiRoot() + "server/" + ServerId + "/favorite",
		"DELETE", UDiscoverySessionManager::Get().GetPlayerAccessToken(), JsonObj, OnComplete);
}

TSharedRef<IHttpRequest> UDiscoveryAPI::ListPublicServerModeration(const UObject* WeakContext, const FString& ServerId,
	FHttpRequestCompleteDelegate& OnComplete)
{
	FJsonObjectWrapper JsonObj;
	return MakeRequest(WeakContext, UDiscoveryAPISettings::Get().GetApiRoot() + "server/" + ServerId + "/moderation",
		"GET", UDiscoverySessionManager::Get().GetPlayerAccessToken(), JsonObj, OnComplete);
}

TSharedRef<IHttpRequest> UDiscoveryAPI::GetPlayerReputation(const UObject* WeakContext,
	FHttpRequestCompleteDelegate& OnComplete)
{
	FJsonObjectWrapper JsonObj;
	return MakeRequest(WeakContext, UDiscoveryAPISettings::Get().GetApiRoot() + "reputation/bans",
		"GET", UDiscoverySessionManager::Get().GetPlayerAccessToken(), JsonObj, OnComplete);
}

TSharedRef<IHttpRequest> UDiscoveryAPI::SignConsents(const UObject* WeakContext, const FGameConsents& Consents,
                                                     FHttpRequestCompleteDelegate& OnComplete)
{
	FJsonObjectWrapper JsonObj;

	FString JsonStr;
	TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> JsonWriter = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonStr, 0);
	FJsonSerializer::Serialize(MakeShared<FJsonValueArray>(Consents.ToJsonArray()), FString(), JsonWriter);
	
	return MakeRequest(WeakContext, UDiscoveryAPISettings::Get().GetApiRoot() + "consents/player/" + Consents.GetConsentType(), "POST", UDiscoverySessionManager::Get().GetPlayerAccessToken(), JsonStr, OnComplete);
}

TSharedRef<IHttpRequest> UDiscoveryAPI::GetPlayerLegalConsents(const UObject* WeakContext,
	FHttpRequestCompleteDelegate& OnComplete)
{
	FJsonObjectWrapper JsonObj;
	return MakeRequest(WeakContext, UDiscoveryAPISettings::Get().GetApiRoot() + "consents/player/legal",
		"GET", UDiscoverySessionManager::Get().GetPlayerAccessToken(), JsonObj, OnComplete);
}

TSharedRef<IHttpRequest> UDiscoveryAPI::GetPlayerRulesConsents(const UObject* WeakContext,
	FHttpRequestCompleteDelegate& OnComplete)
{
	FJsonObjectWrapper JsonObj;
	return MakeRequest(WeakContext, UDiscoveryAPISettings::Get().GetApiRoot() + "consents/player/rules",
		"GET", UDiscoverySessionManager::Get().GetPlayerAccessToken(), JsonObj, OnComplete);
}

TSharedRef<IHttpRequest> UDiscoveryAPI::MakeRequest(const UObject* WeakContext, FString RequestURI, FString HttpMethod,
                                                    FString HeaderToken, const FString& JsonText, FHttpRequestCompleteDelegate& OnCompleteCallback)
{
	FHttpModule& HTTPModule = FHttpModule::Get();

	TSharedRef<IHttpRequest> Request = HTTPModule.CreateRequest();

	Request->SetVerb(HttpMethod);

	// We'll need to tell the server what type of content to expect in the POST data
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	Request->SetHeader(TEXT(DISCOVERY_API_HEADER_NODECRAFT_TOKEN), HeaderToken);

	// SET BODY
	if (HttpMethod.Equals("Get", ESearchCase::IgnoreCase) == false)
	{
		Request->SetContentAsString(JsonText);
	}
		
	Request->SetURL(RequestURI);

	Request->OnProcessRequestComplete().BindWeakLambda(WeakContext, [OnCompleteCallback](FHttpRequestPtr Req, FHttpResponsePtr Res, bool bConnectedSuccessfully)
	{
		OnCompleteCallback.ExecuteIfBound(Req, Res, bConnectedSuccessfully);
	});

	return Request;
}

TSharedRef<IHttpRequest> UDiscoveryAPI::MakeRequest(const UObject* WeakContext, FString RequestURI, FString HttpMethod,
                                                    FString HeaderToken, const FJsonObjectWrapper& RequestBody, FHttpRequestCompleteDelegate& OnCompleteCallback)
{
	FString JsonStr;
	RequestBody.JsonObjectToString(JsonStr);
	return MakeRequest(WeakContext, RequestURI, HttpMethod, HeaderToken, JsonStr, OnCompleteCallback);
}
