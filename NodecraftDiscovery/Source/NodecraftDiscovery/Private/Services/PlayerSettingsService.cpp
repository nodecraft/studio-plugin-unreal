// Nodecraft, Inc. © 2012-2024, All Rights Reserved.


#include "Services/PlayerSettingsService.h"

#include "JsonObjectWrapper.h"
#include "Api/NodecraftStudioApi.h"
#include "Api/NodecraftStudioSessionManagerSubsystem.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "Models/PlayerSettings.h"
#include "Subsystems/MessageRouterSubsystem.h"
#include "Utility/NodecraftUtility.h"

bool UPlayerSettingsService::UpdateCurrentPlayerRegion(const FString& RegionID,
                                                       FPlayerSessionResponseDelegate& OnComplete)
{
	FHttpRequestCompleteDelegate ReqCallback;

	ReqCallback.BindWeakLambda(this, [this, OnComplete](FHttpRequestPtr Req, FHttpResponsePtr Res, bool bConnectedSuccessfully)
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
					UNodecraftStudioSessionManager::Get().SetPlayerSession(Session);					
					OnComplete.ExecuteIfBound(Session, true, FText::GetEmpty());
				}
				else
				{
					OnComplete.ExecuteIfBound(Session, false, FText::FromString("UIdentService::UpdateCurrentPlayerRegion: Failed to get data from response"));
				}
			}
			else
			{
				const FText ErrorMsg = UNodecraftUtility::ParseMessage(Res, __FUNCTION__);
				OnComplete.ExecuteIfBound(Session, false, ErrorMsg);
			}
			UMessageRouterSubsystem::Get().RouteHTTPResult(Res, __FUNCTION__);
		}
		else
		{
			UMessageRouterSubsystem::Get().RouteFailureToConnect(__FUNCTION__);
			OnComplete.ExecuteIfBound(Session, false, FText::FromString("UIdentService::UpdateCurrentPlayerRegion: Failed to connect to server"));
		}
	});
	return UNodecraftStudioApi::UpdateCurrentPlayerRegion(this, RegionID, ReqCallback)->ProcessRequest();
}

bool UPlayerSettingsService::GetPlayerSettings(FGetPlayerSettingsDelegate& OnComplete)
{
	FHttpRequestCompleteDelegate ReqCallback;
	ReqCallback.BindWeakLambda(this, [this, OnComplete](FHttpRequestPtr Req, FHttpResponsePtr Res, bool bConnectedSuccessfully)
	{
		if (bConnectedSuccessfully)
		{
			if (EHttpResponseCodes::IsOk(Res.Get()->GetResponseCode()))
			{
				FJsonObjectWrapper ResJson;
				ResJson.JsonObjectFromString(Res.Get()->GetContentAsString());
				if (const TSharedPtr<FJsonObject>& Data = ResJson.JsonObject->GetObjectField("data"); Data.IsValid())
				{
					UPlayerSettings* PlayerSettings = UPlayerSettings::FromJson(Data.ToSharedRef());
					OnComplete.ExecuteIfBound(PlayerSettings, true, FText::GetEmpty());
				}
				else
				{
					const FText ErrorText = FText::FromString("UPlayerSettingsService::GetPlayerSettings: Could not retrieve player settings from data object.");
					OnComplete.ExecuteIfBound(nullptr, false, ErrorText);
				}
			}
			else
			{
				const FText ErrorMsg = UNodecraftUtility::ParseMessage(Res, FString(__FUNCTION__));
				OnComplete.ExecuteIfBound(nullptr, false, ErrorMsg);
			}
			UMessageRouterSubsystem::Get().RouteHTTPResult(Res, __FUNCTION__);
		}
		else
		{
			UMessageRouterSubsystem::Get().RouteFailureToConnect(__FUNCTION__);
			const FText ErrorText = FText::FromString("UPlayerSettingsService::GetPlayerSettings: Failed to connect to the server.");
			OnComplete.ExecuteIfBound(nullptr, false, ErrorText);
		}
	});
	return UNodecraftStudioApi::GetPlayerSettings(this, ReqCallback)->ProcessRequest();
}

bool UPlayerSettingsService::UpdateDefaultPlayerRegion(const FString& DefaultRegionID,
	FGetPlayerSettingsDelegate& OnComplete)
{
	FHttpRequestCompleteDelegate ReqCallback;

	ReqCallback.BindWeakLambda(this, [this, OnComplete](FHttpRequestPtr Req, FHttpResponsePtr Res, bool bConnectedSuccessfully)
	{
		if (bConnectedSuccessfully)
		{
			if (EHttpResponseCodes::IsOk(Res.Get()->GetResponseCode()))
			{
				UPlayerSettings* PlayerSettings;
				FJsonObjectWrapper ResJson;
				ResJson.JsonObjectFromString(Res.Get()->GetContentAsString());
				if (const TSharedPtr<FJsonObject>& Data = ResJson.JsonObject->GetObjectField("data"); Data.IsValid())
				{
					PlayerSettings = UPlayerSettings::FromJson(Data.ToSharedRef());
					OnComplete.ExecuteIfBound(PlayerSettings, true, FText::GetEmpty());
				}
				else
				{
					OnComplete.ExecuteIfBound(nullptr, false, FText::FromString("UIdentService::UpdateDefaultPlayerRegion: Failed to get data from response"));
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
			OnComplete.ExecuteIfBound(nullptr, false, FText::FromString("UIdentService::UpdateDefaultPlayerRegion: Failed to connect to server"));
		}
	});
	return UNodecraftStudioApi::UpdatePlayerSettings(this, TOptional<bool>(), DefaultRegionID, ReqCallback)->ProcessRequest();
}

bool UPlayerSettingsService::UpdatePlayerAnalyticsOptOut(bool AnalyticsOptOut, FGetPlayerSettingsDelegate& OnComplete)
{
	FHttpRequestCompleteDelegate ReqCallback;
	ReqCallback.BindWeakLambda(this, [this, OnComplete](FHttpRequestPtr Req, FHttpResponsePtr Res, bool bConnectedSuccessfully)
	{
		if (bConnectedSuccessfully)
		{
			if (EHttpResponseCodes::IsOk(Res.Get()->GetResponseCode()))
			{
				UPlayerSettings* PlayerSettings;
				FJsonObjectWrapper ResJson;
				ResJson.JsonObjectFromString(Res.Get()->GetContentAsString());
				if (const TSharedPtr<FJsonObject>& Data = ResJson.JsonObject->GetObjectField("data"); Data.IsValid())
				{
					PlayerSettings = UPlayerSettings::FromJson(Data.ToSharedRef());
					OnComplete.ExecuteIfBound(PlayerSettings, true, FText::GetEmpty());
				}
				else
				{
					OnComplete.ExecuteIfBound(nullptr, false, FText::FromString("UIdentService::UpdatePlayerAnalyticsOptOut: Failed to get data from response"));
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
			OnComplete.ExecuteIfBound(nullptr, false, FText::FromString("UIdentService::UpdatePlayerAnalyticsOptOut: Failed to connect to server"));
		}
	});

	return UNodecraftStudioApi::UpdatePlayerSettings(this, AnalyticsOptOut, TOptional<FString>(), ReqCallback)->ProcessRequest();
}
