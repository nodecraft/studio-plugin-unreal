// Nodecraft, Inc. © 2012-2024, All Rights Reserved.


#include "UI/NotificationsList/NotificationsDrawerWidget.h"

#include "Services/NotificationsService.h"
#include "Stores/NotificationsStore.h"

#define LOCTEXT_NAMESPACE "NotificationsDrawerWidget"

DEFINE_LOG_CATEGORY_STATIC(LogNotificationsDrawerWidget, Log, All);

void UNotificationsDrawerWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	// TODO: This should happen every 15 seconds, and should actually happen elsewhere so that we can show a badge in the header
	// ActiveNotificationsList->LoadData(ENotificationListType::Active);

	SwitchNotifsPageButton->OnClicked().Clear();
	SwitchNotifsPageButton->OnClicked().AddWeakLambda(this, [this]()
	{
		if (ActiveNotificationsList->GetVisibility() == ESlateVisibility::Visible)
		{
			ShowNotifsPage(ENotificationListType::Historic);
		}
		else
		{
			ShowNotifsPage(ENotificationListType::Active);
		}
	});

	ShowNotifsPage(ENotificationListType::Active);

	ClearAllButton->OnClicked().Clear();
	ClearAllButton->OnClicked().AddWeakLambda(this, [this]()
	{
		LoadGuard->SetIsLoading(true);
		FSimpleServiceResponseDelegate OnDismissAllComplete = FSimpleServiceResponseDelegate::CreateWeakLambda(this, [this](bool Success, TOptional<FText> Error)
		{
			if (Success)
			{
				ActiveNotificationsList->SetListItems(TArray<UNotificationDataObject*>());
			}
			else
			{
				UE_LOG(LogNotificationsDrawerWidget, Error, TEXT("UNotificationsDrawerWidget::NativeOnInitialized: Failed to dismiss all notifications: %s"), *Error.Get(FText::GetEmpty()).ToString());
			}
			LoadGuard->SetIsLoading(false);
		});
		UNotificationsService::Get().DismissAllNotifications(GetWorld(), OnDismissAllComplete);
	});
}

void UNotificationsDrawerWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// Get Notifications Store and listen for live notifications, then update active notifications list

	if (UNotificationsStore* NotificationsStore = UNotificationsStore::Get(GetWorld()))
	{
		if (LiveNotifsListenerHandle.IsValid() == false)
		{
			LiveNotifsListenerHandle = NotificationsStore->AddLiveNotificationListener(FOnNotificationsUpdated::FDelegate::CreateWeakLambda(this, [this](const TArray<UNotificationDataObject*>& Notifications)
				{
					ActiveNotificationsList->SetListItems(Notifications);
				}));
		}
		else
		{
			UE_LOG(LogNotificationsDrawerWidget, Error, TEXT("UNotificationsDrawerWidget::NativeConstruct: Tried to add live notifications listener, but it was already valid"));
		}
	}
	else
	{
		UE_LOG(LogNotificationsDrawerWidget, Error, TEXT("UNotificationsDrawerWidget::NativeConstruct: Tried to add live notifications listener, but failed to get notifications store"));
	}
	
	
}

void UNotificationsDrawerWidget::NativeDestruct()
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
		UE_LOG(LogNotificationsDrawerWidget, Error, TEXT("UNotificationsDrawerWidget::NativeDestruct: Failed to get notifications store"));
	}

	Super::NativeDestruct();
}

void UNotificationsDrawerWidget::ShowNotifsPage(const ENotificationListType NotifListType)
{
	if (NotifListType == ENotificationListType::Active)
	{
		ClearAllButton->SetVisibility(ESlateVisibility::Visible);
		ActiveNotificationsList->SetVisibility(ESlateVisibility::Visible);
		HistoricNotificationsList->SetVisibility(ESlateVisibility::Collapsed);
		HeaderTextBlock->SetText(LOCTEXT("LiveNotificationsHeader", "Notifications"));
		SwitchNotifsPageButton->SetButtonText(LOCTEXT("SwitchToHistoricNotifsButtonText", "Show Old Notifications"));
	}
	else
	{
		LoadGuard->SetIsLoading(true);
		HistoricNotificationsList->LoadData(ENotificationListType::Historic, FSimpleDelegate::CreateWeakLambda(this, [this]()
		{
			LoadGuard->SetIsLoading(false);
		}));

		ClearAllButton->SetVisibility(ESlateVisibility::Collapsed);
		ActiveNotificationsList->SetVisibility(ESlateVisibility::Collapsed);
		HistoricNotificationsList->SetVisibility(ESlateVisibility::Visible);
		HeaderTextBlock->SetText(LOCTEXT("DismissedNotificationsHeader", "Old Notifications"));
		SwitchNotifsPageButton->SetButtonText(LOCTEXT("SwitchToLiveNotifsButtonText", "Show Live Notifications"));
	}
}

#undef LOCTEXT_NAMESPACE