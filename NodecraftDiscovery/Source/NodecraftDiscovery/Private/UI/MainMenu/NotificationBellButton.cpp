// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MainMenu/NotificationBellButton.h"

#include "Stores/NotificationsStore.h"

DEFINE_LOG_CATEGORY_STATIC(LogNotificationBellButton, Log, All);

void UNotificationBellButton::NativeConstruct()
{
	Super::NativeConstruct();

	if (UNotificationsStore* NotificationsStore = UNotificationsStore::Get(GetWorld()))
	{
		if (LiveNotifsListenerHandle.IsValid() == false)
		{
			LiveNotifsListenerHandle = NotificationsStore->AddLiveNotificationListener(FOnNotificationsUpdated::FDelegate::CreateWeakLambda(this, [this](const TArray<UNotificationDataObject*>& Notifications)
			{
				SetBadgeText(FText::AsNumber(Notifications.Num()));
				SetIsBadged(Notifications.Num() > 0);
			}));
		}
		else
		{
			UE_LOG(LogNotificationBellButton, Error, TEXT("LogNotificationBellButton::NativeConstruct: Tried to add live notifications listener, but it was already valid"));
		}
	}
	else
	{
		UE_LOG(LogNotificationBellButton, Error, TEXT("LogNotificationBellButton::NativeConstruct: Tried to add live notifications listener, but failed to get notifications store"));
	}
}

void UNotificationBellButton::NativeDestruct()
{
	if (UNotificationsStore* Store = UNotificationsStore::Get(GetWorld()))
	{
		if (Store->RemoveLiveNotificationListener(LiveNotifsListenerHandle))
		{
			LiveNotifsListenerHandle.Reset();
		}
	}
	else
	{
		UE_LOG(LogNotificationBellButton, Error, TEXT("LogNotificationBellButton::NativeDestruct: Failed to get notifications store"));
	}

	Super::NativeDestruct();
}
