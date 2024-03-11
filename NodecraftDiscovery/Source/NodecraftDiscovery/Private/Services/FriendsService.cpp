﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "Services/FriendsService.h"

#include "API/DiscoveryAPI.h"
#include "Interfaces/IHttpRequest.h"
#include "Models/FriendDataObject.h"

void UFriendsService::CreateFriendsListDelegate(const FGetFriendsListDelegate& OnComplete,
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
					TArray<UFriendDataObject*> Friends;
					for (TSharedPtr<FJsonValue, ESPMode::ThreadSafe> JsonValue : Data)
					{
						UFriendDataObject* FriendDataObject = UFriendDataObject::FromJson(JsonValue.Get()->AsObject().ToSharedRef());
						Friends.Add(FriendDataObject);
					}
					OnComplete.ExecuteIfBound(Friends, true, FText());
				}
				else
				{
					OnComplete.ExecuteIfBound({}, false, FText::FromString("UFriendsService::CreateFriendsListDelegate: Error while grabbing data from response"));
				}
			}
			else
			{
				FJsonObjectWrapper ResJson;
				ResJson.JsonObjectFromString(Res.Get()->GetContentAsString());
				if (const FString Message = ResJson.JsonObject->GetStringField("message"); Message.IsEmpty() == false)
				{
					const FString ErrorText = FString::Printf(TEXT("UFriendsService::CreateFriendsListDelegate: Error code: %d. Message: %ls"), Res.Get()->GetResponseCode(), *Message);
					OnComplete.ExecuteIfBound({}, false, TOptional<FText>(FText::FromString(ErrorText)));
				}
				else
				{
					const FString ErrorText = FString::Printf(TEXT("UFriendsService::CreateFriendsListDelegate: Received %d response w/out error message"), Res.Get()->GetResponseCode());
					OnComplete.ExecuteIfBound({}, false, TOptional<FText>(FText::FromString(ErrorText)));
				}
			}
		}
		else
		{
			OnComplete.ExecuteIfBound({}, false, FText::FromString("UFriendsService::CreateFriendsListDelegate: Failed to connect to server"));
		}
	});
}


bool UFriendsService::GetFriends(FGetFriendsListDelegate& OnComplete)
{
	FHttpRequestCompleteDelegate ReqCallback;
	CreateFriendsListDelegate(OnComplete, ReqCallback);
	return UDiscoveryAPI::ListFriends(this, ReqCallback)->ProcessRequest();
}
