// Nodecraft, Inc. © 2012-2024, All Rights Reserved.


#include "Services/AllowsService.h"

#include "API/NodecraftStudioApi.h"
#include "Interfaces/IHttpRequest.h"
#include "Models/AllowsDataObject.h"
#include "Subsystems/MessageRouterSubsystem.h"
#include "Stores/AllowsStore.h"
#include "Utility/NodecraftUtility.h"

bool UAllowsService::AcceptAllow(const FString& AllowId, FSimpleServiceResponseDelegate OnComplete)
{
	FHttpRequestCompleteDelegate ReqCallback;
	ReqCallback.BindWeakLambda(this, [this, OnComplete](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully) mutable
	{
		if (bConnectedSuccessfully)
		{
			UMessageRouterSubsystem::Get().RouteHTTPResult(Response, __FUNCTION__);
		}
		else
		{
			UMessageRouterSubsystem::Get().RouteFailureToConnect(__FUNCTION__);
		}
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

	return UNodecraftStudioApi::UpdatePlayerAllowStatus(this, AllowId, EAllowStatus::Accepted, ReqCallback)->ProcessRequest();
}

bool UAllowsService::DeclineAllow(const FString& AllowId, FSimpleServiceResponseDelegate OnComplete)
{
	FHttpRequestCompleteDelegate ReqCallback;
	ReqCallback.BindWeakLambda(this, [this, OnComplete](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully) mutable
	{
		if (bConnectedSuccessfully)
		{
			UMessageRouterSubsystem::Get().RouteHTTPResult(Response, __FUNCTION__);
		}
		else
		{
			UMessageRouterSubsystem::Get().RouteFailureToConnect(__FUNCTION__);
		}
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

	return UNodecraftStudioApi::UpdatePlayerAllowStatus(this, AllowId, EAllowStatus::Declined, ReqCallback)->ProcessRequest();
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
				FText ErrorMsg = UNodecraftUtility::ParseMessage(Response, __FUNCTION__);
				OnComplete.ExecuteIfBound({}, false, ErrorMsg);
			}
			UMessageRouterSubsystem::Get().RouteHTTPResult(Response, __FUNCTION__);
		}
		else
		{
			UMessageRouterSubsystem::Get().RouteFailureToConnect(__FUNCTION__);
			const FText ErrorMsg = FText::FromString("UAllowsService::ListPlayerAllows: could not connect to the server.");
			OnComplete.ExecuteIfBound({}, false, ErrorMsg);
		}
	});

	return UNodecraftStudioApi::ListPlayerAllows(this, ReqCallback)->ProcessRequest();
}

bool UAllowsService::UpdateAllow(const FString& ServerId, const FString& AllowId, const EAllowStatus Status,
	FUpdateAllowResponseDelegate OnComplete)
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
				if (const TSharedPtr<FJsonObject> Data = ResJson.JsonObject->GetObjectField("data"); Data.IsValid())
				{
					UAllowsDataObject* Allow = UAllowsDataObject::FromJson(Data.ToSharedRef());
					UAllowsStore::Get().UpdateAllow(Allow);
					OnComplete.ExecuteIfBound(Allow, true, TOptional<FText>());
				}
				else
				{
					const FText ErrorMsg = FText::FromString("UAllowsService::UpdateAllow: connected to server but Data is invalid.");
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
			const FText ErrorMsg = FText::FromString("UAllowsService::UpdateAllow: could not connect to the server.");
			OnComplete.ExecuteIfBound(nullptr, false, ErrorMsg);
		}
	});
	return UNodecraftStudioApi::UpdateAllowStatus(this, ServerId, AllowId, Status, ReqCallback)->ProcessRequest();
}

bool UAllowsService::ListAllowsByServer(const FString& ServerId, FListPlayerAllowsResponseDelegate OnComplete)
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
					UAllowsStore::Get().SetAllowsForCurrentServer(Allows);
					OnComplete.ExecuteIfBound(Allows, true, TOptional<FText>());
				}
				else
				{
					const FText ErrorMsg = FText::FromString("UAllowsService::ListAllowsByServer: successfully got data, but it had no allows.");
					OnComplete.ExecuteIfBound({}, true, ErrorMsg);
				}
			}
			else
			{
				FText ErrorMsg = UNodecraftUtility::ParseMessage(Response, __FUNCTION__);
				OnComplete.ExecuteIfBound({}, false, ErrorMsg);
			}
			UMessageRouterSubsystem::Get().RouteHTTPResult(Response, __FUNCTION__);
		}
		else
		{
			UMessageRouterSubsystem::Get().RouteFailureToConnect(__FUNCTION__);
			const FText ErrorMsg = FText::FromString("UAllowsService::ListAllowsByServer: could not connect to the server.");
			OnComplete.ExecuteIfBound({}, false, ErrorMsg);
		}
	});
	return UNodecraftStudioApi::ListAllowsByServer(this, ServerId, ReqCallback)->ProcessRequest();
}

bool UAllowsService::CreateAllow(const FString& ServerId, const FString& PlayerId,
                                 FUpdateAllowResponseDelegate OnComplete)
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
				if (const TSharedPtr<FJsonObject> Data = ResJson.JsonObject->GetObjectField("data"); Data.IsValid())
				{
					UAllowsDataObject* Allow = UAllowsDataObject::FromJson(Data.ToSharedRef());
					UAllowsStore::Get().AddAllowToCurrentServer(Allow);
					OnComplete.ExecuteIfBound(Allow, true, TOptional<FText>());
				}
				else
				{
					const FText ErrorMsg = FText::FromString("UAllowsService::CreateAllow: data is invalid!");
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
			const FText ErrorMsg = FText::FromString("UAllowsService::CreateAllow: could not connect to the server.");
			OnComplete.ExecuteIfBound(nullptr, false, ErrorMsg);
		}
	});
	return UNodecraftStudioApi::CreateAllow(this, ServerId, PlayerId, ReqCallback)->ProcessRequest();
}
