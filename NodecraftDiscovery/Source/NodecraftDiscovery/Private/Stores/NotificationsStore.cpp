// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#include "Stores/NotificationsStore.h"

#include "API/NodecraftStudioSessionManagerSubsystem.h"
#include "DeveloperSettings/NodecraftStudioApiSettings.h"
#include "Services/NotificationsService.h"
#include "TimerManager.h"

DEFINE_LOG_CATEGORY_STATIC(LogNotificationsStore, Log, All);

void UNotificationsStore::StartPollingForNotifications()
{
	FTimerDelegate TimerDelegate;

	TimerDelegate.BindWeakLambda(this, [this]()
	{
		// If player session is valid, get notifications
		if (UNodecraftStudioSessionManager::Get().IsPlayerSessionValid())
		{
			FGetNotificationsResponseDelegate LiveNotificationsReceivedDelegate = FGetNotificationsResponseDelegate::CreateWeakLambda(this, [this](TArray<UNotificationDataObject*> Notifications, bool Success, TOptional<FText> Error)
			{
				if (Success)
				{
					LiveNotifications = Notifications;
					OnLiveNotificationsUpdated.Broadcast(Notifications);
				}
			});
			
			UNotificationsService::Get().GetNotifications(LiveNotificationsReceivedDelegate);
		}
		else
		{
			// Cancel timer here?
		}
	});

	if (const UWorld* World = GetWorld())
	{
		if (const UNodecraftStudioApiSettings* NodecraftStudioApiSettings = GetDefault<UNodecraftStudioApiSettings>())
		{
			UE_LOG(LogNotificationsStore, Log, TEXT("Starting to poll for notifications. Polling every %f seconds."), NodecraftStudioApiSettings->GetNotificationPollingInterval());
			World->GetTimerManager().SetTimer(PollingTimerHandle, TimerDelegate, NodecraftStudioApiSettings->GetNotificationPollingInterval(), true, 0.0f);
		}
		else
		{
			UE_LOG(LogNotificationsStore, Error, TEXT("UNotificationsStore::StartPollingForNotifications: Failed to get discovery api settings. Will poll every 15 seconds."));
			World->GetTimerManager().SetTimer(PollingTimerHandle, TimerDelegate, 15.0f, true, 0.0f);
		}
	}
	else
	{
		UE_LOG(LogNotificationsStore, Error, TEXT("UNotificationsStore::StartPollingForNotifications: Failed to get world. Polling will not be started."));
	}
}

void UNotificationsStore::StopPollingForNotifications()
{
	if (const UWorld* World = GetWorld())
	{
		UE_LOG(LogNotificationsStore, Log, TEXT("Stopping polling for notifications."));
		World->GetTimerManager().ClearTimer(PollingTimerHandle);
	}
	else
	{
		UE_LOG(LogNotificationsStore, Error, TEXT("UNotificationsStore::StopPollingForNotifications: Failed to get world. Polling will not be stopped."));
	}
}

void UNotificationsStore::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);
	StartPollingForNotifications();

}

void UNotificationsStore::RemoveLiveNotification(UNotificationDataObject* Notification)
{
	UE_LOG(LogNotificationsStore, Log, TEXT("Removing notification with id: %s"), *Notification->GetId());
	LiveNotifications.Remove(Notification);
	OnLiveNotificationsUpdated.Broadcast(LiveNotifications);
}

FDelegateHandle UNotificationsStore::AddLiveNotificationListener(const FOnNotificationsUpdated::FDelegate& Delegate)
{
	Delegate.ExecuteIfBound(LiveNotifications);
	return OnLiveNotificationsUpdated.Add(Delegate);
}

bool UNotificationsStore::RemoveLiveNotificationListener(const FDelegateHandle& Handle)
{
	return OnLiveNotificationsUpdated.Remove(Handle);
}

void UNotificationsStore::ClearAllNotifications()
{
	UE_LOG(LogNotificationsStore, Log, TEXT("Clearing all notifications."));
	LiveNotifications.Empty();
	OnLiveNotificationsUpdated.Broadcast(LiveNotifications);
}
TArray<FString> UNotificationsStore::GetActiveNotificationIds()
{
	TArray<FString> NotificationIds;
	for (const UNotificationDataObject* Notification : LiveNotifications)
	{
		NotificationIds.Add(Notification->GetId());
	}
	return NotificationIds;
}

void UNotificationsStore::ClearNotifications(const TArray<FString>& Array)
{
	UE_LOG(LogNotificationsStore, Log, TEXT("Clearing notifications with ids: %s"), *FString::Join(Array, TEXT(", ")));
	// Create a copy of all live notifications, then remove the ones that are in the array
	TArray<UNotificationDataObject*> NotificationsCopy = LiveNotifications;
	for (const FString& NotificationId : Array)
	{
		for (UNotificationDataObject* Notification : NotificationsCopy)
		{
			if (Notification->GetId() == NotificationId)
			{
				LiveNotifications.Remove(Notification);
			}
		}
	}
	// Broadcast the updated list
	OnLiveNotificationsUpdated.Broadcast(LiveNotifications);
}

