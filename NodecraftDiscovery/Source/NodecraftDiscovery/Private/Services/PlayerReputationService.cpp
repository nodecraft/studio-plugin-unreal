// Fill out your copyright notice in the Description page of Project Settings.


#include "Services/PlayerReputationService.h"

#include "JsonObjectWrapper.h"
#include "API/DiscoveryAPI.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "Models/AccountReputationEntryDataObject.h"
#include "Utility/NodecraftUtility.h"

bool UPlayerReputationService::GetPlayerReputation(FGetPlayerReputationDelegate& OnComplete)
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
				if (const TArray<TSharedPtr<FJsonValue>> Data = ResJson.JsonObject->GetArrayField("data"); Data.Num() > 0)
				{
					TArray<UAccountReputationEntryDataObject*> ReputationEntries;
					for (const TSharedPtr<FJsonValue> JsonValue : Data)
					{
						UAccountReputationEntryDataObject* ReputationEntry = UAccountReputationEntryDataObject::FromJson(JsonValue->AsObject().ToSharedRef());
						ReputationEntries.Add(ReputationEntry);
					}
					OnComplete.ExecuteIfBound(ReputationEntries, true, TOptional<FText>());
				}
				else
				{
					// successfully received data but there are no bans
					OnComplete.ExecuteIfBound({}, true, TOptional<FText>());
				}
			}
			else
			{
				const FText ErrorMsg = UNodecraftUtility::ParseError(Res, __FUNCTION__);
				OnComplete.ExecuteIfBound({}, false, ErrorMsg);
			}
		}
		else
		{
			OnComplete.ExecuteIfBound({}, false, FText::FromString("UPlayerReputationService::GetPlayerReputation: Failed to connect to server"));
		}
	});

	return UDiscoveryAPI::GetPlayerReputation(this, ReqCallback)->ProcessRequest();
}
