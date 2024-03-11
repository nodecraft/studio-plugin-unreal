// Fill out your copyright notice in the Description page of Project Settings.


#include "Services/ModerationService.h"
#include "Interfaces/IHttpResponse.h"

#include "JsonObjectWrapper.h"
#include "API/DiscoveryAPI.h"

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
				if (const TArray<TSharedPtr<FJsonValue>>& Data = ResJson.JsonObject->GetArrayField("data"); Data.Num() > 0)
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
				if (const FString Message = ResJson.JsonObject->GetStringField("message"); Message.IsEmpty() == false)
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
		}
		else
		{
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
				if (const TSharedPtr<FJsonObject>& Data = ResJson.JsonObject->GetObjectField("data"); Data.IsValid())
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
				if (const FString Message = ResJson.JsonObject->GetStringField("message"); Message.IsEmpty() == false)
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
		}
		else
		{
			OnComplete.ExecuteIfBound(PlayerServerDetailsDataObject, false, FText::FromString("UModerationService::CreatePlayersListModerationDelegate: Failed to connect to server"));
		}
	});
}

TSharedRef<IHttpRequest> UModerationService::GetModerators(const FString& ServerId, FGetPlayerServerDetailsList OnComplete)
{
	FHttpRequestCompleteDelegate HttpRequestComplete;
	CreatePlayersListModerationDelegate(OnComplete, HttpRequestComplete);
	return UDiscoveryAPI::GetModerators(this, ServerId, HttpRequestComplete);
}

TSharedRef<IHttpRequest> UModerationService::GetOnlinePlayers(const FString& ServerId, FGetPlayerServerDetailsList OnComplete)
{
	FHttpRequestCompleteDelegate HttpRequestComplete;
	CreatePlayersListModerationDelegate(OnComplete, HttpRequestComplete);
	return UDiscoveryAPI::GetOnlinePlayersForModeration(this, ServerId, HttpRequestComplete);
}

TSharedRef<IHttpRequest> UModerationService::GetOfflinePlayers(const FString& ServerId, FGetPlayerServerDetailsList OnComplete)
{
	FHttpRequestCompleteDelegate HttpRequestComplete;
	CreatePlayersListModerationDelegate(OnComplete, HttpRequestComplete);
	return UDiscoveryAPI::GetOfflinePlayersForModeration(this, ServerId, HttpRequestComplete);
}

TSharedRef<IHttpRequest> UModerationService::GetBannedPlayers(const FString& ServerId, FGetPlayerServerDetailsList OnComplete)
{
	FHttpRequestCompleteDelegate HttpRequestComplete;
	CreatePlayersListModerationDelegate(OnComplete, HttpRequestComplete);
	return UDiscoveryAPI::GetBannedPlayers(this, ServerId, HttpRequestComplete);
}

TSharedRef<IHttpRequest> UModerationService::GetOwner(const FString& ServerId, FGetPlayerServerDetails OnComplete)
{
	FHttpRequestCompleteDelegate HttpRequestComplete;
	CreateServerPlayersDetailDelegate(OnComplete, HttpRequestComplete);
	return UDiscoveryAPI::GetOwner(this, ServerId, HttpRequestComplete);
}
