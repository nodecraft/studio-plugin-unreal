// Nodecraft, Inc. © 2012-2024, All Rights Reserved.


#include "Services/ServerQueueService.h"

#include "JsonObjectWrapper.h"
#include "API/NodecraftStudioApi.h"
#include "Interfaces/IHttpResponse.h"
#include "Models/ServerDataObject.h"
#include "Models/ServerSessionDataObject.h"
#include "Models/ServerQueueDataObject.h"
#include "Models/ServerQueueTokenDataObject.h"
#include "Subsystems/MessageRouterSubsystem.h"
#include "Utility/NodecraftUtility.h"

class UServerPasswordModal;

void UServerQueueService::CreateJoinServerDelegate(FHttpRequestCompleteDelegate& ReqCallbackOut, UServerDataObject* Server)
{
	ReqCallbackOut.BindWeakLambda(this, [this, Server](FHttpRequestPtr Req, FHttpResponsePtr Res, bool bConnectedSuccessfully)
	{
		if (bConnectedSuccessfully)
		{
			int32 ResponseCode = Res.Get()->GetResponseCode();
			if (EHttpResponseCodes::IsOk(ResponseCode))
			{
				FJsonObjectWrapper ResJson;
				ResJson.JsonObjectFromString(Res.Get()->GetContentAsString());
				if (const TSharedPtr<FJsonObject> Data = ResJson.JsonObject->GetObjectField(TEXT("data")); Data.IsValid())
				{
					const UServerQueueTokenDataObject* ServerQueueToken = UServerQueueTokenDataObject::FromJson(Data.ToSharedRef());
					// Once we have the server queue token, we can start polling the server queue
					ServerQueueTokenDataObject = ServerQueueToken;
					StartPollingServerQueue();
					
					// OnComplete.ExecuteIfBound(ServerQueueToken, true, TOptional<FText>());
				}
				else
				{
					// OnComplete.ExecuteIfBound(nullptr, false, FText::FromString("UServersService::CreateJoinServerDelegate: Error while grabbing data from response"));
				}
			}
			else
			{
				const FText ErrorMsg = UNodecraftUtility::ParseMessage(Res, __FUNCTION__);
				if (ResponseCode == 400)
				{
					FJsonObjectWrapper ResJson;
					ResJson.JsonObjectFromString(Res.Get()->GetContentAsString());
					
					if (FString Code; ResJson.JsonObject->TryGetStringField(TEXT("code"), Code))
					{
						if (Code == "discovery.server.password_required" || Code == "discovery.server.invalid_password")
						{
							OnPasswordIncorrect.Broadcast();
						}
					}
				}
				// TODO: Adapt this to use the new message router
				else if (ResponseCode == 409)
				{
					FString ErrorCode = UNodecraftUtility::ParseMessageResultCode(Res);
					OnUserMustAcceptServerConsents.Broadcast(Server, ErrorMsg);
				}
			}
			UMessageRouterSubsystem::Get().RouteHTTPResult(Res, __FUNCTION__);
		}
		else
		{
			UMessageRouterSubsystem::Get().RouteHTTPResult(Res, __FUNCTION__);
			// OnComplete.ExecuteIfBound(nullptr, false, FText::FromString("UServersService::CreateJoinServerDelegate: Could not connect to the server."));
		}
	});
}

void UServerQueueService::CreateGetServerQueueDelegate(const FGetServerQueueDelegate& OnComplete,
	FHttpRequestCompleteDelegate& ReqCallbackOut)
{
	ReqCallbackOut.BindWeakLambda(this, [OnComplete, this](FHttpRequestPtr Req, FHttpResponsePtr Res, bool bConnectedSuccessfully)
	{
		if (bConnectedSuccessfully)
		{
			if (EHttpResponseCodes::IsOk(Res.Get()->GetResponseCode()))
			{
				FJsonObjectWrapper ResJson;
				ResJson.JsonObjectFromString(Res.Get()->GetContentAsString());
				if (const TSharedPtr<FJsonObject>& Data = ResJson.JsonObject->GetObjectField(TEXT("data")); Data.IsValid())
				{
					UServerQueueDataObject* QueueDataObject = UServerQueueDataObject::FromJson(Data.ToSharedRef());					
					OnGetServerQueue.ExecuteIfBound(QueueDataObject, true, TOptional<FText>());
					OnComplete.ExecuteIfBound(QueueDataObject, true, TOptional<FText>());
				}
				else
				{
					const FText ErrorMsg = FText::FromString("UServersService::CreateGetServerQueueDelegate: error while grabbing data from response");
					OnComplete.ExecuteIfBound(nullptr, false, ErrorMsg);
				}
			}
			else
			{
				const FText ErrorMsg = UNodecraftUtility::ParseMessage(Res, __FUNCTION__);
				OnComplete.ExecuteIfBound(nullptr, false, ErrorMsg);
			}
			UMessageRouterSubsystem::Get().RouteHTTPResult(Res, __FUNCTION__);
		}
		else
		{
			UMessageRouterSubsystem::Get().RouteFailureToConnect(__FUNCTION__);
			OnComplete.ExecuteIfBound(nullptr, false, FText::FromString("UServersService::CreateGetServerQueueDelegate: could not connect to server."));
		}
	});
}

void UServerQueueService::CreateRenewServerQueueDelegate(FHttpRequestCompleteDelegate& ReqCallbackOut)
{
	ReqCallbackOut.BindWeakLambda(this, [this](FHttpRequestPtr Req, FHttpResponsePtr Res, bool bConnectedSuccessfully)
	{
		if (bConnectedSuccessfully)
		{
			if (EHttpResponseCodes::IsOk(Res.Get()->GetResponseCode()))
			{
				FJsonObjectWrapper ResJson;
				ResJson.JsonObjectFromString(Res.Get()->GetContentAsString());
				if (const TSharedPtr<FJsonObject>& Data = ResJson.JsonObject->GetObjectField(TEXT("data")); Data.IsValid())
				{
					UServerQueueTokenDataObject* TokenDataObject = UServerQueueTokenDataObject::FromJson(Data.ToSharedRef());
					ServerQueueTokenDataObject = TokenDataObject;
					OnRenewServerQueue.ExecuteIfBound(TokenDataObject);
				}
				else
				{
					const FText ErrorMsg = FText::FromString("UServersService::CreateRenewServerQueueDelegate: error while grabbing data from response");
					// TODO: Handle errors in renewal in UI
					// Probably we should broadcast this error to the UI
				}
			}
			else
			{
				const FText ErrorMsg = UNodecraftUtility::ParseMessage(Res, __FUNCTION__);
				// TODO: Broadcast / handle error here
				// Probably we should broadcast this error to the UI
			}
			UMessageRouterSubsystem::Get().RouteHTTPResult(Res, __FUNCTION__);
		}
		else
		{
			UMessageRouterSubsystem::Get().RouteFailureToConnect(__FUNCTION__);
		}
	});
}

void UServerQueueService::CreateCancelServerQueueDelegate(const FSimpleServiceResponseDelegate& OnComplete,
	FHttpRequestCompleteDelegate& ReqCallbackOut)
{
	ReqCallbackOut.BindWeakLambda(this, [OnComplete, this](FHttpRequestPtr Req, FHttpResponsePtr Res, bool bConnectedSuccessfully)
	{
		if (bConnectedSuccessfully)
		{
			if (EHttpResponseCodes::IsOk(Res.Get()->GetResponseCode()))
			{
				FJsonObjectWrapper ResJson;
				ResJson.JsonObjectFromString(Res.Get()->GetContentAsString());
				if (const TSharedPtr<FJsonObject>& Data = ResJson.JsonObject->GetObjectField(TEXT("data")); Data.IsValid())
				{
					OnComplete.ExecuteIfBound(true, TOptional<FText>());
				}
				else
				{
					const FText ErrorMsg = FText::FromString("UServersService::CreateCancelServerQueueDelegate: error while grabbing data from response");
					OnComplete.ExecuteIfBound(false, ErrorMsg);
				}
				StopPollingServerQueue();
				OnServerQueueCancelled.Broadcast();
			}
			else
			{
				const FText ErrorMsg = UNodecraftUtility::ParseMessage(Res, __FUNCTION__);
				OnComplete.ExecuteIfBound(false, ErrorMsg);
			}
			UMessageRouterSubsystem::Get().RouteHTTPResult(Res, __FUNCTION__);
		}
		else
		{
			UMessageRouterSubsystem::Get().RouteFailureToConnect(__FUNCTION__);
			OnComplete.ExecuteIfBound(false, FText::FromString("UServersService::CreateCancelServerQueueDelegate: could not connect to server."));
		}
	});
}

bool UServerQueueService::JoinServer(UServerDataObject* Server, const TOptional<FString>& ServerPassword)
{
	FHttpRequestCompleteDelegate ReqCallback;
	CreateJoinServerDelegate(ReqCallback, Server);
	return UNodecraftStudioApi::JoinServer(this, Server->GetId(), ServerPassword, ReqCallback)->ProcessRequest();
}

bool UServerQueueService::GetServerQueue(FGetServerQueueDelegate& OnComplete)
{
	if (ServerQueueTokenDataObject == nullptr)
	{
		OnComplete.ExecuteIfBound(nullptr, false, FText::FromString("UServerQueueService::GetServerQueue: ServerQueueTokenDataObject is null"));
		return false;
	}
	FHttpRequestCompleteDelegate ReqCallback;
	CreateGetServerQueueDelegate(OnComplete, ReqCallback);
	return UNodecraftStudioApi::GetServerQueue(this, ServerQueueTokenDataObject->GetToken(), ReqCallback)->ProcessRequest();
}

bool UServerQueueService::RenewServerQueue()
{
	if (ServerQueueTokenDataObject == nullptr)
	{
		return false;
	}
	FHttpRequestCompleteDelegate ReqCallback;
	CreateRenewServerQueueDelegate(ReqCallback);
	return UNodecraftStudioApi::RenewServerQueue(this, ServerQueueTokenDataObject->GetToken(), ReqCallback)->ProcessRequest();
}

bool UServerQueueService::CancelServerQueue()
{
	auto EmptyComplete = FSimpleServiceResponseDelegate::CreateWeakLambda(this, [](bool bSuccess, TOptional<FText> Error) {});
	return CancelServerQueue(EmptyComplete);
}

bool UServerQueueService::CancelServerQueue(FSimpleServiceResponseDelegate& OnComplete)
{
	if (ServerQueueTokenDataObject == nullptr)
	{
		OnComplete.ExecuteIfBound(false, FText::FromString("UServerQueueService::CancelServerQueue: ServerQueueTokenDataObject is null"));
		OnServerQueueCancelled.Broadcast();
		return false;
	}
	
	FHttpRequestCompleteDelegate ReqCallback;
	CreateCancelServerQueueDelegate(OnComplete, ReqCallback);
	return UNodecraftStudioApi::CancelServerQueue(this, ServerQueueTokenDataObject->GetToken(), ReqCallback)->ProcessRequest();
}

void UServerQueueService::PollServerQueue()
{
	if (IsValid(ServerQueueTokenDataObject))
	{
		FGetServerQueueDelegate OnGetServerQueueComplete;
		OnGetServerQueueComplete.BindWeakLambda(this, [this](UServerQueueDataObject* RetrievedServerQueueDataObject, bool bSuccess, TOptional<FText> Error)
		{
			if (bSuccess)
			{
				ServerQueueDataObject = RetrievedServerQueueDataObject;
				if (RetrievedServerQueueDataObject)
				{
					// If we have a server session, it means we are ready to join our match! Otherwise, we need to keep polling.
					if (RetrievedServerQueueDataObject->GetServerSession())
					{
						OnGetServerSession.Broadcast(RetrievedServerQueueDataObject->GetServerSession());
						FString ConnectionString = RetrievedServerQueueDataObject->GetServerSession()->GetServerConnection() + "?join_token=" + RetrievedServerQueueDataObject->GetServerSession()->GetToken();
						// TODO: Make this more robust, and test it
						ConnectionString.ReplaceInline( TEXT( "tcp://" ), TEXT( "" ) );
						OnGetServerConnectionString.Broadcast(ConnectionString);
						// Stop polling the server queue
						StopPollingServerQueue();
						// Clear any stale data
						ServerQueueDataObject = nullptr;
						return;
					}

					// dispatch the new server queue so the UI can update the wait time
					OnGetServerQueue.ExecuteIfBound(ServerQueueDataObject, true, TOptional<FText>());
				}
				
				if (ServerQueueTokenDataObject->GetQueueRenews() <= FDateTime::UtcNow())
				{
					OnIdleCheckinRequired.Broadcast();
				}
			}
			else
			{
				// If we failed to get the server queue, we should cancel the queue and stop polling
				CancelServerQueue();
				OnServerQueueError.ExecuteIfBound(Error.GetValue());
			}
		});
		GetServerQueue(OnGetServerQueueComplete);
	}
}

void UServerQueueService::StartPollingServerQueue()
{
	StopPollingServerQueue();
	
	ServerQueuePollTimerDelegate.BindUObject(this, &UServerQueueService::PollServerQueue);
	const float PollingInterval = UNodecraftStudioApiSettings::Get().GetServerJoinPollingInterval();
	GetWorld()->GetTimerManager().SetTimer(ServerQueuePollTimerHandle, ServerQueuePollTimerDelegate, PollingInterval, true);
	OnStartedPollingServerQueue.Broadcast();
	
	CancelQueueTimerDelegate.BindWeakLambda(this, [this]()
	{
		CancelServerQueue();
	});
	const float QueueExpiresIn = (ServerQueueTokenDataObject->GetQueueExpires() - FDateTime::UtcNow()).GetTotalSeconds();
	GetWorld()->GetTimerManager().SetTimer(CancelQueueTimerHandle, CancelQueueTimerDelegate, QueueExpiresIn, false);
}

void UServerQueueService::StopPollingServerQueue()
{
	ServerQueuePollTimerDelegate.Unbind();
	if (ServerQueuePollTimerHandle.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(ServerQueuePollTimerHandle);
	}

	CancelQueueTimerDelegate.Unbind();
	if (CancelQueueTimerHandle.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(CancelQueueTimerHandle);
	}
}

void UServerQueueService::AddGetServerQueueDelegate(FGetServerQueueDelegate Delegate)
{
	Delegate.ExecuteIfBound(ServerQueueDataObject, true, TOptional<FText>());
	OnGetServerQueue.BindWeakLambda(this, [Delegate](UServerQueueDataObject* ServerQueue, bool bSuccess, TOptional<FText> Error)
	{
		Delegate.ExecuteIfBound(ServerQueue, bSuccess, Error);
	});
}

void UServerQueueService::RemoveGetServerQueueDelegate()
{
	OnGetServerQueue.Unbind();
}
