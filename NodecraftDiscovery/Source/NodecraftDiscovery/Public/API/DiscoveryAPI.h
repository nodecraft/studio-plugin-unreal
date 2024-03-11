﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DiscoverySessionManagerSubsystem.h"
#include "HttpModule.h"
#include "Models/PlayerSession.h"
#include "JsonObjectConverter.h"
#include "Interfaces/IHttpResponse.h"
#include "Models/Consents.h"
#include "UObject/Object.h"
#include "DeveloperSettings/DiscoveryAPISettings.h"
#include "Models/AllowStatus.h"
#include "Models/InviteStatus.h"
#include "DiscoveryAPI.generated.h"

#define DISCOVERY_API_HEADER_NODECRAFT_TOKEN "x-nodecraft-token"

#define DISCOVERY_API_ENDPOINT_GAME FString("game")

UENUM()
enum class EHttpMethod : uint8
{
	Get,
	Post,
	Put,
	Delete,
	COUNT UMETA(Hidden)
};
ENUM_RANGE_BY_COUNT(EHttpMethod, EHttpMethod::COUNT);

UENUM()
enum class EAccessTokenType : uint8
{
	GameAccessToken_Public,
	PlayerAccessToken,
	COUNT UMETA(Hidden)
};
ENUM_RANGE_BY_COUNT(EAccessTokenType, EAccessTokenType::COUNT);

/**
 * 
 */
UCLASS()
class NODECRAFTDISCOVERY_API UDiscoveryAPI : public UObject
{
	GENERATED_BODY()

public:
	// Ident/Manual-Challenge
	template<typename Functor>
	static TSharedRef<IHttpRequest> Ident_ManualChallenge(const UObject* WeakContext, FString Email, Functor Callback)
	{

		UDiscoverySessionManager& SessionManager = UDiscoverySessionManager::Get();
		const FString JSONText = "{   \"type\": \"email\",   \"data\": {     \"email\": \"" + Email + "\"   } }";
		FJsonObjectWrapper JsonObj;
		JsonObj.JsonObjectFromString(JSONText);
		return MakeRequest(WeakContext, UDiscoveryAPISettings::Get().GetApiRoot() + "ident/manual-challenge", "POST", SessionManager.GetGameAccessToken(), JsonObj, Callback);
	}

	// Ident/token
	static TSharedRef<IHttpRequest> SendIdentToken(const UObject* WeakContext, const FJsonObjectWrapper& RequestBody, FHttpRequestCompleteDelegate& OnComplete);

	// TODO: Deprecate and remove this function. Replace with SendIdentToken
	template<typename Functor>
	static TSharedRef<IHttpRequest> Ident_Token(const UObject* WeakContext, FString Email, FString Token, TOptional<FGameConsents>& GameConsents, Functor Callback)
	{

		UDiscoverySessionManager& SessionManager = UDiscoverySessionManager::Get();
		const FString JSONText = "{   \"type\": \"email\",   \"data\": {     \"email\": \""
			+ Email
			+ "\",     \"code\": \""
			+ Token
			+ "\"   } }";
		FJsonObjectWrapper JsonObj;
		JsonObj.JsonObjectFromString(JSONText);

		if (GameConsents.IsSet())
		{
			FGameConsents Consents = GameConsents.GetValue();
			JsonObj.JsonObject->SetArrayField("consents", Consents.ToJsonArray());
		}
		
		
		return MakeRequest(WeakContext, UDiscoveryAPISettings::Get().GetApiRoot() + "ident/token", "POST", SessionManager.GetGameAccessToken(),
		                   JsonObj, [Callback](
		                   FHttpRequestPtr Req,
		                   FHttpResponsePtr Res,
		                   bool bConnectedSuccessfully) mutable
		                   {
			                   if (bConnectedSuccessfully)
			                   {
				                   FJsonObjectWrapper ResJson;
				                   ResJson.JsonObjectFromString(Res.Get()->GetContentAsString());
				                   if (const TSharedPtr<FJsonObject>& Data = ResJson.JsonObject->GetObjectField("data"); Data.IsValid())
				                   {
										UDiscoverySessionManager& SessionManager = UDiscoverySessionManager::Get();
				                   		SessionManager.SetPlayerSession(FPlayerSession::FromJson(Data));
				                   }
			                   }
			                   Callback(Req, Res, bConnectedSuccessfully);
		                   });
	}

	static TSharedRef<IHttpRequest> GetPlayerSession(const UObject* WeakContext, FHttpRequestCompleteDelegate& OnComplete);
	static TSharedRef<IHttpRequest> GetPlayerSettings(const UObject* WeakContext, FHttpRequestCompleteDelegate& OnComplete);
	static TSharedRef<IHttpRequest> UpdatePlayerSettings(const UObject* WeakContext, TOptional<bool> AnalyticsOptOut, TOptional<FString> DefaultRegionID, FHttpRequestCompleteDelegate& OnComplete);
	static TSharedRef<IHttpRequest> UpdateCurrentPlayerRegion(const UObject* WeakContext, const FString& RegionID, FHttpRequestCompleteDelegate& OnComplete);
	

	// game
	static TSharedRef<IHttpRequest> GameDetails(const UObject* WeakContext, FHttpRequestCompleteDelegate& OnComplete);

	static TSharedRef<IHttpRequest> GetGameLegalConsents(
		const UObject* WeakContext, const ::FString& IdentType, FHttpRequestCompleteDelegate& OnComplete);


	// Server Lists
	static TSharedRef<IHttpRequest> ListPopularServers(const UObject* WeakContext, FHttpRequestCompleteDelegate& OnComplete);
	static TSharedRef<IHttpRequest> ListRecommendedServers(const UObject* WeakContext, FHttpRequestCompleteDelegate& OnComplete);
	static TSharedRef<IHttpRequest> ListPlayerOwnedServers(const UObject* WeakContext, FHttpRequestCompleteDelegate& OnComplete);
	static TSharedRef<IHttpRequest> ListFavoriteServers(const UObject* WeakContext, FHttpRequestCompleteDelegate& OnComplete);
	static TSharedRef<IHttpRequest> ListInvitableServers(const UObject* WeakContext, const FString& InviteeID, FHttpRequestCompleteDelegate& OnComplete);

	// Server join queue
	static TSharedRef<IHttpRequest> GetServerQueue(const UObject* WeakContext, const FString& Token, FHttpRequestCompleteDelegate& OnComplete);
	static TSharedRef<IHttpRequest> RenewServerQueue(const UObject* WeakContext, const FString& Token, FHttpRequestCompleteDelegate& OnComplete);
	static TSharedRef<IHttpRequest> CancelServerQueue(const UObject* WeakContext, const FString& Token, FHttpRequestCompleteDelegate& OnComplete);
	static TSharedRef<IHttpRequest> JoinServer(const UObject* WeakContext, const FString& ServerID, TOptional<FString> ServerPassword, FHttpRequestCompleteDelegate& OnComplete);
	
	// Consents
	static TSharedRef<IHttpRequest> SignConsents(const UObject* WeakContext, const FGameConsents& Consents, FHttpRequestCompleteDelegate& OnComplete);
	static TSharedRef<IHttpRequest> GetPlayerLegalConsents(const UObject* WeakContext, FHttpRequestCompleteDelegate& OnComplete);
	static TSharedRef<IHttpRequest> GetPlayerRulesConsents(const UObject* WeakContext, FHttpRequestCompleteDelegate& OnComplete);
	
	// Server details
	static TSharedRef<IHttpRequest> GetServerDetails(const UObject* WeakContext, const FString& ServerId, FHttpRequestCompleteDelegate& OnComplete);
	static TSharedRef<IHttpRequest> FavoriteServer(const UObject* WeakContext, const FString& ServerId, FHttpRequestCompleteDelegate& OnComplete);
	static TSharedRef<IHttpRequest> UnfavoriteServer(const UObject* WeakContext, const FString& ServerId, FHttpRequestCompleteDelegate& OnComplete);

	// Server moderation
	static TSharedRef<IHttpRequest> ListPublicServerModeration(const UObject* WeakContext, const FString& ServerId, FHttpRequestCompleteDelegate& OnComplete);
	static TSharedRef<IHttpRequest> GetPlayerReputation(const UObject* WeakContext, FHttpRequestCompleteDelegate& OnComplete);

	// List Players
	static TSharedRef<IHttpRequest> GetRecentPlayers(const UObject* WeakContext, const FString& ServerId, FHttpRequestCompleteDelegate& OnComplete);
	static TSharedRef<IHttpRequest> GetOnlinePlayers(const UObject* WeakContext, const FString& ServerId, FHttpRequestCompleteDelegate& OnComplete);

	// Notifications
	static TSharedRef<IHttpRequest> GetNotifications(const UObject* WeakContext, FHttpRequestCompleteDelegate& OnComplete);
	static TSharedRef<IHttpRequest> GetNotificationsHistory(const UObject* WeakContext, FHttpRequestCompleteDelegate& OnComplete);
	static TSharedRef<IHttpRequest> DismissNotification(const UObject* WeakContext, const FString& NotificationId, FHttpRequestCompleteDelegate& OnComplete);
	static TSharedRef<IHttpRequest> DismissNotifications(const UObject* WeakContext, const TArray<FString> NotificationIds, FHttpRequestCompleteDelegate& OnComplete);
	
	// Friends
	static TSharedRef<IHttpRequest> ListFriends(const UObject* WeakContext, FHttpRequestCompleteDelegate& OnComplete);

	// Invites
	static TSharedRef<IHttpRequest> UpdateInviteStatus(const UObject* WeakContext, const FString& InviteId, const ENodecraftInviteStatus Status, FHttpRequestCompleteDelegate&
	                                                   OnComplete);

	// Server Moderation
	static TSharedRef<IHttpRequest> GetOwner(const UObject* WeakContext, const FString& ServerId, FHttpRequestCompleteDelegate& OnComplete);
	static TSharedRef<IHttpRequest> GetModerators(const UObject* WeakContext, const FString& ServerId, FHttpRequestCompleteDelegate& OnComplete);
	static TSharedRef<IHttpRequest> GetOnlinePlayersForModeration(const UObject* WeakContext, const FString& ServerId, FHttpRequestCompleteDelegate& OnComplete);
	static TSharedRef<IHttpRequest> GetOfflinePlayersForModeration(const UObject* WeakContext, const FString& ServerId, FHttpRequestCompleteDelegate& OnComplete);
	static TSharedRef<IHttpRequest> GetBannedPlayers(const UObject* WeakContext, const FString& ServerId, FHttpRequestCompleteDelegate& OnComplete);
	
	// Allows
	static TSharedRef<IHttpRequest> UpdateAllowStatus(const UObject* WeakContext, const FString& AllowId, const EAllowStatus Status, FHttpRequestCompleteDelegate& OnComplete);
	static TSharedRef<IHttpRequest> ListPlayerAllows(const UObject* WeakContext, FHttpRequestCompleteDelegate& OnComplete);
	
	template<typename Functor>
	static TSharedRef<IHttpRequest> MakeRequest(const UObject* WeakContext, FString RequestURI, FString HttpMethod,
	                                            FString HeaderToken, const FJsonObjectWrapper& RequestBody,
	                                            Functor OnCompleteCallback)
	{
        FHttpModule& HTTPModule = FHttpModule::Get();

        TSharedRef<IHttpRequest> Request = HTTPModule.CreateRequest();

        Request->SetVerb(HttpMethod);

        // We'll need to tell the server what type of content to expect in the POST data
        Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
        Request->SetHeader(TEXT(DISCOVERY_API_HEADER_NODECRAFT_TOKEN), HeaderToken);

		// SET BODY
		if (HttpMethod.Equals("Get") == false)
		{
			FString JsonText;
			RequestBody.JsonObjectToString(JsonText);
			Request->SetContentAsString(JsonText);
		}
		
        Request->SetURL(RequestURI);

		Request->OnProcessRequestComplete().BindWeakLambda(WeakContext, OnCompleteCallback);

        return Request;
    }

	static TSharedRef<IHttpRequest> MakeRequest(const UObject* WeakContext, FString RequestURI, FString HttpMethod,
	                                            FString HeaderToken, const FString& JsonText, FHttpRequestCompleteDelegate& OnCompleteCallback);

	static TSharedRef<IHttpRequest> MakeRequest(const UObject* WeakContext, FString RequestURI,
	                                            FString HttpMethod, FString HeaderToken,
	                                            const FJsonObjectWrapper& RequestBody, FHttpRequestCompleteDelegate& OnCompleteCallback);
};


