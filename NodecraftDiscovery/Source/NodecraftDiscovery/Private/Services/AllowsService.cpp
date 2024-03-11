// Fill out your copyright notice in the Description page of Project Settings.


#include "Services/AllowsService.h"

#include "API/DiscoveryAPI.h"
#include "Interfaces/IHttpRequest.h"
#include "Models/AllowsDataObject.h"
#include "Utility/NodecraftUtility.h"

bool UAllowsService::AcceptAllow(const FString& AllowId, FSimpleServiceResponseDelegate OnComplete)
{
	FHttpRequestCompleteDelegate ReqCallback;
	ReqCallback.BindWeakLambda(this, [this, OnComplete](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully) mutable
	{
		if (bConnectedSuccessfully && EHttpResponseCodes::IsOk(Response.Get()->GetResponseCode()))
		{
			// Dismiss the notification
			OnComplete.ExecuteIfBound(true, FText());
		}
		else
		{
			volatile auto Res = Response.Get();
			volatile auto Code = Response.Get()->GetResponseCode();
			OnComplete.ExecuteIfBound(false, FText::FromString(Response->GetContentAsString()));
		}
	});

	return UDiscoveryAPI::UpdateAllowStatus(this, AllowId, EAllowStatus::Accepted, ReqCallback)->ProcessRequest();
}

bool UAllowsService::DeclineAllow(const FString& AllowId, FSimpleServiceResponseDelegate OnComplete)
{
	FHttpRequestCompleteDelegate ReqCallback;
	ReqCallback.BindWeakLambda(this, [this, OnComplete](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully) mutable
	{
		if (bConnectedSuccessfully && EHttpResponseCodes::IsOk(Response.Get()->GetResponseCode()))
		{
			// Dismiss the notification
			OnComplete.ExecuteIfBound(true, FText());
		}
		else
		{
			OnComplete.ExecuteIfBound(false, FText::FromString(Response->GetContentAsString()));
		}
	});

	return UDiscoveryAPI::UpdateAllowStatus(this, AllowId, EAllowStatus::Declined, ReqCallback)->ProcessRequest();
}

bool UAllowsService::ListPlayerAllows(FListPlayerAllowsResponseDelegate OnComplete)
{
	FHttpRequestCompleteDelegate ReqCallback;
	ReqCallback.BindWeakLambda(this, [this, OnComplete](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
	{
		if (bConnectedSuccessfully)
		{
			if (EHttpResponseCodes::IsOk(Response.Get()->GetResponseCode()))
			{
				FJsonObjectWrapper ResJson;
				ResJson.JsonObjectFromString(Response.Get()->GetContentAsString());
				if (const TArray<TSharedPtr<FJsonValue>> Data = ResJson.JsonObject->GetArrayField("data"); Data.Num() > 0)
				{
					TArray<UAllowsDataObject*> Allows;
					for (const TSharedPtr<FJsonValue> JsonValue : Data)
					{
						UAllowsDataObject* Allow = UAllowsDataObject::FromJson(JsonValue->AsObject().ToSharedRef());
						Allows.Add(Allow);
					}
					OnComplete.ExecuteIfBound(Allows, true, TOptional<FText>());
				}
				else
				{
					const FText ErrorMsg = FText::FromString("UAllowsService::ListPlayerAllows: successfully got data, but it had no allows.");
					OnComplete.ExecuteIfBound({}, true, ErrorMsg);
				}
			}
			else
			{
				FText ErrorMsg = UNodecraftUtility::ParseError(Response, __FUNCTION__);
				OnComplete.ExecuteIfBound({}, false, ErrorMsg);
			}
		}
		else
		{
			const FText ErrorMsg = FText::FromString("UAllowsService::ListPlayerAllows: could not connect to the server.");
			OnComplete.ExecuteIfBound({}, true, ErrorMsg);
		}
	});

	return UDiscoveryAPI::ListPlayerAllows(this, ReqCallback)->ProcessRequest();
}
