// Fill out your copyright notice in the Description page of Project Settings.


#include "Services/IdentService.h"

#include "API/DiscoveryAPI.h"
#include "Models/PlayerSession.h"
#include "Models/Consents.h"
#include "DataTypes/IdentityType.h"
#include "DeveloperSettings/SupportedAuthPlatformsPreprocessor.h"
#include "Services/GameService.h"
#include "Utility/NodecraftUtility.h"

#if NC_AUTH_STEAM_ENABLED
#include "SteamAuthSubsystem.h"
#endif

DEFINE_LOG_CATEGORY_STATIC(LogIdentService, Log, All);

void UIdentService::SetHasFailedWithIdentityType(const EIdentityType IdentityType)
{
	FailedIdentityTypes.Add(IdentityType);
}

void UIdentService::ClearFailedIdentityTypes()
{
	FailedIdentityTypes.Empty();
}

FHttpRequestCompleteDelegate UIdentService::CreateSendTokenCompleteDelegate(FAcceptConsentsDelegate AcceptConsentsDelegate, FSendIdentTokenResponseDelegate OnComplete)
{
	FHttpRequestCompleteDelegate ReqCallback;

	ReqCallback.BindWeakLambda(this, [this, OnComplete, AcceptConsentsDelegate](FHttpRequestPtr Req, FHttpResponsePtr Res, bool bConnectedSuccessfully) mutable
	{
		FSendIdentTokenResponseDelegateData DelegateData;
		if (bConnectedSuccessfully)
		{
			if (EHttpResponseCodes::IsOk(Res.Get()->GetResponseCode()))
			{
				DelegateData.Status = ESendIdentTokenResponseStatus::Success;
				OnComplete.ExecuteIfBound(DelegateData);
			}
			else if (Res.Get()->GetResponseCode() == EHttpResponseCodes::Conflict)
			{
				DelegateData.AcceptConsentsDelegate = AcceptConsentsDelegate;
				DelegateData.Status = ESendIdentTokenResponseStatus::RequiresConsents;
				OnComplete.ExecuteIfBound(DelegateData);
			}
			else
			{
				// TODO: Handle error here
				DelegateData.Status = ESendIdentTokenResponseStatus::Error;
				DelegateData.Error = FText::FromString("TODO: Parse error from JSON");
				OnComplete.ExecuteIfBound(DelegateData);
			}
		}
		else
		{
			DelegateData.Status = ESendIdentTokenResponseStatus::Error;
			DelegateData.Error = FText::FromString("Failed to connect to server. Please try again.");
			OnComplete.ExecuteIfBound(DelegateData);
		}
	});

	return ReqCallback;
}
#if NC_AUTH_STEAM_ENABLED
bool UIdentService::SendIdentTokenSteam(const FString& Ticket, const FString& Identity, FSendIdentTokenResponseDelegate& OnComplete, TOptional<FGameConsents> GameConsents)
{
	FJsonObjectWrapper Json;
	Json.JsonObject->SetStringField("type", "steam");
	FJsonObjectWrapper Data;
	Data.JsonObject->SetStringField("ticket", Ticket);
	Data.JsonObject->SetStringField("identity", Identity);
	Json.JsonObject->SetObjectField("data", Data.JsonObject);
	
	if (GameConsents.IsSet())
	{
		FGameConsents Consents = GameConsents.GetValue();
		Json.JsonObject->SetArrayField("consents", Consents.ToJsonArray());
	}

	FAcceptConsentsDelegate AcceptConsentsDelegate = FAcceptConsentsDelegate::CreateWeakLambda(this, [this, OnComplete, Ticket, Identity](const FGameConsents& Consents) mutable
	{
		SendIdentTokenSteam(Ticket, Identity, OnComplete, Consents);
	});

	FHttpRequestCompleteDelegate ReqCallback = CreateSendTokenCompleteDelegate(AcceptConsentsDelegate, OnComplete);

	LastAcceptConsentsDelegate = AcceptConsentsDelegate;
	LastIdentTokenJsonPayload = Json;
	return UDiscoveryAPI::SendIdentToken(this, Json, ReqCallback)->ProcessRequest();
}

void UIdentService::AttemptSteamAuth()
{
	FSendIdentTokenResponseDelegate SendTokenResponseDelegate;
	SendTokenResponseDelegate.BindWeakLambda(this, [this](const FSendIdentTokenResponseDelegateData Data)
	{
		switch (Data.Status)
		{
		case ESendIdentTokenResponseStatus::Success:
			OnAutoAuthSuccess.ExecuteIfBound();
			UE_LOG(LogIdentService, Log, TEXT("Auto auth success"));
			break;
		case ESendIdentTokenResponseStatus::RequiresConsents:
			OnAutoAuthRequiresConsent.ExecuteIfBound(Data.AcceptConsentsDelegate, "steam");
			UE_LOG(LogIdentService, Log, TEXT("Auto auth requires consents"));
			break;
		case ESendIdentTokenResponseStatus::Error:
			FailedIdentityTypes.Add(EIdentityType::Steam);
			OnAutoAuthFailure.ExecuteIfBound();
			UE_LOG(LogIdentService, Error, TEXT("Auto auth error: %s"), *Data.Error.ToString());
			break;
		default:
			unimplemented();
		}
	});
	
	FOnIdentTokenReceived OnIdentTokenReceived;
	OnIdentTokenReceived.BindWeakLambda(this, [this, SendTokenResponseDelegate](bool bSuccess, const FString& Token, const FString& Error) mutable
	{
		if (bSuccess)
		{
			// Retrieved auth token. Send it to the server
			SendIdentTokenSteam(Token, "test", SendTokenResponseDelegate);
			UE_LOG(LogIdentService, Log, TEXT("Auto auth success"));
		}
		else
		{
			// Attempt next auth method
			FailedIdentityTypes.Add(EIdentityType::Steam);
			OnAutoAuthFailure.ExecuteIfBound();
			UE_LOG(LogIdentService, Error, TEXT("Steam auth error: %s"), *Error);
		}
	});

	
	USteamAuthSubsystem::Get().GetIdentTokenAsync(OnIdentTokenReceived);
}

#endif
bool UIdentService::SendEmailToken(FString Email, FString Token, FSendIdentTokenResponseDelegate& OnComplete, TOptional<FGameConsents> GameConsents)
{
	FJsonObjectWrapper Json;
	Json.JsonObject->SetStringField("type", "email");
	FJsonObjectWrapper Data;
	Data.JsonObject->SetStringField("email", Email);
	Data.JsonObject->SetStringField("code", Token);
	Json.JsonObject->SetObjectField("data", Data.JsonObject);
	
	if (GameConsents.IsSet())
	{
		FGameConsents Consents = GameConsents.GetValue();
		Json.JsonObject->SetArrayField("consents", Consents.ToJsonArray());
	}

	FAcceptConsentsDelegate AcceptConsentsDelegate = FAcceptConsentsDelegate::CreateWeakLambda(this, [this, OnComplete, Email, Token](const FGameConsents& Consents) mutable
	{
		SendEmailToken(Email, Token, OnComplete, Consents);
	});

	FHttpRequestCompleteDelegate ReqCallback = CreateSendTokenCompleteDelegate(AcceptConsentsDelegate, OnComplete);

	LastAcceptConsentsDelegate = AcceptConsentsDelegate;
	LastIdentTokenJsonPayload = Json;
	return UDiscoveryAPI::SendIdentToken(this, Json, ReqCallback)->ProcessRequest();
}

bool UIdentService::VerifyPlayerSession(FPlayerSessionResponseDelegate OnComplete)
{
	FHttpRequestCompleteDelegate ReqCallback;

	ReqCallback.BindWeakLambda(this, [this, OnComplete](FHttpRequestPtr Req, FHttpResponsePtr Res, bool bConnectedSuccessfully) mutable
	{
		FPlayerSession Session = FPlayerSession();
		if (bConnectedSuccessfully)
		{
			if (EHttpResponseCodes::IsOk(Res.Get()->GetResponseCode()))
			{
				FJsonObjectWrapper ResJson;
				ResJson.JsonObjectFromString(Res.Get()->GetContentAsString());
				if (const TSharedPtr<FJsonObject>& Data = ResJson.JsonObject->GetObjectField("data"); Data.IsValid())
				{
					Session = FPlayerSession::FromJson(Data);
					UDiscoverySessionManager::Get().SetPlayerSession(Session);
					OnComplete.ExecuteIfBound(Session, true, FText::GetEmpty());
				}
				else
				{
					OnComplete.ExecuteIfBound(Session, false, FText::FromString("UIdentService::GetPlayerSession: Failed to get data from response"));
				}
			}
			else
			{
				const FText ErrorMessage = UNodecraftUtility::ParseError(Res, FString(__FUNCTION__));
				OnComplete.ExecuteIfBound(Session, false, ErrorMessage);
			}
		}
		else
		{
			OnComplete.ExecuteIfBound(Session, false, FText::FromString("UIdentService::GetPlayerSession: Failed to connect to server"));
		}
	});
	
	return UDiscoveryAPI::GetPlayerSession(this, ReqCallback)->ProcessRequest();
}

bool UIdentService::ResendPreviousTokenWithConsents(const FSendIdentTokenResponseDelegate& Delegate,
                                                    const FGameConsents& InConsents)
{
	const FJsonObjectWrapper Json = LastIdentTokenJsonPayload;
	Json.JsonObject->SetArrayField("consents", InConsents.ToJsonArray());
	
	LastIdentTokenJsonPayload = Json;

	FHttpRequestCompleteDelegate ReqCallback = CreateSendTokenCompleteDelegate(LastAcceptConsentsDelegate, Delegate);
	
	return UDiscoveryAPI::SendIdentToken(this, Json, ReqCallback)->ProcessRequest();
}

bool DoesClientSupportIdentityType(const EIdentityType IdentityType)
{
	bool bIsSupported = false;
	switch (IdentityType)
	{
	case EIdentityType::Steam:
		bIsSupported = NC_AUTH_STEAM_ENABLED;
		break;
	case EIdentityType::Email:
		bIsSupported = NC_AUTH_EMAIL_ENABLED;
		break;
	default:
		break;
	}
	return bIsSupported;
}

EIdentityType UIdentService::GetNextAutoAuthPlatformToTry()
{
	EIdentityType IdentityTypeToTry = EIdentityType::UNDEFINED;
	for (const EIdentityType BackendSupportedIdentType : UGameService::Get().GetCachedIdentTypes())
	{
		// Skip manual auth. We don't care about it in this context
		if (BackendSupportedIdentType == EIdentityType::Email) { continue; }

		if (DoesClientSupportIdentityType(BackendSupportedIdentType) && !FailedIdentityTypes.Contains(BackendSupportedIdentType))
		{
			IdentityTypeToTry = BackendSupportedIdentType;
			break;
		}
	}
	return IdentityTypeToTry;
}

bool UIdentService::IsAutoAuthAvailable()
{
	EIdentityType IdentityTypeToTry = GetNextAutoAuthPlatformToTry();
	return IdentityTypeToTry != EIdentityType::UNDEFINED;
}

void UIdentService::AttemptAutoAuth()
{
	EIdentityType IdentityTypeToTry = GetNextAutoAuthPlatformToTry();
	if (IdentityTypeToTry == EIdentityType::UNDEFINED)
	{
		UE_LOG(LogIdentService, Error, TEXT("Tried to auto auth but no identity types available to try. Failing."));
		OnAutoAuthFailure.ExecuteIfBound();
		return;
	}
	switch (IdentityTypeToTry)
	{
	case EIdentityType::Steam:
#if NC_AUTH_STEAM_ENABLED
		AttemptSteamAuth();
		break;
#else
			[[fallthrough]];
#endif
	default:
		UE_LOG(LogIdentService, Error, TEXT("Tried to auto auth with unsupported identity type. Failing."));
		OnAutoAuthFailure.ExecuteIfBound();
		break;
	}
}
