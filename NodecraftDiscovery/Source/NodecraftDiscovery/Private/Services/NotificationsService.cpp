// Fill out your copyright notice in the Description page of Project Settings.


#include "Services/NotificationsService.h"

#include "API/DiscoveryAPI.h"
#include "Stores/NotificationsStore.h"

bool UNotificationsService::GetNotifications(FGetNotificationsResponseDelegate& OnComplete)
{
	FHttpRequestCompleteDelegate ReqCallback;
	ReqCallback.BindWeakLambda(this, [this, OnComplete](FHttpRequestPtr Req, FHttpResponsePtr Res, bool bConnectedSuccessfully) mutable 
	{
		TArray<UNotificationDataObject*> Notifications;

		if (bConnectedSuccessfully)
		{
			if (EHttpResponseCodes::IsOk(Res.Get()->GetResponseCode()))
			{
				FJsonObjectWrapper ResJson;
				ResJson.JsonObjectFromString(Res.Get()->GetContentAsString());
				if (const TArray<TSharedPtr<FJsonValue>>& Data = ResJson.JsonObject->GetArrayField("data"); Data.Num() > 0)
				{
					for (TSharedPtr<FJsonValue> JsonValue : Data)
					{
						UNotificationDataObject* Notification = UNotificationDataObject::FromJson(JsonValue.Get()->AsObject().ToSharedRef());
						Notification->SetListType(ENotificationListType::Active);
						Notifications.Add(Notification);
					}
					OnComplete.ExecuteIfBound(Notifications, true, FText());
				}
				else
				{
					OnComplete.ExecuteIfBound(Notifications, false, FText::FromString("NotificationsService::GetNotifications: No notifications found in response"));					
				}
			}
			else
			{
				//Pull error from JSON and pass it to the delegate
				FJsonObjectWrapper ResJson;
				ResJson.JsonObjectFromString(Res.Get()->GetContentAsString());
				FString Error = ResJson.JsonObject->GetStringField("error");
				OnComplete.ExecuteIfBound(Notifications, false, FText::FromString(Error));
			}
		}
		else
		{
			OnComplete.ExecuteIfBound(Notifications, false, FText::FromString("NotificationsService::GetNotifications: Failed to connect to server. Please try again."));
		}
	});
	
	return UDiscoveryAPI::GetNotifications(this, ReqCallback)->ProcessRequest();
}

bool UNotificationsService::GetNotificationsHistory(FGetNotificationsResponseDelegate& OnComplete)
{
	FHttpRequestCompleteDelegate ReqCallback;
	ReqCallback.BindWeakLambda(this, [this, OnComplete](FHttpRequestPtr Req, FHttpResponsePtr Res, bool bConnectedSuccessfully) mutable 
	{
		TArray<UNotificationDataObject*> Notifications;

		if (bConnectedSuccessfully)
		{
			if (EHttpResponseCodes::IsOk(Res.Get()->GetResponseCode()))
			{
				FJsonObjectWrapper ResJson;
				ResJson.JsonObjectFromString(Res.Get()->GetContentAsString());
				if (const TArray<TSharedPtr<FJsonValue>>& Data = ResJson.JsonObject->GetArrayField("data"); Data.Num() > 0)
				{
					for (TSharedPtr<FJsonValue> JsonValue : Data)
					{
						UNotificationDataObject* Notification = UNotificationDataObject::FromJson(JsonValue.Get()->AsObject().ToSharedRef());
						Notification->SetListType(ENotificationListType::Historic);
						Notifications.Add(Notification);
					}
					OnComplete.ExecuteIfBound(Notifications, true, FText());
				}
				else
				{
					OnComplete.ExecuteIfBound(Notifications, false, FText::FromString("NotificationsService::GetNotificationsHistory: No notifications found in response"));					
				}
			}
			else
			{
				//Pull error from JSON and pass it to the delegate
				FJsonObjectWrapper ResJson;
				ResJson.JsonObjectFromString(Res.Get()->GetContentAsString());
				const FString Error = ResJson.JsonObject->GetStringField("error");
				OnComplete.ExecuteIfBound(Notifications, false, FText::FromString(Error));
			}
		}
		else
		{
			OnComplete.ExecuteIfBound(Notifications, false, FText::FromString("NotificationsService::GetNotificationsHistory: Failed to connect to server. Please try again."));
		}
	});
	
	return UDiscoveryAPI::GetNotificationsHistory(this, ReqCallback)->ProcessRequest();
}

bool UNotificationsService::DismissNotification(const UWorld* World, const FString& NotificationId)
{
	FSimpleServiceResponseDelegate OnComplete;
	return DismissNotification(World, NotificationId, OnComplete);
}

bool UNotificationsService::DismissNotification(const UWorld* World, const FString& NotificationId, FSimpleServiceResponseDelegate& OnComplete)
{
	FHttpRequestCompleteDelegate ReqCallback;
	ReqCallback.BindWeakLambda(this, [OnComplete, NotificationId, World](FHttpRequestPtr Req, FHttpResponsePtr Res, bool bConnectedSuccessfully) mutable 
	{
		if (bConnectedSuccessfully)
		{
			if (EHttpResponseCodes::IsOk(Res.Get()->GetResponseCode()))
			{
				UNotificationsStore* Store = UNotificationsStore::Get(World);
				TArray<FString> NotifsToClear = TArray({NotificationId});
				Store->ClearNotifications(NotifsToClear);
				OnComplete.ExecuteIfBound(true, FText());
			}
			else
			{
				//Pull error from JSON and pass it to the delegate
				FJsonObjectWrapper ResJson;
				ResJson.JsonObjectFromString(Res.Get()->GetContentAsString());
				const FString Error = ResJson.JsonObject->GetStringField("error");
				OnComplete.ExecuteIfBound(false, FText::FromString(Error));
			}
		}
		else
		{
			OnComplete.ExecuteIfBound(false, FText::FromString("NotificationsService::DismissNotification: Failed to connect to server. Please try again."));
		}
	});
	
	return UDiscoveryAPI::DismissNotification(this, NotificationId, ReqCallback)->ProcessRequest();
}

bool UNotificationsService::DismissAllNotifications(UWorld* World, FSimpleServiceResponseDelegate& OnComplete)
{
	UNotificationsStore* Store = UNotificationsStore::Get(World);
	const TArray<FString>& NotificationIds = UNotificationsStore::Get(World)->GetActiveNotificationIds();

	FHttpRequestCompleteDelegate ReqCallback;
	ReqCallback.BindWeakLambda(this, [this, OnComplete, NotificationIds, Store](FHttpRequestPtr Req, FHttpResponsePtr Res, bool bConnectedSuccessfully) mutable 
	{
		if (bConnectedSuccessfully)
		{
			if (EHttpResponseCodes::IsOk(Res.Get()->GetResponseCode()))
			{
				Store->ClearNotifications(NotificationIds);
				OnComplete.ExecuteIfBound(true, FText());
			}
			else
			{
				//Pull error from JSON and pass it to the delegate
				FJsonObjectWrapper ResJson;
				ResJson.JsonObjectFromString(Res.Get()->GetContentAsString());
				const FString Error = ResJson.JsonObject->GetStringField("error");
				OnComplete.ExecuteIfBound(false, FText::FromString(Error));
			}
		}
		else
		{
			OnComplete.ExecuteIfBound(false, FText::FromString("NotificationsService::DismissAllNotifications: Failed to connect to server. Please try again."));
		}
	});
	
	return UDiscoveryAPI::DismissNotifications(this, NotificationIds, ReqCallback)->ProcessRequest();
}
