// Fill out your copyright notice in the Description page of Project Settings.


#include "Services/ServerQueueService.h"

#include "JsonObjectWrapper.h"
#include "API/DiscoveryAPI.h"
#include "Interfaces/IHttpResponse.h"
#include "Models/ServerSessionDataObject.h"
#include "Models/ServerQueueDataObject.h"
#include "Models/ServerQueueTokenDataObject.h"
#include "Utility/NodecraftUtility.h"

class UServerPasswordModal;

void UServerQueueService::CreateJoinServerDelegate(const FJoinServerDelegate& OnComplete,
                                                   FHttpRequestCompleteDelegate& ReqCallbackOut)
{
	ReqCallbackOut.BindWeakLambda(this, [OnComplete](FHttpRequestPtr Req, FHttpResponsePtr Res, bool bConnectedSuccessfully)
	{
		if (bConnectedSuccessfully)
		{
			if (EHttpResponseCodes::IsOk(Res.Get()->GetResponseCode()))
			{
				FJsonObjectWrapper ResJson;
				ResJson.JsonObjectFromString(Res.Get()->GetContentAsString());
				if (const TSharedPtr<FJsonObject> Data = ResJson.JsonObject->GetObjectField("data"); Data.IsValid())
				{
					UServerQueueTokenDataObject* ServerQueueToken = UServerQueueTokenDataObject::FromJson(Data.ToSharedRef());
					OnComplete.ExecuteIfBound(ServerQueueToken, true, TOptional<FText>());
				}
				else
				{
					OnComplete.ExecuteIfBound(nullptr, false, FText::FromString("UServersService::CreateJoinServerDelegate: Error while grabbing data from response"));
				}
			}
			else
			{
				const FText ErrorMsg = UNodecraftUtility::ParseError(Res, __FUNCTION__);
				OnComplete.ExecuteIfBound(nullptr, false, ErrorMsg);
			}
		}
		else
		{
			OnComplete.ExecuteIfBound(nullptr, false, FText::FromString("UServersService::CreateJoinServerDelegate: Could not connect to the server."));
		}
	});
}

void UServerQueueService::CreateGetServerQueueDelegate(const FGetServerQueueDelegate& OnComplete,
	FHttpRequestCompleteDelegate& ReqCallbackOut)
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
					UServerQueueDataObject* ServerQueueDataObject = UServerQueueDataObject::FromJson(Data.ToSharedRef());
					OnComplete.ExecuteIfBound(ServerQueueDataObject, true, TOptional<FText>());
				}
				else
				{
					const FText ErrorMsg = FText::FromString("UServersService::CreateGetServerQueueDelegate: error while grabbing data from response");
					OnComplete.ExecuteIfBound(nullptr, false, ErrorMsg);
				}
			}
			else
			{
				const FText ErrorMsg = UNodecraftUtility::ParseError(Res, __FUNCTION__);
				OnComplete.ExecuteIfBound(nullptr, false, ErrorMsg);
			}
		}
		else
		{
			OnComplete.ExecuteIfBound(nullptr, false, FText::FromString("UServersService::CreateGetServerQueueDelegate: could not connect to server."));
		}
	});
}

void UServerQueueService::CreateRenewServerQueueDelegate(const FJoinServerDelegate& OnComplete,
	FHttpRequestCompleteDelegate& ReqCallbackOut)
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
					UServerQueueTokenDataObject* TokenDataObject = UServerQueueTokenDataObject::FromJson(Data.ToSharedRef());
					OnComplete.ExecuteIfBound(TokenDataObject, true, TOptional<FText>());
				}
				else
				{
					const FText ErrorMsg = FText::FromString("UServersService::CreateRenewServerQueueDelegate: error while grabbing data from response");
					OnComplete.ExecuteIfBound(nullptr, false, ErrorMsg);
				}
			}
			else
			{
				const FText ErrorMsg = UNodecraftUtility::ParseError(Res, __FUNCTION__);
				OnComplete.ExecuteIfBound(nullptr, false, ErrorMsg);
			}
		}
		else
		{
			OnComplete.ExecuteIfBound(nullptr, false, FText::FromString("UServersService::CreateRenewServerQueueDelegate: could not connect to server."));
		}
	});
}

void UServerQueueService::CreateCancelServerQueueDelegate(const FSimpleServiceResponseDelegate& OnComplete,
	FHttpRequestCompleteDelegate& ReqCallbackOut)
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
					OnComplete.ExecuteIfBound(true, TOptional<FText>());
				}
				else
				{
					const FText ErrorMsg = FText::FromString("UServersService::CreateCancelServerQueueDelegate: error while grabbing data from response");
					OnComplete.ExecuteIfBound(false, ErrorMsg);
				}
			}
			else
			{
				const FText ErrorMsg = UNodecraftUtility::ParseError(Res, __FUNCTION__);
				OnComplete.ExecuteIfBound(false, ErrorMsg);
			}
		}
		else
		{
			OnComplete.ExecuteIfBound(false, FText::FromString("UServersService::CreateCancelServerQueueDelegate: could not connect to server."));
		}
	});
}

bool UServerQueueService::JoinServer(const FString& ServerId, const TOptional<FString>& ServerPassword, FJoinServerDelegate& OnComplete)
{
	FHttpRequestCompleteDelegate ReqCallback;
	CreateJoinServerDelegate(OnComplete, ReqCallback);
	return UDiscoveryAPI::JoinServer(this, ServerId, ServerPassword, ReqCallback)->ProcessRequest();
}

bool UServerQueueService::GetServerQueue(const FString& Token, FGetServerQueueDelegate& OnComplete)
{
	FHttpRequestCompleteDelegate ReqCallback;
	CreateGetServerQueueDelegate(OnComplete, ReqCallback);
	return UDiscoveryAPI::GetServerQueue(this, Token, ReqCallback)->ProcessRequest();
}

bool UServerQueueService::RenewServerQueue(const FString& Token, FJoinServerDelegate& OnComplete)
{
	FHttpRequestCompleteDelegate ReqCallback;
	CreateRenewServerQueueDelegate(OnComplete, ReqCallback);
	return UDiscoveryAPI::RenewServerQueue(this, Token, ReqCallback)->ProcessRequest();
}

bool UServerQueueService::CancelServerQueue(const FString& Token, FSimpleServiceResponseDelegate& OnComplete)
{
	StopPollingServerQueue();
	
	FHttpRequestCompleteDelegate ReqCallback;
	CreateCancelServerQueueDelegate(OnComplete, ReqCallback);
	return UDiscoveryAPI::CancelServerQueue(this, Token, ReqCallback)->ProcessRequest();
}

void UServerQueueService::PollServerQueue(const UServerQueueTokenDataObject* ServerQueueTokenDataObject)
{
	if (IsValid(ServerQueueTokenDataObject))
	{
		FGetServerQueueDelegate OnGetServerQueueComplete;
		OnGetServerQueueComplete.BindWeakLambda(this, [this, ServerQueueTokenDataObject](UServerQueueDataObject* ServerQueueDataObject, bool bSuccess, TOptional<FText> Error)
		{
			if (bSuccess)
			{
				if (ServerQueueDataObject)
				{
					if (ServerQueueDataObject->GetServerSession())
					{
						OnGetServerSession.Broadcast(ServerQueueDataObject->GetServerSession());
						StopPollingServerQueue();
						return;
					}

					// dispatch the new server queue so the UI can update the wait time
					OnGetServerQueue.ExecuteIfBound(ServerQueueDataObject, true, TOptional<FText>());
				}
				
				// renew queue if the queue token expires in <60 seconds
				if ((ServerQueueTokenDataObject->GetQueueExpires() - FDateTime::UtcNow()).GetTotalSeconds() < 60)
				{
					FJoinServerDelegate OnServerQueueRenewed;
					OnServerQueueRenewed.BindWeakLambda(this, [this](UServerQueueTokenDataObject* TokenDataObject, bool bSuccess, TOptional<FText> Error)
					{
						if (bSuccess)
						{
							OnRenewServerQueue.ExecuteIfBound(TokenDataObject);
						}
					});
					RenewServerQueue(ServerQueueTokenDataObject->GetToken(), OnServerQueueRenewed);
				}
			}
		});
		GetServerQueue(ServerQueueTokenDataObject->GetToken(), OnGetServerQueueComplete);
	}
}

void UServerQueueService::StartPollingServerQueue(const UServerQueueTokenDataObject* TokenDataObject)
{
	StopPollingServerQueue();
	
	ServerQueuePollTimerDelegate.BindUObject(this, &UServerQueueService::PollServerQueue, TokenDataObject);
	const float PollingInterval = UDiscoveryAPISettings::Get().GetServerJoinPollingInterval();
	GetWorld()->GetTimerManager().SetTimer(ServerQueuePollTimerHandle, ServerQueuePollTimerDelegate, PollingInterval, true);
}

void UServerQueueService::StopPollingServerQueue()
{
	ServerQueuePollTimerDelegate.Unbind();
	if (ServerQueuePollTimerHandle.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(ServerQueuePollTimerHandle);
	}
}
