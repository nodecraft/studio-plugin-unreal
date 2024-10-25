// Nodecraft, Inc. © 2012-2024, All Rights Reserved.


#include "Services/LinksService.h"

#include "NodecraftLogCategories.h"
#include "API/NodecraftStudioApi.h"
#include "DataTypes/LinkTypes.h"
#include "DataTypes/PlayerConnectionSubjects.h"
#include "DeveloperSettings/NodecraftStudioApiSettings.h"
#include "GenericPlatform/GenericPlatformHttp.h"
#include "Models/PlayerConnectionDataObject.h"
#include "Subsystems/MessageRouterSubsystem.h"
#include "TimerManager.h"
#include "Utility/NodecraftUtility.h"

#define POLL_RATE 5.0f

// TODO: Move me to nodecraft utility
void ULinksService::OpenURLInBrowser(ELinkType LinkType, FString URL)
{
	const FString Root = UNodecraftStudioApiSettings::Get().GetApiRoot();
	
	FString Type = "";
	switch (LinkType)
	{
	case ELinkType::Internal:
		Type = "internal";
		break;
	case ELinkType::External:
		Type = "external";
		break;
	default: ;
	}

	const FString RedirectUrl = Root + "link/" + Type + "/" + FGenericPlatformHttp::UrlEncode(*URL);
	FPlatformProcess::LaunchURL(*RedirectUrl, nullptr, nullptr);
}

bool ULinksService::CreatePlayerConnection_Internal(const FString& SubjectStr, FOnPlayerConnection OnComplete)
{
	FHttpRequestCompleteDelegate ReqCallback;
	ReqCallback.BindWeakLambda(this, [this, OnComplete]
		(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
	{
		if (bConnectedSuccessfully)
		{
			if (EHttpResponseCodes::IsOk(Response.Get()->GetResponseCode()))
			{
				FJsonObjectWrapper ResJson;
				ResJson.JsonObjectFromString(Response.Get()->GetContentAsString());
				if (const TSharedPtr<FJsonObject> Data = ResJson.JsonObject->GetObjectField(TEXT("data")); Data.IsValid())
				{
					UPlayerConnectionDataObject* PlayerConnectionDataObject = UPlayerConnectionDataObject::FromJson(Data.ToSharedRef());
					CurrentConnectionId = PlayerConnectionDataObject->GetId();
					OnComplete.ExecuteIfBound(PlayerConnectionDataObject, true, TOptional<FText>());
				}
				else
				{
					const FText ErrorMsg = FText::FromString("CreatePlayerConnection: data is invalid!");
					UE_LOG(LogNodecraft_QrCodeLinks, Error, TEXT("%s"), *ErrorMsg.ToString());
					OnComplete.ExecuteIfBound(nullptr, false, ErrorMsg);
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
			const FText ErrorMsg = FText::FromString("Could not connect to the server.");
			UE_LOG(LogNodecraft_QrCodeLinks, Error, TEXT("%s"), *ErrorMsg.ToString());
			OnComplete.ExecuteIfBound(nullptr, false, ErrorMsg);
		}
	});
	return UNodecraftStudioApi::CreatePlayerConnection(this, SubjectStr, ReqCallback)->ProcessRequest();
}

bool ULinksService::CreatePlayerConnection(EPlayerConnectionSubject Subject, FOnPlayerConnection OnComplete)
{
	return CreatePlayerConnection_Internal(EPlayerConnectionSubjectHelper::ToString(Subject), OnComplete);
}

bool ULinksService::CreatePlayerConnectionForServer(EPlayerConnectionSubject Subject, const FString& ServerId, FOnPlayerConnection OnComplete)
{
	return CreatePlayerConnection_Internal(EPlayerConnectionSubjectHelper::ToString(Subject) + "/server/" + ServerId,
	                                       OnComplete);
}

void ULinksService::StartPollingForPlayerConnectionStatus(UWorld* World)
{
	FTimerDelegate TimerDelegate;

	TimerDelegate.BindWeakLambda(this, [this]()
	{
		GetPlayerConnection(CurrentConnectionId, FOnPlayerConnection());
	});

	if (World)
	{
		UE_LOG(LogNodecraft_QrCodeLinks, Log, TEXT("Polling started for player connection status."));
		World->GetTimerManager().SetTimer(PollingTimerHandle, TimerDelegate, POLL_RATE, true, 0.0f);
	}
	else
	{
		UE_LOG(LogNodecraft_QrCodeLinks, Error, TEXT("World null. Polling will not be started for player connection status."));
	}
}

void ULinksService::StopPollingForPlayerConnectionStatus(UWorld* World)
{
	if (World)
	{
		UE_LOG(LogNodecraft_QrCodeLinks, Log, TEXT("Polling stopped for player connection status."));
		World->GetTimerManager().ClearTimer(PollingTimerHandle);
	}
	else
	{
		UE_LOG(LogNodecraft_QrCodeLinks, Error, TEXT("World null. Polling will not be stopped."));
	}
}

bool ULinksService::GetPlayerConnection(const FString& ConnectionId, FOnPlayerConnection OnComplete)
{
	FHttpRequestCompleteDelegate ReqCallback;
	ReqCallback.BindWeakLambda(this, [this, OnComplete]
		(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
	{
		if (bConnectedSuccessfully)
		{
			if (EHttpResponseCodes::IsOk(Response.Get()->GetResponseCode()))
			{
				FJsonObjectWrapper ResJson;
				ResJson.JsonObjectFromString(Response.Get()->GetContentAsString());
				if (const TSharedPtr<FJsonObject> Data = ResJson.JsonObject->GetObjectField(TEXT("data")); Data.IsValid())
				{
					UPlayerConnectionDataObject* PlayerConnectionDataObject = UPlayerConnectionDataObject::FromJson(Data.ToSharedRef());
					OnComplete.ExecuteIfBound(PlayerConnectionDataObject, true, TOptional<FText>());
					// If we succeed, instead of normally routing our HTTP result we're going to create our own
					// special message that appends the status property of our data to the 'code' property of the response
					// Our modal UI listens for this special message and updates the UI accordingly
					const FString MessageResultCode = UNodecraftUtility::ParseMessageResultCode(Response);
					const FString ConnectionStatus = PlayerConnectionDataObject->GetConnectionStatusRaw();
					// const FString Message = FString::Printf(TEXT("%s.%s"), *MessageResultCode, *ConnectionStatus);
					UMessageRouterSubsystem::Get().RouteMessage(MessageResultCode,
						FText::FromString(ConnectionStatus),
					                                            EAlertType::Message);
				}
				else
				{
					const FText ErrorMsg = FText::FromString("Player Connection data is invalid!");
					UE_LOG(LogNodecraft_QrCodeLinks, Error, TEXT("%s"), *ErrorMsg.ToString());
					OnComplete.ExecuteIfBound(nullptr, false, ErrorMsg);
					UMessageRouterSubsystem::Get().RouteHTTPResult(Response, __FUNCTION__);
				}
			}
			else
			{
				const FText ErrorMsg = UNodecraftUtility::ParseMessage(Response, __FUNCTION__);
				OnComplete.ExecuteIfBound(nullptr, false, ErrorMsg);
				UMessageRouterSubsystem::Get().RouteHTTPResult(Response, __FUNCTION__);
			}
		}
		else
		{
			UMessageRouterSubsystem::Get().RouteFailureToConnect(__FUNCTION__);
			const FText ErrorMsg = FText::FromString("Could not connect to the server.");
			UE_LOG(LogNodecraft_QrCodeLinks, Error, TEXT("%s"), *ErrorMsg.ToString());
			OnComplete.ExecuteIfBound(nullptr, false, ErrorMsg);
		}
	});
	return UNodecraftStudioApi::GetPlayerConnection(this, ConnectionId, ReqCallback)->ProcessRequest();
}

bool ULinksService::ConsumeChallengeResponse(const FString& Code)
{
	// log
	UE_LOG(LogNodecraft_QrCodeLinks, Log, TEXT("ConsumeChallengeResponse: %s"), *Code);
	FHttpRequestCompleteDelegate ReqCallback;
	ReqCallback.BindWeakLambda(this, [this]
		(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
	{
		if (bConnectedSuccessfully)
		{
			//log response with code
			UE_LOG(LogNodecraft_QrCodeLinks, Log, TEXT("ConsumeChallengeResponse: Code: %d Content: %s"), Response->GetResponseCode(), *Response->GetContentAsString());
			
			UMessageRouterSubsystem::Get().RouteHTTPResult(Response, __FUNCTION__);
		}
		else
		{
			UMessageRouterSubsystem::Get().RouteFailureToConnect(__FUNCTION__);
		}
	});
	return UNodecraftStudioApi::ConsumePlayerConnectionChallengeResponse(this, CurrentConnectionId,Code, ReqCallback)->ProcessRequest();
}

#undef POLL_RATE
