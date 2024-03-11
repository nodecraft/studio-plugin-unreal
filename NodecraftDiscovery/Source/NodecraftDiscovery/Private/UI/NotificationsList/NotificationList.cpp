// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/NotificationsList/NotificationList.h"
#include "Services/NotificationsService.h"

#define LOCTEXT_NAMESPACE "NotificationList"

void UNotificationList::LoadData(ENotificationListType NotifListType, FSimpleDelegate Callback)
{
	NotifsListView->ClearListItems();
	AlertMessageWidget->Hide();
	// Get the servers service
	UNotificationsService& NotifsService = UNotificationsService::Get();

	// Use it to request the popular servers
	FGetNotificationsResponseDelegate OnComplete;
	// Call BindLambda on OnComplete, and pass in a lambda that calls PopulateWithServerJson

	OnComplete.BindWeakLambda(this, [this, NotifListType, Callback]
		(TArray<UNotificationDataObject*> Notifications, bool Success, TOptional<FText> Error)
	{
		Callback.ExecuteIfBound();
		if (Success)
		{
			AlertMessageWidget->Hide();
			SetListItems(Notifications);
		}
		else
		{
			AlertMessageWidget->Show(
				LOCTEXT("FailedToLoadNotifications", "Failed to load notifications"), EAlertType::Error);
		}
	});

	switch (NotifListType)
	{
	case ENotificationListType::Active:
		NotifsService.GetNotifications(OnComplete);
		break;
	
	case ENotificationListType::Historic:
		NotifsService.GetNotificationsHistory(OnComplete);
		break;
	
	default:
		break;
	}
}

void UNotificationList::SetListItems(const TArray<UNotificationDataObject*>& Notifs)
{
	NotifsListView->SetListItems(Notifs);
	NotifsListView->SetVisibility(Notifs.Num() > 0 ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	NoNotificationsSlot->SetVisibility(Notifs.Num() > 0 ? ESlateVisibility::Collapsed : ESlateVisibility::Visible);
}

void UNotificationList::NativeConstruct()
{
	Super::NativeConstruct();
	NoNotificationsSlot->SetVisibility(ESlateVisibility::Collapsed);
	AlertMessageWidget->Hide();
}

#undef LOCTEXT_NAMESPACE
