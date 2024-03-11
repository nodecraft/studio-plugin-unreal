// Fill out your copyright notice in the Description page of Project Settings.


#include "Services/InvitesService.h"

#include "API/DiscoveryAPI.h"
#include "Interfaces/IHttpRequest.h"
#include "Models/ServerDataObject.h"
#include "Utility/NodecraftUtility.h"

bool UInvitesService::AcceptInvite(const FString& InviteId, FSimpleServiceResponseDelegate OnComplete)
{
	FHttpRequestCompleteDelegate ReqCallback;
	ReqCallback.BindWeakLambda(this, [this, OnComplete](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully) mutable
	{
		if (bConnectedSuccessfully && EHttpResponseCodes::IsOk(Response.Get()->GetResponseCode()))
		{
			// Dismiss the notification
			OnComplete.ExecuteIfBound(true, FText());
		}
		else
		{
			OnComplete.ExecuteIfBound(false, FText::FromString(Response->GetContentAsString()));
		}
	});

	return UDiscoveryAPI::UpdateInviteStatus(this, InviteId, ENodecraftInviteStatus::Accepted, ReqCallback)->ProcessRequest();
}

bool UInvitesService::DeclineInvite(const FString& InviteId, FSimpleServiceResponseDelegate OnComplete)
{
	FHttpRequestCompleteDelegate ReqCallback;
	ReqCallback.BindWeakLambda(this, [this, OnComplete](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful) mutable
	{
		if (bWasSuccessful)
		{
			// Dismiss the notification
			OnComplete.ExecuteIfBound(true, FText());
		}
		else
		{
			OnComplete.ExecuteIfBound(false, FText::FromString(Response->GetContentAsString()));
		}
	});

	return UDiscoveryAPI::UpdateInviteStatus(this, InviteId, ENodecraftInviteStatus::Declined, ReqCallback)->ProcessRequest();
}

bool UInvitesService::ListInvitableServers(const FString& FriendID, FOnListInvitableServers OnComplete)
{
	FHttpRequestCompleteDelegate ReqCallback;
	ReqCallback.BindWeakLambda(this, [this, OnComplete](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
	{
		if (bWasSuccessful)
		{
			if (EHttpResponseCodes::IsOk(Response.Get()->GetResponseCode()))
			{
				FJsonObjectWrapper ResJson;
				ResJson.JsonObjectFromString(Response.Get()->GetContentAsString());
				if (const TArray<TSharedPtr<FJsonValue>>& Data = ResJson.JsonObject->GetArrayField("data"); Data.Num() > 0)
				{
					TArray<UServerDataObject*> Servers;
					for (TSharedPtr<FJsonValue, ESPMode::ThreadSafe> JsonValue : Data)
					{
						if (UServerDataObject* Server = UServerDataObject::FromJson(JsonValue.Get()->AsObject().ToSharedRef()))
						{
							Servers.Add(Server);
						}
					}
					OnComplete.ExecuteIfBound(Servers, true, TOptional<FText>());
				}
				else
				{
					OnComplete.ExecuteIfBound({}, true, FText::FromString("UInvitesService::ListInvitableServers: No servers to list."));
				}
			}
			else
			{
				const FText ErrorMsg = UNodecraftUtility::ParseError(Response, __FUNCTION__);
				OnComplete.ExecuteIfBound({}, false, ErrorMsg);
			}
		}
		else
		{
			OnComplete.ExecuteIfBound({}, false, FText::FromString("UInvitesService::ListInvitableServers: Failed to connect to server."));
		}
	});

	return UDiscoveryAPI::ListInvitableServers(this, FriendID, ReqCallback)->ProcessRequest();
}
