// Nodecraft, Inc. © 2012-2024, All Rights Reserved.


#include "Services/ConsentsService.h"
#include "Json/Public/Dom/JsonValue.h"
#include "Json/Public/Dom/JsonObject.h"
#include "API/NodecraftStudioApi.h"
#include "Models/RulesDataObject.h"
#include "Subsystems/MessageRouterSubsystem.h"
#include "Utility/NodecraftUtility.h"

bool UConsentsService::GetGameLegalConsents(const FString& IdentType, FConsentsResponseDelegate& OnComplete)
{
	FHttpRequestCompleteDelegate ReqCallback;
	ReqCallback.BindWeakLambda(this, [OnComplete](FHttpRequestPtr Req, FHttpResponsePtr Res, bool bConnectedSuccessfully)
	{
		if (bConnectedSuccessfully)
		{
			if (EHttpResponseCodes::IsOk(Res.Get()->GetResponseCode()))
			{
				FJsonObjectWrapper ResJson;
				ResJson.JsonObjectFromString(Res.Get()->GetContentAsString());
				if (const TSharedPtr<FJsonObject>& Data = ResJson.JsonObject->GetObjectField("data"); Data.IsValid())
				{
					FGameConsents GameConsents = FGameConsents();

					const TArray<TSharedPtr<FJsonValue>>* Consents;
					if (Data->TryGetArrayField("consents", Consents))
					{
						FString Content;
						if (Data->TryGetStringField("content", Content))
						{
							GameConsents.Content = Content;
							for (TSharedPtr<FJsonValue> JsonValue : *Consents)
                            {
                            	FConsentPageDetails ConsentPage = FConsentPageDetails::FromJson(JsonValue.Get()->AsObject());
								GameConsents.ConsentPages.Add(ConsentPage);
                            }
						}

						OnComplete.ExecuteIfBound(GameConsents, true, TOptional<FText>());
					}
					else
					{
						// TODO: Do some kind of error handling or default fallback here
					}
				}
			}
			else
			{
				OnComplete.ExecuteIfBound(FGameConsents(), false, FText::FromString("Response wasn't okay I guess"));
			}
			UMessageRouterSubsystem::Get().RouteHTTPResult(Res, __FUNCTION__);
		}
		else
		{
			UMessageRouterSubsystem::Get().RouteFailureToConnect(__FUNCTION__);
			OnComplete.ExecuteIfBound(FGameConsents(), false, FText::FromString("Failed to connect to server"));
		}
	});

	return UNodecraftStudioApi::GetGameLegalConsents(this, IdentType, ReqCallback)->ProcessRequest();
}

bool UConsentsService::SignGameRulesConsents(const FGameConsents& Consents, FSignConsentsResponseDelegate& OnComplete)
{
	FHttpRequestCompleteDelegate ReqCallback;
	ReqCallback.BindWeakLambda(this, [OnComplete](FHttpRequestPtr Req, FHttpResponsePtr Res, bool bConnectedSuccessfully)
	{
		if (bConnectedSuccessfully)
		{
			if (EHttpResponseCodes::IsOk(Res.Get()->GetResponseCode()))
			{
				FJsonObjectWrapper ResJson;
				ResJson.JsonObjectFromString(Res.Get()->GetContentAsString());

				
				if (const TSharedPtr<FJsonObject>& Data = ResJson.JsonObject->GetObjectField("data"); Data.IsValid())
				{
					URulesDataObject* Rules = URulesDataObject::FromJson(Data.ToSharedRef());
					// FGameConsents GameConsents = FGameConsents::FromDataJson(Data);
					OnComplete.ExecuteIfBound(Rules, true, TOptional<FText>());
				}
				else
				{
					OnComplete.ExecuteIfBound(nullptr, false, FText::FromString("UConsentsService::SignGameRulesConsents: Failed to get data from response"));
				}
			}
			else
			{
				OnComplete.ExecuteIfBound(nullptr, false, FText::FromString("Response wasn't okay I guess"));
			}
			UMessageRouterSubsystem::Get().RouteHTTPResult(Res, __FUNCTION__);
		}
		else
		{
			UMessageRouterSubsystem::Get().RouteFailureToConnect(__FUNCTION__);
			OnComplete.ExecuteIfBound(nullptr, false, FText::FromString("Failed to connect to server"));
		}
	});

	return UNodecraftStudioApi::SignConsents(this, Consents, ReqCallback)->ProcessRequest();
}

bool UConsentsService::GetPlayerLegalConsents(FGetPlayerConsentsResponseDelegate& OnComplete)
{
	FHttpRequestCompleteDelegate ReqCallback;
	ReqCallback.BindWeakLambda(this, [OnComplete](FHttpRequestPtr Req, FHttpResponsePtr Res, bool bConnectedSuccessfully)
	{
		if (bConnectedSuccessfully)
		{
			if (EHttpResponseCodes::IsOk(Res.Get()->GetResponseCode()))
			{
				FJsonObjectWrapper ResJson;
				ResJson.JsonObjectFromString(Res.Get()->GetContentAsString());
				if (const TSharedPtr<FJsonObject>& Data = ResJson.JsonObject->GetObjectField("data"); Data.IsValid())
				{
					const FPlayerConsents PlayerConsents = FPlayerConsents::FromJson(Data);
					OnComplete.ExecuteIfBound(PlayerConsents, true, TOptional<FText>());
				}
				else
				{
					OnComplete.ExecuteIfBound(FPlayerConsents(), false, FText::FromString("UConsentsService::GetPlayerLegalConsents: Failed to get data from response"));
				}
			}
			else
			{
				const FText ErrorMessage = UNodecraftUtility::ParseMessage(Res, __FUNCTION__);
				OnComplete.ExecuteIfBound(FPlayerConsents(), false, ErrorMessage);
			}
			UMessageRouterSubsystem::Get().RouteHTTPResult(Res, __FUNCTION__);
		}
		else
		{
			UMessageRouterSubsystem::Get().RouteFailureToConnect(__FUNCTION__);
			OnComplete.ExecuteIfBound(FPlayerConsents(), false, FText::FromString("UConsentsService::GetPlayerLegalConsents: Failed to connect to the server."));
		}
	});

	return UNodecraftStudioApi::GetPlayerLegalConsents(this, ReqCallback)->ProcessRequest();
}

bool UConsentsService::GetPlayerRulesConsents(FGetPlayerConsentsResponseDelegate& OnComplete)
{
	FHttpRequestCompleteDelegate ReqCallback;
	ReqCallback.BindWeakLambda(this, [OnComplete](FHttpRequestPtr Req, FHttpResponsePtr Res, bool bConnectedSuccessfully)
	{
		if (bConnectedSuccessfully)
		{
			if (EHttpResponseCodes::IsOk(Res.Get()->GetResponseCode()))
			{
				FJsonObjectWrapper ResJson;
				ResJson.JsonObjectFromString(Res.Get()->GetContentAsString());
				if (const TSharedPtr<FJsonObject>& Data = ResJson.JsonObject->GetObjectField("data"); Data.IsValid())
				{
					const FPlayerConsents PlayerConsents = FPlayerConsents::FromJson(Data);
					OnComplete.ExecuteIfBound(PlayerConsents, true, TOptional<FText>());
				}
				else
				{
					OnComplete.ExecuteIfBound(FPlayerConsents(), false, FText::FromString("UConsentsService::GetPlayerRulesConsents: Failed to get data from response"));
				}
			}
			else
			{
				const FText ErrorMessage = UNodecraftUtility::ParseMessage(Res, __FUNCTION__);
				OnComplete.ExecuteIfBound(FPlayerConsents(), false, ErrorMessage);
			}
			UMessageRouterSubsystem::Get().RouteHTTPResult(Res, __FUNCTION__);
		}
		else
		{
			UMessageRouterSubsystem::Get().RouteFailureToConnect(__FUNCTION__);
			OnComplete.ExecuteIfBound(FPlayerConsents(), false, FText::FromString("UConsentsService::GetPlayerRulesConsents: Failed to connect to the server."));
		}
	});

	return UNodecraftStudioApi::GetPlayerRulesConsents(this, ReqCallback)->ProcessRequest();
}
