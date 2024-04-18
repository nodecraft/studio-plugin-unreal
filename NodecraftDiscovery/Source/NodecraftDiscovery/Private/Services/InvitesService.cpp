// Nodecraft, Inc. © 2012-2024, All Rights Reserved.


#include "Services/InvitesService.h"

#include "Api/NodecraftStudioApi.h"
#include "Interfaces/IHttpRequest.h"
#include "Models/InviteDataObject.h"
#include "Models/ServerDataObject.h"
#include "Subsystems/MessageRouterSubsystem.h"
#include "Utility/NodecraftUtility.h"

bool UInvitesService::AcceptInvite(const FString& InviteId, FSimpleServiceResponseDelegate OnComplete)
{
	FHttpRequestCompleteDelegate ReqCallback;
	ReqCallback.BindWeakLambda(this, [this, OnComplete](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully) mutable
	{
		if (bConnectedSuccessfully)
		{
			UMessageRouterSubsystem::Get().RouteHTTPResult(Response, __FUNCTION__);
		}
		else
		{
			UMessageRouterSubsystem::Get().RouteFailureToConnect(__FUNCTION__);
		}
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

	return UNodecraftStudioApi::UpdateInviteStatus(this, InviteId, ENodecraftInviteStatus::Accepted, ReqCallback)->ProcessRequest();
}

bool UInvitesService::DeclineInvite(const FString& InviteId, FSimpleServiceResponseDelegate OnComplete)
{
	FHttpRequestCompleteDelegate ReqCallback;
	ReqCallback.BindWeakLambda(this, [this, OnComplete](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully) mutable
	{
		if (bConnectedSuccessfully)
		{
			UMessageRouterSubsystem::Get().RouteHTTPResult(Response, __FUNCTION__);
		}
		else
		{
			UMessageRouterSubsystem::Get().RouteFailureToConnect(__FUNCTION__);
		}
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

	return UNodecraftStudioApi::UpdateInviteStatus(this, InviteId, ENodecraftInviteStatus::Declined, ReqCallback)->ProcessRequest();
}

bool UInvitesService::ListInvitableServers(const FString& FriendID, FOnListInvitableServers OnComplete)
{
	FHttpRequestCompleteDelegate ReqCallback;
	ReqCallback.BindWeakLambda(this, [this, OnComplete](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
	{
		if (bConnectedSuccessfully)
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
				const FText ErrorMsg = UNodecraftUtility::ParseMessage(Response, __FUNCTION__);
				OnComplete.ExecuteIfBound({}, false, ErrorMsg);
			}
			UMessageRouterSubsystem::Get().RouteHTTPResult(Response, __FUNCTION__);
		}
		else
		{
			UMessageRouterSubsystem::Get().RouteFailureToConnect(__FUNCTION__);
			OnComplete.ExecuteIfBound({}, false, FText::FromString("UInvitesService::ListInvitableServers: Failed to connect to server."));
		}
	});

	return UNodecraftStudioApi::ListInvitableServers(this, FriendID, ReqCallback)->ProcessRequest();
}

bool UInvitesService::CreateInvite(const FString& ServerID, const FString& FriendID, FOnCreateInvite OnComplete)
{
	FHttpRequestCompleteDelegate ReqCallback;
	ReqCallback.BindWeakLambda(this, [this, OnComplete](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
	{
		if (bConnectedSuccessfully)
		{
			if (EHttpResponseCodes::IsOk(Response.Get()->GetResponseCode()))
			{
				FJsonObjectWrapper ResJson;
				ResJson.JsonObjectFromString(Response.Get()->GetContentAsString());
				if (const TSharedPtr<FJsonObject> Data = ResJson.JsonObject->GetObjectField("data"); Data.IsValid())
				{
					const UInviteDataObject* InviteDataObject = UInviteDataObject::FromJson(Data.ToSharedRef());
					OnComplete.ExecuteIfBound(InviteDataObject, true, TOptional<FText>());
				}
				else
				{
					OnComplete.ExecuteIfBound(nullptr, false, FText::FromString("UInvitesService::CreateInvite: No invite data."));
				}
			}
			else
			{
				const FText ErrorMsg = UNodecraftUtility::ParseMessage(Response, __FUNCTION__);
				OnComplete.ExecuteIfBound(nullptr, false, ErrorMsg);
			}
			UMessageRouterSubsystem::Get().RouteHTTPResult(Response, __FUNCTION__);
		}
		else
		{
			UMessageRouterSubsystem::Get().RouteFailureToConnect(__FUNCTION__);
			OnComplete.ExecuteIfBound(nullptr, false, FText::FromString("UInvitesService::CreateInvite: Failed to connect to server."));
		}
	});
	
	return UNodecraftStudioApi::CreateInvite(this, ServerID, FriendID, ReqCallback)->ProcessRequest();
}
