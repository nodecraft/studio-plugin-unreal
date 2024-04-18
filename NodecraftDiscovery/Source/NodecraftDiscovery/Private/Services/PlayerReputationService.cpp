// Nodecraft, Inc. © 2012-2024, All Rights Reserved.


#include "Services/PlayerReputationService.h"

#include "JsonObjectWrapper.h"
#include "Api/NodecraftStudioApi.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "Models/AccountReputationEntryDataObject.h"
#include "Subsystems/MessageRouterSubsystem.h"
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
				const FText ErrorMsg = UNodecraftUtility::ParseMessage(Res, __FUNCTION__);
				OnComplete.ExecuteIfBound({}, false, ErrorMsg);
			}
			UMessageRouterSubsystem::Get().RouteHTTPResult(Res, __FUNCTION__);
		}
		else
		{
			UMessageRouterSubsystem::Get().RouteFailureToConnect(__FUNCTION__);
			OnComplete.ExecuteIfBound({}, false, FText::FromString("UPlayerReputationService::GetPlayerReputation: Failed to connect to server"));
		}
	});

	return UNodecraftStudioApi::GetPlayerReputation(this, ReqCallback)->ProcessRequest();
}
