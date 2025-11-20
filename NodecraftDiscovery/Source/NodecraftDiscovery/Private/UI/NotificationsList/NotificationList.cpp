// Nodecraft, Inc. © 2012-2024, All Rights Reserved.


#include "UI/NotificationsList/NotificationList.h"

#include "API/NodecraftMessageCodes.h"
#include "Components/NamedSlot.h"
#include "Services/NotificationsService.h"
#include "Subsystems/MessageRouterSubsystem.h"

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
	NotifsListView->OnEntryWidgetGenerated().AddWeakLambda(this, [this](UUserWidget& EntryWidget)
	{
		EntryWidget.SetFocus();
		NotifsListView->OnEntryWidgetGenerated().RemoveAll(this);
	});
	NotifsListView->SetListItems(Notifs);
	NotifsListView->SetVisibility(Notifs.Num() > 0 ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);
	NoNotificationsSlot->SetVisibility(Notifs.Num() > 0 ? ESlateVisibility::Collapsed : ESlateVisibility::SelfHitTestInvisible);
}

void UNotificationList::NativeConstruct()
{
	Super::NativeConstruct();
	AlertMessageWidget->Hide();
	NoNotificationsSlot->SetVisibility(ESlateVisibility::Collapsed);

	const TArray<FString> MessageCodes = {
		ALLOWS_UPDATE_SUCCESS, ALLOWS_UPDATE_STATUS_MISMATCH_FAILURE,ALLOWS_UPDATE_FAILED,
		PLAYER_ACCESS_TOKEN_INVALID_TOKEN_FAILURE,
		INVITES_UPDATE_SUCCESS, INVITES_UPDATE_FAILED, INVITES_UPDATE_STATUS_MISMATCH_FAILURE,
		NOTIFICATIONS_DISMISSED_SUCCESS, NOTIFICATIONS_DISMISSED_MULTI_SUCCESS
	};
	
	FOnRoutedMessageReceived OnMessageReceived;
	OnMessageReceived.BindWeakLambda(this, [this](const FString& MessageCode, const FText& Message, EAlertType AlertType)
	{
		if (AlertType == EAlertType::Error)
		{
			AlertMessageWidget->Show(Message, AlertType);
		}
		else if (AlertType == EAlertType::Success)
		{
			AlertMessageWidget->Hide();
		}
	});
	UMessageRouterSubsystem::Get().AddMessageReceiver("NotificationList", MessageCodes, OnMessageReceived);
}

void UNotificationList::NativeDestruct()
{
	Super::NativeDestruct();
	UMessageRouterSubsystem::Get().RemoveMessageReceiver("NotificationList");
}

#undef LOCTEXT_NAMESPACE
