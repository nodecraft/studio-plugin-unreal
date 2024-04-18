// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#include "API/NodecraftStudioApi.h"

#include "Models/Server/ServerStatus.h"

TSharedRef<IHttpRequest> UNodecraftStudioApi::SendIdentToken(const UObject* WeakContext,
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
					UNodecraftStudioSessionManager& SessionManager = UNodecraftStudioSessionManager::Get();
					SessionManager.SetPlayerSession(FPlayerSession::FromJson(Data));
				}
			}
		}
		OnComplete.ExecuteIfBound(Req, Res, bConnectedSuccessfully);
	});
	
	return MakeRequest(WeakContext, UNodecraftStudioApiSettings::Get().GetApiRoot() + "ident/token", "POST", UNodecraftStudioSessionManager::Get().GetGameAccessToken(), RequestBody, ReqCallback);
}

TSharedRef<IHttpRequest> UNodecraftStudioApi::GetPlayerSession(const UObject* WeakContext,
                                                         FHttpRequestCompleteDelegate& OnComplete)
{
	FJsonObjectWrapper JsonObj;
	return MakeRequest(WeakContext, UNodecraftStudioApiSettings::Get().GetApiRoot() + "ident/session", "GET", UNodecraftStudioSessionManager::Get().GetPlayerAccessToken(), JsonObj, OnComplete);
}

TSharedRef<IHttpRequest> UNodecraftStudioApi::GetPlayerSettings(const UObject* WeakContext,
	FHttpRequestCompleteDelegate& OnComplete)
{
	FJsonObjectWrapper JsonObj;
	return MakeRequest(WeakContext, UNodecraftStudioApiSettings::Get().GetApiRoot() + "player/settings", "GET",
		UNodecraftStudioSessionManager::Get().GetPlayerAccessToken(), JsonObj, OnComplete);
}

TSharedRef<IHttpRequest> UNodecraftStudioApi::UpdatePlayerSettings(const UObject* WeakContext,
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
	return MakeRequest(WeakContext, UNodecraftStudioApiSettings::Get().GetApiRoot() +
		"player/settings", "POST", UNodecraftStudioSessionManager::Get().GetPlayerAccessToken(), JsonObj, OnComplete);
}

TSharedRef<IHttpRequest> UNodecraftStudioApi::UpdateCurrentPlayerRegion(const UObject* WeakContext, const FString& RegionID,
                                                                  FHttpRequestCompleteDelegate& OnComplete)
{
	FJsonObjectWrapper JsonObj;
	JsonObj.JsonObject->SetStringField("server_region_id", RegionID);
	return MakeRequest(WeakContext, UNodecraftStudioApiSettings::Get().GetApiRoot() +
		"ident/session", "POST", UNodecraftStudioSessionManager::Get().GetPlayerAccessToken(), JsonObj, OnComplete);
}

TSharedRef<IHttpRequest> UNodecraftStudioApi::GameDetails(const UObject* WeakContext,
                                                    FHttpRequestCompleteDelegate& OnComplete)
{
	FJsonObjectWrapper JsonObj;
	return MakeRequest(WeakContext, UNodecraftStudioApiSettings::Get().GetApiRoot() + "game", "GET", UNodecraftStudioSessionManager::Get().GetGameAccessToken(), JsonObj, OnComplete);
}

TSharedRef<IHttpRequest> UNodecraftStudioApi::GetGameLegalConsents(const UObject* WeakContext, const FString& IdentType,
                                                             FHttpRequestCompleteDelegate& OnComplete)
{
	FJsonObjectWrapper JsonObj;
	return MakeRequest(WeakContext, UNodecraftStudioApiSettings::Get().GetApiRoot() + "consents/game/legal?ident_type=" + IdentType, "GET", UNodecraftStudioSessionManager::Get().GetGameAccessToken(), JsonObj, OnComplete);
}

TSharedRef<IHttpRequest> UNodecraftStudioApi::ListPopularServers(const UObject* WeakContext,
                                                           FHttpRequestCompleteDelegate& OnComplete)
{
	FJsonObjectWrapper JsonObj;
	return MakeRequest(WeakContext, UNodecraftStudioApiSettings::Get().GetApiRoot() + "discovery/popular", "GET", UNodecraftStudioSessionManager::Get().GetPlayerAccessToken(), JsonObj, OnComplete);
}

TSharedRef<IHttpRequest> UNodecraftStudioApi::ListRecommendedServers(const UObject* WeakContext,
                                                               FHttpRequestCompleteDelegate& OnComplete)
{
	FJsonObjectWrapper JsonObj;
	return MakeRequest(WeakContext, UNodecraftStudioApiSettings::Get().GetApiRoot() + "discovery/recommended", "GET", UNodecraftStudioSessionManager::Get().GetPlayerAccessToken(), JsonObj, OnComplete);
}

TSharedRef<IHttpRequest> UNodecraftStudioApi::ListPlayerOwnedServers(const UObject* WeakContext,
                                                               FHttpRequestCompleteDelegate& OnComplete)
{
	FJsonObjectWrapper JsonObj;
	return MakeRequest(WeakContext, UNodecraftStudioApiSettings::Get().GetApiRoot() + "discovery/friends", "GET", UNodecraftStudioSessionManager::Get().GetPlayerAccessToken(), JsonObj, OnComplete);
}

TSharedRef<IHttpRequest> UNodecraftStudioApi::ListFavoriteServers(const UObject* WeakContext,
                                                            FHttpRequestCompleteDelegate& OnComplete)
{
	FJsonObjectWrapper JsonObj;
	return MakeRequest(WeakContext, UNodecraftStudioApiSettings::Get().GetApiRoot() + "discovery/favorite", "GET", UNodecraftStudioSessionManager::Get().GetPlayerAccessToken(), JsonObj, OnComplete);
}

TSharedRef<IHttpRequest> UNodecraftStudioApi::ListInvitableServers(const UObject* WeakContext, const FString& InviteeID,
                                                             FHttpRequestCompleteDelegate& OnComplete)
{
	FJsonObjectWrapper JsonObj;
	return MakeRequest(WeakContext, UNodecraftStudioApiSettings::Get().GetApiRoot() + "discovery/friend/" + InviteeID,
		"GET", UNodecraftStudioSessionManager::Get().GetPlayerAccessToken(), JsonObj, OnComplete);
}

TSharedRef<IHttpRequest> UNodecraftStudioApi::GetServerQueue(const UObject* WeakContext, const FString& Token,
FHttpRequestCompleteDelegate& OnComplete)
{
	FJsonObjectWrapper JsonObj;
	return MakeRequest(WeakContext, UNodecraftStudioApiSettings::Get().GetApiRoot() + "queue/" + Token,
		"GET", UNodecraftStudioSessionManager::Get().GetPlayerAccessToken(), JsonObj, OnComplete);
}

TSharedRef<IHttpRequest> UNodecraftStudioApi::RenewServerQueue(const UObject* WeakContext, const FString& Token,
	FHttpRequestCompleteDelegate& OnComplete)
{
	FJsonObjectWrapper JsonObj;
	return MakeRequest(WeakContext, UNodecraftStudioApiSettings::Get().GetApiRoot() + "queue/" + Token,
		"POST", UNodecraftStudioSessionManager::Get().GetPlayerAccessToken(), JsonObj, OnComplete);
}

TSharedRef<IHttpRequest> UNodecraftStudioApi::CancelServerQueue(const UObject* WeakContext, const FString& Token,
	FHttpRequestCompleteDelegate& OnComplete)
{
	FJsonObjectWrapper JsonObj;
	return MakeRequest(WeakContext, UNodecraftStudioApiSettings::Get().GetApiRoot() + "queue/" + Token,
		"DELETE", UNodecraftStudioSessionManager::Get().GetPlayerAccessToken(), JsonObj, OnComplete);
}

TSharedRef<IHttpRequest> UNodecraftStudioApi::JoinServer(const UObject* WeakContext, const FString& ServerID, TOptional<FString> ServerPassword,
	FHttpRequestCompleteDelegate& OnComplete)
{
	FJsonObjectWrapper JsonObj;
	if (ServerPassword.IsSet())
	{
		JsonObj.JsonObject->SetStringField("password", ServerPassword.GetValue());
	}
	return MakeRequest(WeakContext, UNodecraftStudioApiSettings::Get().GetApiRoot() + "server/" + ServerID + "/join",
		"POST", UNodecraftStudioSessionManager::Get().GetPlayerAccessToken(), JsonObj, OnComplete);
}

TSharedRef<IHttpRequest> UNodecraftStudioApi::GetRecentPlayers(const UObject* WeakContext, const FString& ServerId,
                                                         FHttpRequestCompleteDelegate& OnComplete)
{
	FJsonObjectWrapper JsonObj;
	return MakeRequest(WeakContext, UNodecraftStudioApiSettings::Get().GetApiRoot() + "server/" + ServerId + "/players/recent", "GET", UNodecraftStudioSessionManager::Get().GetPlayerAccessToken(), JsonObj, OnComplete);
}

TSharedRef<IHttpRequest> UNodecraftStudioApi::GetOnlinePlayers(const UObject* WeakContext, const FString& ServerId,
                                                         FHttpRequestCompleteDelegate& OnComplete)
{
	FJsonObjectWrapper JsonObj;
	return MakeRequest(WeakContext, UNodecraftStudioApiSettings::Get().GetApiRoot() + "server/" + ServerId + "/players", "GET", UNodecraftStudioSessionManager::Get().GetPlayerAccessToken(), JsonObj, OnComplete);
}

TSharedRef<IHttpRequest> UNodecraftStudioApi::GetNotifications(const UObject* WeakContext,
	FHttpRequestCompleteDelegate& OnComplete)
{
	FJsonObjectWrapper JsonObj;
	return MakeRequest(WeakContext, UNodecraftStudioApiSettings::Get().GetApiRoot() + "notifications", "GET", UNodecraftStudioSessionManager::Get().GetPlayerAccessToken(), JsonObj, OnComplete);
}

TSharedRef<IHttpRequest> UNodecraftStudioApi::GetNotificationsHistory(const UObject* WeakContext,
	FHttpRequestCompleteDelegate& OnComplete)
{
	FJsonObjectWrapper JsonObj;
	return MakeRequest(WeakContext, UNodecraftStudioApiSettings::Get().GetApiRoot() + "notifications/history", "GET", UNodecraftStudioSessionManager::Get().GetPlayerAccessToken(), JsonObj, OnComplete);
}

TSharedRef<IHttpRequest> UNodecraftStudioApi::DismissNotification(const UObject* WeakContext, const FString& NotificationId,
	FHttpRequestCompleteDelegate& OnComplete)
{
	FJsonObjectWrapper JsonObj;
	return MakeRequest(WeakContext, UNodecraftStudioApiSettings::Get().GetApiRoot() + "notifications/" + NotificationId + "/dismiss", "POST", UNodecraftStudioSessionManager::Get().GetPlayerAccessToken(), JsonObj, OnComplete);
}

TSharedRef<IHttpRequest> UNodecraftStudioApi::DismissNotifications(const UObject* WeakContext, const TArray<FString> NotificationIds,
	FHttpRequestCompleteDelegate& OnComplete)
{
	FJsonObjectWrapper JsonObj;
	TArray<TSharedPtr<FJsonValue>> NotificationIdsJson;
	for (const FString& NotificationId : NotificationIds)
	{
		NotificationIdsJson.Add(MakeShared<FJsonValueString>(NotificationId));
	}
	JsonObj.JsonObject->SetArrayField("ids", NotificationIdsJson);
	return MakeRequest(WeakContext, UNodecraftStudioApiSettings::Get().GetApiRoot() + "notifications/dismiss", "POST", UNodecraftStudioSessionManager::Get().GetPlayerAccessToken(), JsonObj, OnComplete);
}

TSharedRef<IHttpRequest> UNodecraftStudioApi::ListFriends(const UObject* WeakContext,
	FHttpRequestCompleteDelegate& OnComplete)
{
	FJsonObjectWrapper JsonObj;
	return MakeRequest(WeakContext, UNodecraftStudioApiSettings::Get().GetApiRoot() + "friends", "GET", UNodecraftStudioSessionManager::Get().GetPlayerAccessToken(), JsonObj, OnComplete);
}

TSharedRef<IHttpRequest> UNodecraftStudioApi::UpdateInviteStatus(const UObject* WeakContext, const FString& InviteId,
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
	return MakeRequest(WeakContext, UNodecraftStudioApiSettings::Get().GetApiRoot() + "invites/" + InviteId + "/" + StatusString, "PUT", UNodecraftStudioSessionManager::Get().GetPlayerAccessToken(), JsonObj, OnComplete);
}

TSharedRef<IHttpRequest> UNodecraftStudioApi::CreateInvite(const UObject* WeakContext, const ::FString& ServerID, const ::FString& FriendID, FHttpRequestCompleteDelegate& OnComplete)
{
	FJsonObjectWrapper JsonObj;
	JsonObj.JsonObject->SetStringField("server_id", ServerID);
	JsonObj.JsonObject->SetStringField("player_id", FriendID);
	return MakeRequest(WeakContext, UNodecraftStudioApiSettings::Get().GetApiRoot() + "invites/create",
		"POST", UNodecraftStudioSessionManager::Get().GetPlayerAccessToken(), JsonObj, OnComplete);
}

TSharedRef<IHttpRequest> UNodecraftStudioApi::GetOwner(const UObject* WeakContext, const FString& ServerId,
                                                 FHttpRequestCompleteDelegate& OnComplete)
{
	FJsonObjectWrapper JsonObj;
	return MakeRequest(WeakContext, UNodecraftStudioApiSettings::Get().GetApiRoot() + "server/" + ServerId + "/moderator/owner", "GET", UNodecraftStudioSessionManager::Get().GetPlayerAccessToken(), JsonObj, OnComplete);
}

TSharedRef<IHttpRequest> UNodecraftStudioApi::GetModerators(const UObject* WeakContext, const FString& ServerId,
	FHttpRequestCompleteDelegate& OnComplete)
{
	FJsonObjectWrapper JsonObj;
	return MakeRequest(WeakContext, UNodecraftStudioApiSettings::Get().GetApiRoot() + "server/" + ServerId + "/moderator/staff", "GET", UNodecraftStudioSessionManager::Get().GetPlayerAccessToken(), JsonObj, OnComplete);
}

TSharedRef<IHttpRequest> UNodecraftStudioApi::GetOnlinePlayersForModeration(const UObject* WeakContext,
	const FString& ServerId, FHttpRequestCompleteDelegate& OnComplete)
{
	FJsonObjectWrapper JsonObj;
	return MakeRequest(WeakContext, UNodecraftStudioApiSettings::Get().GetApiRoot() + "server/" + ServerId + "/moderator/online", "GET", UNodecraftStudioSessionManager::Get().GetPlayerAccessToken(), JsonObj, OnComplete);
}

TSharedRef<IHttpRequest> UNodecraftStudioApi::GetOfflinePlayersForModeration(const UObject* WeakContext,
	const FString& ServerId, FHttpRequestCompleteDelegate& OnComplete)
{
	FJsonObjectWrapper JsonObj;
	return MakeRequest(WeakContext, UNodecraftStudioApiSettings::Get().GetApiRoot() + "server/" + ServerId + "/moderator/offline", "GET", UNodecraftStudioSessionManager::Get().GetPlayerAccessToken(), JsonObj, OnComplete);
}

TSharedRef<IHttpRequest> UNodecraftStudioApi::GetBannedPlayers(const UObject* WeakContext, const FString& ServerId,
	FHttpRequestCompleteDelegate& OnComplete)
{
	FJsonObjectWrapper JsonObj;
	return MakeRequest(WeakContext, UNodecraftStudioApiSettings::Get().GetApiRoot() + "server/" + ServerId + "/moderator/banned", "GET", UNodecraftStudioSessionManager::Get().GetPlayerAccessToken(), JsonObj, OnComplete);
}

TSharedRef<IHttpRequest> UNodecraftStudioApi::SendHeartbeat(const UObject* WeakContext,
	const FHeartbeatRequestDataObject& HeartbeatRequestDataObject, FHttpRequestCompleteDelegate& OnComplete)
{
	FJsonObjectWrapper JsonObj;

	JsonObj.JsonObject->SetStringField("status", HeartbeatRequestDataObject.Status == EServerStatus::Online ? "online" : "offline");
	
	TArray<TSharedPtr<FJsonValue>> SessionIDs;
	if (HeartbeatRequestDataObject.Sessions.Num() > 0)
	{
		for (const FString& SessionId : HeartbeatRequestDataObject.Sessions)
		{
			SessionIDs.Add(MakeShared<FJsonValueString>(SessionId));
		}
	}
	JsonObj.JsonObject->SetArrayField("sessions", SessionIDs);

	// if (HeartbeatRequestDataObject.Since.GetTicks() > 0)
	// {
		JsonObj.JsonObject->SetStringField("since", HeartbeatRequestDataObject.Since.ToIso8601());
	// }
	// else
	// {
	// 	JsonObj.JsonObject->SetStringField("since", FDateTime().ToIso8601());
	// }
	
	return MakeRequest(WeakContext, UNodecraftStudioApiSettings::Get().GetApiRoot() + "game-server/heartbeat", "POST", UNodecraftStudioSessionManager::Get().GetGameServerAccessToken(), JsonObj, OnComplete);
}

TSharedRef<IHttpRequest> UNodecraftStudioApi::SetServerStatus(const UObject* WeakContext, const bool bIsServerEnabled,
	FHttpRequestCompleteDelegate& OnComplete)
{
	FJsonObjectWrapper JsonObj;
	return MakeRequest(WeakContext, UNodecraftStudioApiSettings::Get().GetApiRoot() + "game-server/status/" + (bIsServerEnabled ? "online" : "offline"), "POST", UNodecraftStudioSessionManager::Get().GetPlayerAccessToken(), JsonObj, OnComplete);
}

TSharedRef<IHttpRequest> UNodecraftStudioApi::ValidatePlayerOnJoin(const UObject* WeakContext,
	const FPlayerJoinRequestData& ReqData, FHttpRequestCompleteDelegate& OnComplete)
{
	FJsonObjectWrapper JsonObj;
	JsonObj.JsonObject->SetStringField("token", ReqData.Token);
	if (ReqData.AnalyticsToken.IsSet())
	{
		JsonObj.JsonObject->SetStringField("analytics_token", ReqData.AnalyticsToken.GetValue());
	}
	JsonObj.JsonObject->SetStringField("ip", ReqData.Ip);
	JsonObj.JsonObject->SetStringField("agent", ReqData.Agent);
	JsonObj.JsonObject->SetStringField("agent_version", ReqData.AgentVersion);

	FString JsonStr;
	TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> JsonWriter = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonStr, 0);
	FJsonSerializer::Serialize(MakeShared<FJsonValueObject>(JsonObj.JsonObject), FString(), JsonWriter);

	return MakeRequest(WeakContext, UNodecraftStudioApiSettings::Get().GetApiRoot() + "game-server/player/join", "POST", UNodecraftStudioSessionManager::Get().GetGameServerAccessToken(), JsonObj, OnComplete);
}

TSharedRef<IHttpRequest> UNodecraftStudioApi::TrackPlayerLeft(const UObject* WeakContext,
	const FPlayerLeaveRequestData& ReqData, FHttpRequestCompleteDelegate& OnComplete)
{
	FJsonObjectWrapper JsonObj;
	JsonObj.JsonObject->SetStringField("session_id", ReqData.SessionId);
	if (ReqData.AnalyticsSessionId.IsSet())
	{
		JsonObj.JsonObject->SetStringField("analytics_session_id", ReqData.AnalyticsSessionId.GetValue());
	}
	if (ReqData.SessionEndMessage.IsSet())
	{
		JsonObj.JsonObject->SetStringField("session_end_message", ReqData.SessionEndMessage.GetValue());
	}
	JsonObj.JsonObject->SetStringField("ip", ReqData.Ip);

	JsonObj.JsonObject->SetBoolField("reconnecting", ReqData.bIsAwaitingReconnection);
	
	return MakeRequest(WeakContext, UNodecraftStudioApiSettings::Get().GetApiRoot() + "game-server/player/leave", "POST", UNodecraftStudioSessionManager::Get().GetGameServerAccessToken(), JsonObj, OnComplete);
}

TSharedRef<IHttpRequest> UNodecraftStudioApi::TrackPlayerKick(const UObject* WeakContext, const FString& SessionId,
	const FString& KickId, FHttpRequestCompleteDelegate& OnComplete)
{
	FJsonObjectWrapper JsonObj;
	JsonObj.JsonObject->SetStringField("session_id", SessionId);
	JsonObj.JsonObject->SetStringField("kick_id", KickId);
	
	return MakeRequest(WeakContext, UNodecraftStudioApiSettings::Get().GetApiRoot() + "game-server/player/kick", "POST", UNodecraftStudioSessionManager::Get().GetGameServerAccessToken(), JsonObj, OnComplete);
}

TSharedRef<IHttpRequest> UNodecraftStudioApi::FinishedReconnectionPhase(const UObject* WeakContext,
	FHttpRequestCompleteDelegate& OnComplete)
{
	FJsonObjectWrapper JsonObj;
	
	return MakeRequest(WeakContext, UNodecraftStudioApiSettings::Get().GetApiRoot() + "game-server/finished-reconnection-phase", "POST", UNodecraftStudioSessionManager::Get().GetGameServerAccessToken(), JsonObj, OnComplete);
}

TSharedRef<IHttpRequest> UNodecraftStudioApi::UpdatePlayerAllowStatus(const UObject* WeakContext, const FString& AllowId,
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
	return MakeRequest(WeakContext, UNodecraftStudioApiSettings::Get().GetApiRoot() + "allows/" + AllowId + "/" + StatusString,
		"PUT", UNodecraftStudioSessionManager::Get().GetPlayerAccessToken(), JsonObj, OnComplete);
}

TSharedRef<IHttpRequest> UNodecraftStudioApi::UpdateAllowStatus(const UObject* WeakContext, const FString& ServerId,
	const FString& AllowId, const EAllowStatus Status, FHttpRequestCompleteDelegate& OnComplete)
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
	return MakeRequest(WeakContext, UNodecraftStudioApiSettings::Get().GetApiRoot() + "allows/server/" + ServerId + "/" + AllowId + "/" + StatusString,
		"PUT", UNodecraftStudioSessionManager::Get().GetPlayerAccessToken(), JsonObj, OnComplete);
}

TSharedRef<IHttpRequest> UNodecraftStudioApi::ListPlayerAllows(const UObject* WeakContext,
                                                         FHttpRequestCompleteDelegate& OnComplete)
{
	FJsonObjectWrapper JsonObj;
	return MakeRequest(WeakContext, UNodecraftStudioApiSettings::Get().GetApiRoot() + "allows",
		"GET", UNodecraftStudioSessionManager::Get().GetPlayerAccessToken(), JsonObj, OnComplete);
}

TSharedRef<IHttpRequest> UNodecraftStudioApi::ListAllowsByServer(const UObject* WeakContext, const FString& ServerId,
	FHttpRequestCompleteDelegate& OnComplete)
{
	FJsonObjectWrapper JsonObj;
	return MakeRequest(WeakContext, UNodecraftStudioApiSettings::Get().GetApiRoot() + "allows/server/" + ServerId,
		"GET", UNodecraftStudioSessionManager::Get().GetPlayerAccessToken(), JsonObj, OnComplete);
}

TSharedRef<IHttpRequest> UNodecraftStudioApi::CreateAllow(const UObject* WeakContext, const FString& ServerId,
                                                    const FString& PlayerId, FHttpRequestCompleteDelegate& OnComplete)
{
	FJsonObjectWrapper JsonObj;
	JsonObj.JsonObject->SetStringField("player_id", PlayerId);
	return MakeRequest(WeakContext, UNodecraftStudioApiSettings::Get().GetApiRoot() + "allows/server/" + ServerId,
		"POST", UNodecraftStudioSessionManager::Get().GetPlayerAccessToken(), JsonObj, OnComplete);
}

TSharedRef<IHttpRequest> UNodecraftStudioApi::CreatePlayerConnection(const UObject* WeakContext, const FString& Subject, FHttpRequestCompleteDelegate& OnComplete)
{
	FJsonObjectWrapper JsonObj;
	JsonObj.JsonObject->SetStringField("subject", Subject);
	return MakeRequest(WeakContext, UNodecraftStudioApiSettings::Get().GetApiRoot() + "connection",
		"POST", UNodecraftStudioSessionManager::Get().GetPlayerAccessToken(), JsonObj, OnComplete);
}

TSharedRef<IHttpRequest> UNodecraftStudioApi::GetPlayerConnection(const UObject* WeakContext, const FString& ConnectionId,
	FHttpRequestCompleteDelegate& OnComplete)
{
	FJsonObjectWrapper JsonObj;
	return MakeRequest(WeakContext, UNodecraftStudioApiSettings::Get().GetApiRoot() + "connection/" + ConnectionId,
		"GET", UNodecraftStudioSessionManager::Get().GetPlayerAccessToken(), JsonObj, OnComplete);
}

TSharedRef<IHttpRequest> UNodecraftStudioApi::ConsumePlayerConnectionChallengeResponse(const UObject* WeakContext, const FString& ConnectionId,
	const FString& Code, FHttpRequestCompleteDelegate& OnComplete)
{
	FJsonObjectWrapper JsonObj;
	JsonObj.JsonObject->SetStringField("code", Code);
	return MakeRequest(WeakContext, UNodecraftStudioApiSettings::Get().GetApiRoot() + "connection/" + ConnectionId + "/challenge",
		"POST", UNodecraftStudioSessionManager::Get().GetPlayerAccessToken(), JsonObj, OnComplete);
}

TSharedRef<IHttpRequest> UNodecraftStudioApi::GetServerDetails(const UObject* WeakContext, const FString& ServerId,
                                                         FHttpRequestCompleteDelegate& OnComplete)
{
	FJsonObjectWrapper JsonObj;
	return MakeRequest(WeakContext, UNodecraftStudioApiSettings::Get().GetApiRoot() + "server/" + ServerId, "GET", UNodecraftStudioSessionManager::Get().GetPlayerAccessToken(), JsonObj, OnComplete);
}

TSharedRef<IHttpRequest> UNodecraftStudioApi::FavoriteServer(const UObject* WeakContext, const FString& ServerId,
	FHttpRequestCompleteDelegate& OnComplete)
{
	FJsonObjectWrapper JsonObj;
	return MakeRequest(WeakContext, UNodecraftStudioApiSettings::Get().GetApiRoot() + "server/" + ServerId + "/favorite",
		"POST", UNodecraftStudioSessionManager::Get().GetPlayerAccessToken(), JsonObj, OnComplete);
}

TSharedRef<IHttpRequest> UNodecraftStudioApi::UnfavoriteServer(const UObject* WeakContext, const FString& ServerId,
	FHttpRequestCompleteDelegate& OnComplete)
{
	FJsonObjectWrapper JsonObj;
	return MakeRequest(WeakContext, UNodecraftStudioApiSettings::Get().GetApiRoot() + "server/" + ServerId + "/favorite",
		"DELETE", UNodecraftStudioSessionManager::Get().GetPlayerAccessToken(), JsonObj, OnComplete);
}

TSharedRef<IHttpRequest> UNodecraftStudioApi::ListPublicServerModeration(const UObject* WeakContext, const FString& ServerId,
	FHttpRequestCompleteDelegate& OnComplete)
{
	FJsonObjectWrapper JsonObj;
	return MakeRequest(WeakContext, UNodecraftStudioApiSettings::Get().GetApiRoot() + "server/" + ServerId + "/moderation",
		"GET", UNodecraftStudioSessionManager::Get().GetPlayerAccessToken(), JsonObj, OnComplete);
}

TSharedRef<IHttpRequest> UNodecraftStudioApi::GetPlayerReputation(const UObject* WeakContext,
	FHttpRequestCompleteDelegate& OnComplete)
{
	FJsonObjectWrapper JsonObj;
	return MakeRequest(WeakContext, UNodecraftStudioApiSettings::Get().GetApiRoot() + "reputation/bans",
		"GET", UNodecraftStudioSessionManager::Get().GetPlayerAccessToken(), JsonObj, OnComplete);
}

TSharedRef<IHttpRequest> UNodecraftStudioApi::GetModerationActionReasons(const UObject* WeakContext,
	FHttpRequestCompleteDelegate& OnComplete)
{
	FJsonObjectWrapper JsonObj;
	return MakeRequest(WeakContext, UNodecraftStudioApiSettings::Get().GetApiRoot() + "reputation/reasons",
		"GET", UNodecraftStudioSessionManager::Get().GetPlayerAccessToken(), JsonObj, OnComplete);
}

TSharedRef<IHttpRequest> UNodecraftStudioApi::KickPlayers(const UObject* WeakContext, TArray<FString> PlayerIds,
	const FString& ServerId, const FString& ReasonId, TOptional<FString> ReasonNotes,
	FHttpRequestCompleteDelegate& OnComplete)
{
	FJsonObjectWrapper JsonObj;

	TArray<TSharedPtr<FJsonValue>> PlayerIdsJson;
	for (const FString& PlayerId : PlayerIds)
	{
		PlayerIdsJson.Add(MakeShared<FJsonValueString>(PlayerId));
	}
	JsonObj.JsonObject->SetArrayField("player_ids", PlayerIdsJson);
	// Set server ID, moderation reason ID, and reason notes
	JsonObj.JsonObject->SetStringField("server_id", ServerId);
	JsonObj.JsonObject->SetStringField("moderation_reason_id", ReasonId);
	if (ReasonNotes.IsSet())
	{
		JsonObj.JsonObject->SetStringField("reason", ReasonNotes.GetValue());
	}

	return MakeRequest(WeakContext, UNodecraftStudioApiSettings::Get().GetApiRoot() + "reputation/kicks", "POST", UNodecraftStudioSessionManager::Get().GetPlayerAccessToken(), JsonObj, OnComplete);
}

TSharedRef<IHttpRequest> UNodecraftStudioApi::BanPlayers(const UObject* WeakContext, TArray<FString> PlayerIds,
	const FString& ServerId, TOptional<FDateTime> DateExpires, const FString& ReasonId, const FString& ReasonNotes,
	FHttpRequestCompleteDelegate& OnComplete)
{
	FJsonObjectWrapper JsonObj;

	TArray<TSharedPtr<FJsonValue>> PlayerIdsJson;
	for (const FString& PlayerId : PlayerIds)
	{
		PlayerIdsJson.Add(MakeShared<FJsonValueString>(PlayerId));
	}
	JsonObj.JsonObject->SetArrayField("player_ids", PlayerIdsJson);
	// Set server ID, moderation reason ID, and reason notes
	JsonObj.JsonObject->SetStringField("server_id", ServerId);
	JsonObj.JsonObject->SetStringField("moderation_reason_id", ReasonId);
	JsonObj.JsonObject->SetStringField("reason", ReasonNotes);
	if (DateExpires.IsSet())
	{
		JsonObj.JsonObject->SetStringField("date_expires", DateExpires.GetValue().ToIso8601());
	}

	return MakeRequest(WeakContext, UNodecraftStudioApiSettings::Get().GetApiRoot() + "reputation/bans", "POST", UNodecraftStudioSessionManager::Get().GetPlayerAccessToken(), JsonObj, OnComplete);
}

TSharedRef<IHttpRequest> UNodecraftStudioApi::UnbanPlayers(const UObject* WeakContext, TArray<FString> BanIds, FHttpRequestCompleteDelegate& OnComplete)
{
	FJsonObjectWrapper JsonObj;

	TArray<TSharedPtr<FJsonValue>> BanIdsJson;
	for (const FString& BanId : BanIds)
	{
		BanIdsJson.Add(MakeShared<FJsonValueString>(BanId));
	}
	JsonObj.JsonObject->SetArrayField("ban_ids", BanIdsJson);

	return MakeRequest(WeakContext, UNodecraftStudioApiSettings::Get().GetApiRoot() + "reputation/bans/revoke", "POST", UNodecraftStudioSessionManager::Get().GetPlayerAccessToken(), JsonObj, OnComplete);
}


TSharedRef<IHttpRequest> UNodecraftStudioApi::UpdatePlayerModStatus(const UObject* WeakContext, TArray<FString> PlayerIds, const FString& ServerId, bool bIsMod,
	FHttpRequestCompleteDelegate& OnComplete)
{
	if (bIsMod)
	{
		return PromoteModerators(WeakContext, PlayerIds, ServerId, OnComplete);
	}
	else
	{
		return DemoteModerators(WeakContext, PlayerIds, ServerId, OnComplete);
	}
}

TSharedRef<IHttpRequest> UNodecraftStudioApi::PromoteModerators(const UObject* WeakContext, TArray<FString> PlayerIds,
	const FString& ServerId, FHttpRequestCompleteDelegate& OnComplete)
{
	FJsonObjectWrapper JsonObj;

	TArray<TSharedPtr<FJsonValue>> PlayerIdsJson;
	for (const FString& PlayerId : PlayerIds)
	{
		PlayerIdsJson.Add(MakeShared<FJsonValueString>(PlayerId));
	}
	JsonObj.JsonObject->SetArrayField("moderator_ids", PlayerIdsJson);

	FString URI = FString::Format(TEXT("{0}server/{1}/role/moderator"), { UNodecraftStudioApiSettings::Get().GetApiRoot(), ServerId });
	return MakeRequest(WeakContext, URI, "POST", UNodecraftStudioSessionManager::Get().GetPlayerAccessToken(), JsonObj, OnComplete);
}

TSharedRef<IHttpRequest> UNodecraftStudioApi::DemoteModerators(const UObject* WeakContext, TArray<FString> PlayerIds,
	const FString& ServerId, FHttpRequestCompleteDelegate& OnComplete)
{
	FJsonObjectWrapper JsonObj;

	TArray<TSharedPtr<FJsonValue>> PlayerIdsJson;
	for (const FString& PlayerId : PlayerIds)
	{
		PlayerIdsJson.Add(MakeShared<FJsonValueString>(PlayerId));
	}
	JsonObj.JsonObject->SetArrayField("moderator_ids", PlayerIdsJson);

	FString URI = FString::Format(TEXT("{0}server/{1}/role/moderator/revoke"), { UNodecraftStudioApiSettings::Get().GetApiRoot(), ServerId });
	return MakeRequest(WeakContext, URI, "POST", UNodecraftStudioSessionManager::Get().GetPlayerAccessToken(), JsonObj, OnComplete);
}

TSharedRef<IHttpRequest> UNodecraftStudioApi::SignConsents(const UObject* WeakContext, const FGameConsents& Consents,
                                                     FHttpRequestCompleteDelegate& OnComplete)
{
	FJsonObjectWrapper JsonObj;

	FString JsonStr;
	TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> JsonWriter = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonStr, 0);
	FJsonSerializer::Serialize(MakeShared<FJsonValueArray>(Consents.ToJsonArray()), FString(), JsonWriter);
	
	return MakeRequest(WeakContext, UNodecraftStudioApiSettings::Get().GetApiRoot() + "consents/player/" + Consents.GetConsentType(), "POST", UNodecraftStudioSessionManager::Get().GetPlayerAccessToken(), JsonStr, OnComplete);
}

TSharedRef<IHttpRequest> UNodecraftStudioApi::GetPlayerLegalConsents(const UObject* WeakContext,
	FHttpRequestCompleteDelegate& OnComplete)
{
	FJsonObjectWrapper JsonObj;
	return MakeRequest(WeakContext, UNodecraftStudioApiSettings::Get().GetApiRoot() + "consents/player/legal",
		"GET", UNodecraftStudioSessionManager::Get().GetPlayerAccessToken(), JsonObj, OnComplete);
}

TSharedRef<IHttpRequest> UNodecraftStudioApi::GetPlayerRulesConsents(const UObject* WeakContext,
	FHttpRequestCompleteDelegate& OnComplete)
{
	FJsonObjectWrapper JsonObj;
	return MakeRequest(WeakContext, UNodecraftStudioApiSettings::Get().GetApiRoot() + "consents/player/rules",
		"GET", UNodecraftStudioSessionManager::Get().GetPlayerAccessToken(), JsonObj, OnComplete);
}

TSharedRef<IHttpRequest> UNodecraftStudioApi::MakeRequest(const UObject* WeakContext, FString RequestURI, FString HttpMethod,
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

TSharedRef<IHttpRequest> UNodecraftStudioApi::MakeRequest(const UObject* WeakContext, FString RequestURI, FString HttpMethod,
                                                    FString HeaderToken, const FJsonObjectWrapper& RequestBody, FHttpRequestCompleteDelegate& OnCompleteCallback)
{
	FString JsonStr;
	RequestBody.JsonObjectToString(JsonStr);
	return MakeRequest(WeakContext, RequestURI, HttpMethod, HeaderToken, JsonStr, OnCompleteCallback);
}
