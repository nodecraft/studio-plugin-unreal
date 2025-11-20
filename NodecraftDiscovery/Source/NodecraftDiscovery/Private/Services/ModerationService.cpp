// Nodecraft, Inc. © 2012-2024, All Rights Reserved.


#include "Services/ModerationService.h"
#include "Interfaces/IHttpResponse.h"

#include "JsonObjectWrapper.h"
#include "NodecraftLogCategories.h"
#include "API/NodecraftStudioApi.h"
#include "Stores/ModerationStore.h"
#include "Subsystems/MessageRouterSubsystem.h"
#include "Utility/NodecraftUtility.h"

void UModerationService::CreatePlayersListModerationDelegate(const FGetPlayerServerDetailsList& OnComplete,
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
				if (const TArray<TSharedPtr<FJsonValue>>& Data = ResJson.JsonObject->GetArrayField(TEXT("data")); Data.Num() > 0)
				{
					TArray<UPlayerServerDetailsDataObject*> Players;
					for (TSharedPtr<FJsonValue> JsonValue : Data)
					{
						UPlayerServerDetailsDataObject* PlayerServerDetailsDataObject = UPlayerServerDetailsDataObject::FromJson(JsonValue.Get()->AsObject().ToSharedRef());
						Players.Add(PlayerServerDetailsDataObject);
					}
					OnComplete.ExecuteIfBound(Players, true, FText());
				}
				else
				{
					// request succeeded but no data
					OnComplete.ExecuteIfBound({}, true, FText());
				}
			}
			else
			{
				FJsonObjectWrapper ResJson;
				ResJson.JsonObjectFromString(Res.Get()->GetContentAsString());
				if (const FString Message = ResJson.JsonObject->GetStringField(TEXT("message")); Message.IsEmpty() == false)
				{
					const FString ErrorText = FString::Printf(TEXT("UModerationService::CreatePlayersListModerationDelegate: Error code: %d. Message: %ls"), Res.Get()->GetResponseCode(), *Message);
					OnComplete.ExecuteIfBound({}, false, TOptional<FText>(FText::FromString(ErrorText)));
					
				}
				else
				{
					const FString ErrorText = FString::Printf(TEXT("UModerationService::CreatePlayersListModerationDelegate: Received %d response w/out error message"), Res.Get()->GetResponseCode());
					OnComplete.ExecuteIfBound({}, false, TOptional<FText>(FText::FromString(ErrorText)));
				}
			}
			UMessageRouterSubsystem::Get().RouteHTTPResult(Res, __FUNCTION__);
		}
		else
		{
			UMessageRouterSubsystem::Get().RouteFailureToConnect(__FUNCTION__);
			OnComplete.ExecuteIfBound({}, false, FText::FromString("UModerationService::CreatePlayersListModerationDelegate: Failed to connect to server"));
		}
	});
}

void UModerationService::CreateServerPlayersDetailDelegate(const FGetPlayerServerDetails& OnComplete,
                                                             FHttpRequestCompleteDelegate& ReqCallbackOut)
{
	ReqCallbackOut.BindWeakLambda(this, [OnComplete](FHttpRequestPtr Req, FHttpResponsePtr Res, bool bConnectedSuccessfully)
	{
		UPlayerServerDetailsDataObject* PlayerServerDetailsDataObject = nullptr;
		if (bConnectedSuccessfully)
		{
			if (EHttpResponseCodes::IsOk(Res.Get()->GetResponseCode()))
			{
				FJsonObjectWrapper ResJson;
				ResJson.JsonObjectFromString(Res.Get()->GetContentAsString());
				if (const TSharedPtr<FJsonObject>& Data = ResJson.JsonObject->GetObjectField(TEXT("data")); Data.IsValid())
				{
					PlayerServerDetailsDataObject = UPlayerServerDetailsDataObject::FromJson(Data.ToSharedRef());
					OnComplete.ExecuteIfBound(PlayerServerDetailsDataObject, true, FText());
					
				}
				else
				{
					OnComplete.ExecuteIfBound(PlayerServerDetailsDataObject, false, FText::FromString("UModerationService::CreateServerPlayersDetailDelegate: Error while grabbing data from response"));
				}
			}
			else
			{
				FJsonObjectWrapper ResJson;
				ResJson.JsonObjectFromString(Res.Get()->GetContentAsString());
				if (const FString Message = ResJson.JsonObject->GetStringField(TEXT("message")); Message.IsEmpty() == false)
				{
					const FString ErrorText = FString::Printf(TEXT("UModerationService::CreateServerPlayersDetailDelegate: Error code: %d. Message: %ls"), Res.Get()->GetResponseCode(), *Message);
					OnComplete.ExecuteIfBound(PlayerServerDetailsDataObject, false, TOptional<FText>(FText::FromString(ErrorText)));
				}
				else
				{
					const FString ErrorText = FString::Printf(TEXT("UModerationService::CreateServerPlayersDetailDelegate: Received %d response w/out error message"), Res.Get()->GetResponseCode());
					OnComplete.ExecuteIfBound(PlayerServerDetailsDataObject, false, TOptional<FText>(FText::FromString(ErrorText)));
				}
			}
			UMessageRouterSubsystem::Get().RouteHTTPResult(Res, __FUNCTION__);
		}
		else
		{
			UMessageRouterSubsystem::Get().RouteFailureToConnect(__FUNCTION__);
			OnComplete.ExecuteIfBound(PlayerServerDetailsDataObject, false, FText::FromString("UModerationService::CreatePlayersListModerationDelegate: Failed to connect to server"));
		}
	});
}

bool UModerationService::GetModerators(const UWorld* World, const FString& ServerId)
{
	return GetModeratorsRequest(World, ServerId)->ProcessRequest();
}

bool UModerationService::GetOnlinePlayers(const UWorld* World, const FString& ServerId)
{
	return GetOnlinePlayersRequest(World, ServerId)->ProcessRequest();
}

bool UModerationService::GetOfflinePlayers(const UWorld* World, const FString& ServerId)
{
	return GetOfflinePlayersRequest(World, ServerId)->ProcessRequest();
}

bool UModerationService::GetBannedPlayers(const UWorld* World, const FString& ServerId)
{
	return GetBannedPlayersRequest(World, ServerId)->ProcessRequest();
}

bool UModerationService::GetOwner(const UWorld* World, const FString& ServerId)
{
	return GetOwnerRequest(World, ServerId)->ProcessRequest();
}

TSharedRef<IHttpRequest> UModerationService::GetModeratorsRequest(const UWorld* World, const FString& ServerId)
{
	FGetPlayerServerDetailsList OnComplete;
	OnComplete.BindWeakLambda(this, [this, World](const TArray<UPlayerServerDetailsDataObject*>& Players, bool bSuccess, TOptional<FText> Error)
	{
		if (bSuccess)
		{
			UE_LOG(LogNodecraft_Moderation, Verbose, TEXT("GetModerators succeeded"));
			if (UModerationStore* ModerationStore = UModerationStore::Get(World))
			{
				ModerationStore->SetModerators(Players);
			}
		}
		else
		{
			UE_LOG(LogNodecraft_Moderation, Verbose, TEXT("GetModerators failed: %s"), *Error.GetValue().ToString());
		}
	});
	FHttpRequestCompleteDelegate HttpRequestComplete;
	CreatePlayersListModerationDelegate(OnComplete, HttpRequestComplete);
	
	return UNodecraftStudioApi::GetModerators(this, ServerId, HttpRequestComplete);
}

TSharedRef<IHttpRequest> UModerationService::GetOnlinePlayersRequest(const UWorld* World, const FString& ServerId)
{
	FGetPlayerServerDetailsList OnComplete;
	OnComplete.BindWeakLambda(this, [this, World](const TArray<UPlayerServerDetailsDataObject*>& Players, bool bSuccess, TOptional<FText> Error)
	{
		if (bSuccess)
		{
			UE_LOG(LogNodecraft_Moderation, Verbose, TEXT("GetOnlinePlayers succeeded"));
			if (UModerationStore* ModerationStore = UModerationStore::Get(World))
			{
				ModerationStore->SetOnlinePlayers(Players);
			}
		}
		else
		{
			UE_LOG(LogNodecraft_Moderation, Verbose, TEXT("GetOnlinePlayers failed: %s"), *Error.GetValue().ToString());
		}
	});
	
	FHttpRequestCompleteDelegate HttpRequestComplete;
	CreatePlayersListModerationDelegate(OnComplete, HttpRequestComplete);
	return UNodecraftStudioApi::GetOnlinePlayersForModeration(this, ServerId, HttpRequestComplete);
}

TSharedRef<IHttpRequest> UModerationService::GetOfflinePlayersRequest(const UWorld* World, const FString& ServerId)
{
	FGetPlayerServerDetailsList OnComplete;
	OnComplete.BindWeakLambda(this, [this, World](const TArray<UPlayerServerDetailsDataObject*>& Players, bool bSuccess, TOptional<FText> Error)
	{
		if (bSuccess)
		{
			UE_LOG(LogNodecraft_Moderation, Verbose, TEXT("GetOfflinePlayers succeeded"));
			if (UModerationStore* ModerationStore = UModerationStore::Get(World))
			{
				ModerationStore->SetOfflinePlayers(Players);
			}
		}
		else
		{
			UE_LOG(LogNodecraft_Moderation, Verbose, TEXT("GetOfflinePlayers failed: %s"), *Error.GetValue().ToString());
		}
	});
	FHttpRequestCompleteDelegate HttpRequestComplete;
	CreatePlayersListModerationDelegate(OnComplete, HttpRequestComplete);
	return UNodecraftStudioApi::GetOfflinePlayersForModeration(this, ServerId, HttpRequestComplete);
}

TSharedRef<IHttpRequest> UModerationService::GetBannedPlayersRequest(const UWorld* World, const FString& ServerId)
{
	FGetPlayerServerDetailsList OnComplete;
	OnComplete.BindWeakLambda(this, [this, World](const TArray<UPlayerServerDetailsDataObject*>& Players, bool bSuccess, TOptional<FText> Error)
	{
		if (bSuccess)
		{
			UE_LOG(LogNodecraft_Moderation, Verbose, TEXT("GetBannedPlayers succeeded"));
			if (UModerationStore* ModerationStore = UModerationStore::Get(World))
			{
				ModerationStore->SetBannedPlayers(Players);
			}
		}
		else
		{
			UE_LOG(LogNodecraft_Moderation, Verbose, TEXT("GetBannedPlayers failed: %s"), *Error.GetValue().ToString());
		}
	});
	FHttpRequestCompleteDelegate HttpRequestComplete;
	CreatePlayersListModerationDelegate(OnComplete, HttpRequestComplete);
	return UNodecraftStudioApi::GetBannedPlayers(this, ServerId, HttpRequestComplete);
}

TSharedRef<IHttpRequest> UModerationService::GetOwnerRequest(const UWorld* World, const FString& ServerId)
{
	FGetPlayerServerDetails OnComplete;
	OnComplete.BindWeakLambda(this, [this, World](UPlayerServerDetailsDataObject* Owner, bool bSuccess, TOptional<FText> Error)
	{
		if (bSuccess)
		{
			UE_LOG(LogNodecraft_Moderation, Verbose, TEXT("GetOwner succeeded"));
			if (UModerationStore* ModerationStore = UModerationStore::Get(World))
			{
				ModerationStore->SetOwner(Owner);
			}
		}
		else
		{
			UE_LOG(LogNodecraft_Moderation, Verbose, TEXT("GetOwner failed: %s"), *Error.GetValue().ToString());
		}
	});
	FHttpRequestCompleteDelegate HttpRequestComplete;
	CreateServerPlayersDetailDelegate(OnComplete, HttpRequestComplete);
	return UNodecraftStudioApi::GetOwner(this, ServerId, HttpRequestComplete);
}

bool UModerationService::GetModerationActionReasons(UWorld* World, FGetModActionReasonsDelegate OnComplete)
{
	// If we already have the reasons, return them
	if (UModerationStore* ModerationStore = UModerationStore::Get(World))
	{
		if (ModerationStore->GetModerationReasons().Num() > 0)
		{
			OnComplete.ExecuteIfBound(ModerationStore->GetModerationReasons(), true, TOptional<FText>());
			return true;
		}
	}
	// Otherwise, fetch them from the server
	FHttpRequestCompleteDelegate HttpRequestComplete;
	HttpRequestComplete.BindWeakLambda(this, [OnComplete, World](FHttpRequestPtr Req, FHttpResponsePtr Res, bool bConnectedSuccessfully)
	{
		TArray<UModerationReasonDataObject*> Reasons = {};
		if (bConnectedSuccessfully)
		{
			if (EHttpResponseCodes::IsOk(Res.Get()->GetResponseCode()))
			{
				FJsonObjectWrapper ResJson;
				ResJson.JsonObjectFromString(Res.Get()->GetContentAsString());
				if (const TArray<TSharedPtr<FJsonValue>> Data = ResJson.JsonObject->GetArrayField(TEXT("data")); Data.Num() > 0)
				{
					for (const TSharedPtr<FJsonValue>& JsonValue : Data)
					{
						UModerationReasonDataObject* Reason = UModerationReasonDataObject::FromJson(JsonValue->AsObject().ToSharedRef());
						Reasons.Add(Reason);
					}
					if (UModerationStore* ModerationStore = UModerationStore::Get(World))
					{
						ModerationStore->SetModerationReasons(Reasons);
					}
					OnComplete.ExecuteIfBound(Reasons, true, TOptional<FText>());
				}
				else
				{
					OnComplete.ExecuteIfBound(Reasons, false, FText::FromString("UModerationService::GetModerationActionReasons: Call succeeded but no elements in data."));
				}
			}
			else
			{
				const FText ErrorMsg = UNodecraftUtility::ParseMessage(Res, __FUNCTION__);
				OnComplete.ExecuteIfBound(Reasons, false, ErrorMsg);
			}
			UMessageRouterSubsystem::Get().RouteHTTPResult(Res, __FUNCTION__);
		}
		else
		{
			UMessageRouterSubsystem::Get().RouteFailureToConnect(__FUNCTION__);
			OnComplete.ExecuteIfBound(Reasons, false, FText::FromString("UModerationService::GetModerationActionReasons: Failed to connect to server"));
		}
	});
	return UNodecraftStudioApi::GetModerationActionReasons(this, HttpRequestComplete)->ProcessRequest();
}

bool UModerationService::KickPlayers(const TArray<FString> PlayerIds, const FString& ServerId,
	const UModerationReasonDataObject* Reason, const FString& ReasonNotes, FPlayersKickedDelegate OnComplete)
{
	// Create the request
	FHttpRequestCompleteDelegate HttpRequestComplete;
	// Bind the lambda to the delegate
	HttpRequestComplete.BindWeakLambda(this, [OnComplete](FHttpRequestPtr Req, FHttpResponsePtr Res, bool bConnectedSuccessfully)
	{
		TArray<FPlayerKickDataObject> Kicks = {};
		if (bConnectedSuccessfully)
		{
			if (EHttpResponseCodes::IsOk(Res.Get()->GetResponseCode()))
			{
				FJsonObjectWrapper ResJson;
				ResJson.JsonObjectFromString(Res.Get()->GetContentAsString());
				if (const TArray<TSharedPtr<FJsonValue>> Data = ResJson.JsonObject->GetArrayField(TEXT("data")); Data.Num() > 0)
				{
					for (const TSharedPtr<FJsonValue>& JsonValue : Data)
					{
						FPlayerKickDataObject Kick = FPlayerKickDataObject::FromJson(JsonValue->AsObject().ToSharedRef());
						Kicks.Add(Kick);
					}
					OnComplete.ExecuteIfBound(Kicks, true, TOptional<FText>());
				}
				else
				{
					OnComplete.ExecuteIfBound(Kicks, false, FText::FromString("UModerationService::KickPlayers: Call succeeded but no elements in data."));
				}
			}
			else
			{
				UMessageRouterSubsystem::Get().RouteHTTPResult(Res, __FUNCTION__);
				const FString MessageResultCode = UNodecraftUtility::ParseMessageResultCode(Res);
				const FText ErrorMsg = UNodecraftUtility::ParseMessage(Res, __FUNCTION__);
				// UMessageRouterSubsystem::Get().RouteMessage(MessageResultCode, ErrorMsg, EAlertType::Error);
				OnComplete.ExecuteIfBound(Kicks, false, ErrorMsg);
			}
			UMessageRouterSubsystem::Get().RouteHTTPResult(Res, __FUNCTION__);
		}
		else
		{
			UMessageRouterSubsystem::Get().RouteFailureToConnect(__FUNCTION__);
			OnComplete.ExecuteIfBound(Kicks, false, FText::FromString("UModerationService::KickPlayers: Failed to connect to server"));
		}
	});
	return UNodecraftStudioApi::KickPlayers(this, PlayerIds, ServerId, Reason->GetId(), ReasonNotes, HttpRequestComplete)->ProcessRequest();
}

bool UModerationService::BanPlayers(TArray<FString> PlayerIds, const FString& ServerId, const TOptional<FDateTime>& DateExpires,
	const UModerationReasonDataObject* ReasonId, const FString& ReasonNotes, FPlayersBannedDelegate OnComplete)
{
	// Create the request
	FHttpRequestCompleteDelegate HttpRequestComplete;
	// Bind the lambda to the delegate
	HttpRequestComplete.BindWeakLambda(this, [OnComplete](FHttpRequestPtr Req, FHttpResponsePtr Res, bool bConnectedSuccessfully)
	{
		TArray<UBanDataObject*> Bans = {};
		if (bConnectedSuccessfully)
		{
			if (EHttpResponseCodes::IsOk(Res.Get()->GetResponseCode()))
			{
				FJsonObjectWrapper ResJson;
				ResJson.JsonObjectFromString(Res.Get()->GetContentAsString());
				if (const TArray<TSharedPtr<FJsonValue>> Data = ResJson.JsonObject->GetArrayField(TEXT("data")); Data.Num() > 0)
				{
					for (const TSharedPtr<FJsonValue>& JsonValue : Data)
					{
						UBanDataObject* Ban = UBanDataObject::FromJson(JsonValue->AsObject().ToSharedRef());
						Bans.Add(Ban);
					}
					OnComplete.ExecuteIfBound(Bans, true, TOptional<FText>());
				}
				else
				{
					OnComplete.ExecuteIfBound(Bans, false, FText::FromString("UModerationService::BanPlayers: Call succeeded but no elements in data."));
				}
			}
			else
			{
				const FText ErrorMsg = UNodecraftUtility::ParseMessage(Res, __FUNCTION__);
				OnComplete.ExecuteIfBound(Bans, false, ErrorMsg);
			}
			UMessageRouterSubsystem::Get().RouteHTTPResult(Res, __FUNCTION__);
		}
		else
		{
			UMessageRouterSubsystem::Get().RouteFailureToConnect(__FUNCTION__);
			OnComplete.ExecuteIfBound(Bans, false, FText::FromString("UModerationService::BanPlayers: Failed to connect to server"));
		}
	});
	return UNodecraftStudioApi::BanPlayers(this, PlayerIds, ServerId,DateExpires, ReasonId->GetId(), ReasonNotes, HttpRequestComplete)->ProcessRequest();
}

bool UModerationService::UnbanPlayers(TArray<FString> BanIds, FPlayersBannedDelegate OnComplete)
{
	// Create the request
	FHttpRequestCompleteDelegate HttpRequestComplete;
	// Bind the lambda to the delegate
	HttpRequestComplete.BindWeakLambda(this, [OnComplete](FHttpRequestPtr Req, FHttpResponsePtr Res, bool bConnectedSuccessfully)
	{
		TArray<UBanDataObject*> Bans = {};
		if (bConnectedSuccessfully)
		{
			if (EHttpResponseCodes::IsOk(Res.Get()->GetResponseCode()))
			{
				FJsonObjectWrapper ResJson;
				ResJson.JsonObjectFromString(Res.Get()->GetContentAsString());
				if (const TArray<TSharedPtr<FJsonValue>> Data = ResJson.JsonObject->GetArrayField(TEXT("data")); Data.Num() > 0)
				{
					for (const TSharedPtr<FJsonValue>& JsonValue : Data)
					{
						UBanDataObject* Ban = UBanDataObject::FromJson(JsonValue->AsObject().ToSharedRef());
						Bans.Add(Ban);
					}
					OnComplete.ExecuteIfBound(Bans, true, TOptional<FText>());
				}
				else
				{
					OnComplete.ExecuteIfBound(Bans, false, FText::FromString("UModerationService::UnbanPlayers: Call succeeded but no elements in data."));
				}
			}
			else
			{
				const FText ErrorMsg = UNodecraftUtility::ParseMessage(Res, __FUNCTION__);
				OnComplete.ExecuteIfBound(Bans, false, ErrorMsg);
			}
			UMessageRouterSubsystem::Get().RouteHTTPResult(Res, __FUNCTION__);
		}
		else
		{
			UMessageRouterSubsystem::Get().RouteFailureToConnect(__FUNCTION__);
			OnComplete.ExecuteIfBound(Bans, false, FText::FromString("UModerationService::UnbanPlayers: Failed to connect to server"));
		}
	});
	return UNodecraftStudioApi::UnbanPlayers(this, BanIds, HttpRequestComplete)->ProcessRequest();
}

bool UModerationService::UpdatePlayerModStatus(UWorld* World, TArray<UPlayerServerDetailsDataObject*> Players, const FString& ServerId,
	const bool bIsModerator, FSimpleServiceResponseDelegate OnComplete)
{
	// Create the request
	FHttpRequestCompleteDelegate HttpRequestComplete;
	// Bind the lambda to the delegate
	HttpRequestComplete.BindWeakLambda(this, [OnComplete, World, bIsModerator, Players](FHttpRequestPtr Req, FHttpResponsePtr Res, bool bConnectedSuccessfully)
	{
		if (bConnectedSuccessfully)
		{
			if (EHttpResponseCodes::IsOk(Res.Get()->GetResponseCode()))
			{
				FJsonObjectWrapper ResJson;
				ResJson.JsonObjectFromString(Res.Get()->GetContentAsString());
				
				UModerationStore* Store = UModerationStore::Get(World);

				for (UPlayerServerDetailsDataObject* Player : Players)
				{
					if (Store)
					{
						bIsModerator ? Store->OnModeratorPromoted(Player) : Store->OnModeratorDemoted(Player);
					}
				}
				OnComplete.ExecuteIfBound(true, TOptional<FText>());
				UMessageRouterSubsystem::Get().RouteHTTPResult(Res, __FUNCTION__);
			}
			else
			{
				UMessageRouterSubsystem::Get().RouteHTTPResult(Res, __FUNCTION__);
				const FText ErrorMsg = UNodecraftUtility::ParseMessage(Res, __FUNCTION__);
				OnComplete.ExecuteIfBound(false, ErrorMsg);
			}
			UMessageRouterSubsystem::Get().RouteHTTPResult(Res, __FUNCTION__);
		}
		else
		{
			UMessageRouterSubsystem::Get().RouteFailureToConnect(__FUNCTION__);
			OnComplete.ExecuteIfBound(false, FText::FromString("UModerationService::UpdatePlayerModStatus: Failed to connect to server"));
		}
	});
	TArray<FString> PlayerIds;
	for (UPlayerServerDetailsDataObject* Player : Players)
	{
		PlayerIds.Add(Player->GetPlayer()->GetId());
	}
	return UNodecraftStudioApi::UpdatePlayerModStatus(this, PlayerIds, ServerId, bIsModerator, HttpRequestComplete)->ProcessRequest();
}
