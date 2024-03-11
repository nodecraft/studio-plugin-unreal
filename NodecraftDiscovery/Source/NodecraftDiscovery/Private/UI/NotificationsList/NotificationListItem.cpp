// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/NotificationsList/NotificationListItem.h"
#include "Models/NotificationDataObject.h"
#include "Services/NotificationsService.h"
#include "Stores/NotificationsStore.h"
#include "Subsystems/AssetStreamerSubsystem.h"

DEFINE_LOG_CATEGORY_STATIC(LogNotificationListItem, Log, All);
#define LOCTEXT_NAMESPACE "NotificationListItem"
#define UNKNOWN_SERVER_TITLE LOCTEXT("UnknownServerTitle", "Unknown")
#define AVATAR_IMAGE_DIMENSIONS 185.0f
#define ICON_IMAGE_DIMENSIONS 80.0f
#define LIST_ITEM_SIZE_SMALL 194.0f
#define LIST_ITEM_SIZE_LARGE 226.0f

void PopulateWithPlayerInfo(UAsyncImage* AvatarImage, UTextBlock* PlayerNameTextBlock, const UPlayerDataObject* PlayerDataObject)
{
	if (PlayerDataObject)
	{
		AvatarImage->LoadPlayerAvatarAsync(PlayerDataObject);
		AvatarImage->SetDesiredSizeOverride(FVector2D(AVATAR_IMAGE_DIMENSIONS));
	}
	PlayerNameTextBlock->SetText(PlayerDataObject ? FText::FromString(PlayerDataObject->GetUsername()) : FText::GetEmpty());
}

void UNotificationListItem::UpdateItemSize(const ENotifListItemSize Size)
{
	switch (Size)
	{
	case Small:
		NotifSizeBox->SetHeightOverride(LIST_ITEM_SIZE_SMALL);
		break;
	case Large:
		NotifSizeBox->SetHeightOverride(LIST_ITEM_SIZE_LARGE);
		break;
	default:
		break;
	}
}

void UNotificationListItem::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	// Reset text and image dimensions. Image Dimensions will get set differently if using avatar image
	EventTextDetailBlock->SetText(FText::GetEmpty());
	NotifImage->SetDesiredSizeOverride(FVector2D(ICON_IMAGE_DIMENSIONS));

	// Hide player info container by default. Set visibility in individual config functions
	PlayerInfoContainer->SetVisibility(ESlateVisibility::Collapsed);
	
	UNotificationDataObject* NotificationDataObject = Cast<UNotificationDataObject>(ListItemObject);

	// Hide/Show buttons depending on list type
	if (NotificationDataObject->GetListType() == ENotificationListType::Active)
	{
		PrimaryActionButton->SetVisibility(ESlateVisibility::Visible);
		DismissActionButton->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		PrimaryActionButton->SetVisibility(ESlateVisibility::Collapsed);
		DismissActionButton->SetVisibility(ESlateVisibility::Collapsed);
	}

	PrimaryActionButton->OnClicked().Clear();
	DismissActionButton->OnClicked().Clear();

	UWorld* World = GetWorld();


	if (NotificationDataObject->GetListType() == ENotificationListType::Active)
	{
		PrimaryActionButton->OnClicked().AddWeakLambda(this, [this, NotificationDataObject, World]() mutable
		{
			SetIsLoading(true);
			FSimpleServiceResponseDelegate Callback = FSimpleServiceResponseDelegate::CreateWeakLambda(this, [this, NotificationDataObject, World](bool Success, TOptional<FText> Error) mutable
			{
				SetIsLoading(false);
				if (Success)
				{
					// Dismiss the notification
					if (World)
					{
						UNotificationsService::Get().DismissNotification(World, NotificationDataObject->GetId());
						if (UNotificationsStore* Store = UNotificationsStore::Get(GetWorld()))
						{
							Store->RemoveLiveNotification(NotificationDataObject);
						}
					}
				}
			});
			NotificationDataObject->PrimaryAction(Callback);
		});
		
		DismissActionButton->OnClicked().AddWeakLambda(this, [this, NotificationDataObject, World]()
		{
			SetIsLoading(true);
			FSimpleServiceResponseDelegate ResponseDelegate = FSimpleServiceResponseDelegate::CreateWeakLambda(this, [this, NotificationDataObject, World](bool Success, TOptional<FText> Error)
			{
				SetIsLoading(false);
				if (Success)
				{
					// Dismiss the notification
					if (World)
					{
						UNotificationsService::Get().DismissNotification(World, NotificationDataObject->GetId());
						if (UNotificationsStore* Store = UNotificationsStore::Get(GetWorld()))
						{
							Store->RemoveLiveNotification(NotificationDataObject);
						}
					}
				}
				else
				{
					UE_LOG(LogNotificationListItem, Warning, TEXT("Failed to dismiss notification: %s"), *Error.Get(FText::GetEmpty()).ToString());
				}
			});
			UE_LOG(LogNotificationListItem, Warning, TEXT("Trying to call Dismiss with NotificationDataObject: %s"), NotificationDataObject ? *NotificationDataObject->GetId() : TEXT("NULL"));
			if (NotificationDataObject)
			{
				NotificationDataObject->DismissAction(ResponseDelegate, World);
			}
		});
	}

	// If this is something the player can accept, set Dismiss button text to "Decline" otherwise set it to "Dismiss"
	const bool bHasAcceptButton = NotificationDataObject->GetListType() == ENotificationListType::Active && (NotificationDataObject->GetType() == ENotificationType::Allow || NotificationDataObject->GetType() == ENotificationType::Invite);
	DismissActionButton->SetButtonText(bHasAcceptButton ? LOCTEXT("DeclineNotification", "Decline") : LOCTEXT("DismissNotification", "Dismiss"));
	PrimaryActionButton->SetVisibility(bHasAcceptButton ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);

	switch (NotificationDataObject->GetType())
	{
	case ENotificationType::Kick:
		ConfigForKickNotif(NotificationDataObject);
		break;
	case ENotificationType::Ban:
		ConfigForBanNotif(NotificationDataObject);
		break;
	case ENotificationType::Allow:
		ConfigForAllowNotif(NotificationDataObject);
		break;
	case ENotificationType::Session:
		ConfigForSessionNotif(NotificationDataObject);
		break;
	case ENotificationType::Invite:
		ConfigForInviteNotif(NotificationDataObject);
		break;
	case ENotificationType::Moderation:
		ConfigForModerationNotif(NotificationDataObject);
		break;
	}

	UpdateTimestamp(NotificationDataObject->GetDateCreated());
	
	FNotificationListItemConfig ListItemConfig = *NotificationTypeConfigs.Find(NotificationDataObject->GetType());

	if (ListItemConfig.PrimaryButtonStyle.IsNull() == false)
	{
		UAssetStreamerSubsystem::Get().LoadAssetAsync(ListItemConfig.PrimaryButtonStyle.ToSoftObjectPath(), FStreamableDelegate::CreateLambda([this, ListItemConfig]
		{
			PrimaryActionButton->SetStyle(ListItemConfig.PrimaryButtonStyle.Get());
		}));
	}

	// Set notif icon if appropriate
	if (ListItemConfig.NotifIcon.IsNull() == false)
	{
		UAssetStreamerSubsystem::Get().LoadAssetAsync(ListItemConfig.NotifIcon, FStreamableDelegate::CreateLambda([this, ListItemConfig]
		{
			NotifImage->SetBrushFromTexture(ListItemConfig.NotifIcon.Get());
		}));
	}

	// Set event icon if appropriate
	EventIcon->SetVisibility(ListItemConfig.EventTextIcon.IsNull() ? ESlateVisibility::Collapsed : ESlateVisibility::Visible);
	if (ListItemConfig.EventTextIcon.IsNull() == false)
	{
		UAssetStreamerSubsystem::Get().LoadAssetAsync(ListItemConfig.EventTextIcon, FStreamableDelegate::CreateLambda([this, ListItemConfig]
		{
			EventIcon->SetBrushFromTexture(ListItemConfig.EventTextIcon.Get());
			EventIcon->SetColorAndOpacity(ListItemConfig.EventTextIconColor);
		}));
	}

	// Set colors
	BorderMaterialInstance->SetVectorParameterValue("Shape color", ListItemConfig.BackgroundColor);
	BorderMaterialInstance->SetVectorParameterValue("Stroke color", ListItemConfig.BorderColor);
	ContentBorder->SetBrushColor(ListItemConfig.ContentAreaBackgroundColor);
	EventTextBlock->SetDefaultColorAndOpacity(FSlateColor(ListItemConfig.EventTextColor));
	
}

void UNotificationListItem::SetIsLoading(const bool bLoading)
{
	LoadGuard->SetIsLoading(bLoading);
}

void UNotificationListItem::NativeConstruct()
{
	Super::NativeConstruct();

	// Create dynamic material instance for border
	BorderMaterialInstance = NotifBorder->GetDynamicMaterial();

	if (BorderMaterialInstance)
	{
		NotifBorder->SetBrushFromMaterial(BorderMaterialInstance);
	}
}

void UNotificationListItem::ConfigForKickNotif(const UNotificationDataObject* NotificationDataObject)
{
	UpdateItemSize(Small);
	EventTextBlock->SetText(
			// TODO: Style server name text
			LOCTEXT("KickedFromServer", "You have been kicked from server:"));
	EventTextDetailBlock->SetText(
			NotificationDataObject->GetServer()
				? NotificationDataObject->GetServer()->GetTitle()
				: UNKNOWN_SERVER_TITLE);
}

void UNotificationListItem::ConfigForBanNotif(const UNotificationDataObject* NotificationDataObject)
{
	UpdateItemSize(Small);
	PrimaryActionButton->SetVisibility(ESlateVisibility::Collapsed);
	PlayerInfoContainer->SetVisibility(ESlateVisibility::Collapsed);
	
	FText BannedText;
	FText DetailText;
	switch (NotificationDataObject->GetBan()->GetScope())
	{
	case EBanScope::Server:
		BannedText = 
			LOCTEXT("BannedFromServer", "You have been banned from server: ");
		DetailText =  NotificationDataObject->GetServer() ? NotificationDataObject->GetServer()->GetTitle() : FText::GetEmpty();
		break;
	case EBanScope::Community:
		BannedText = LOCTEXT("BannedFromCommunity", "You have been banned from all servers for community: ");
		DetailText = NotificationDataObject->GetCommunity()->GetId().IsEmpty() == false ? NotificationDataObject->GetCommunity()->GetTitle() : FText::GetEmpty();
		break;
	case EBanScope::Game:
		BannedText = FText(LOCTEXT("BannedFromGame", "You have been issued a global ban from all multiplayer servers."));
		DetailText = FText::GetEmpty();
		break;
	case EBanScope::Global:
		BannedText = FText(LOCTEXT("BannedGlobally", "You have been banned globally."));
		DetailText = FText::GetEmpty();
		break;
	default:
		// TODO: What to do here?
		break;
	}
	
	EventTextBlock->SetText(BannedText);
	EventTextDetailBlock->SetText(DetailText);
}

void UNotificationListItem::ConfigForAllowNotif(const UNotificationDataObject* NotificationDataObject)
{
	UpdateItemSize(Large);
	PopulateWithPlayerInfo(NotifImage, PlayerNameTextBlock, NotificationDataObject->GetPlayer());
	PlayerInfoContainer->SetVisibility(ESlateVisibility::Visible);
	EventTextBlock->SetText(LOCTEXT("PlayerAllowedAccessToServer", "shared the private server: "));
	EventTextDetailBlock->SetText(NotificationDataObject->GetServer()
			? NotificationDataObject->GetServer()->GetTitle()
			: UNKNOWN_SERVER_TITLE);
	
}

void UNotificationListItem::ConfigForSessionNotif(const UNotificationDataObject* NotificationDataObject)
{
	UpdateItemSize(Large);
	PopulateWithPlayerInfo(NotifImage, PlayerNameTextBlock, NotificationDataObject->GetPlayer());
	PlayerInfoContainer->SetVisibility(ESlateVisibility::Visible);
	EventTextBlock->SetText(LOCTEXT("PlayerJoinedServer", "joined your server: "));
	EventTextDetailBlock->SetText(NotificationDataObject->GetServer()
			? NotificationDataObject->GetServer()->GetTitle()
			: UNKNOWN_SERVER_TITLE);
}

void UNotificationListItem::ConfigForInviteNotif(const UNotificationDataObject* NotificationDataObject)
{
	UpdateItemSize(Large);
	PopulateWithPlayerInfo(NotifImage, PlayerNameTextBlock, NotificationDataObject->GetPlayer());
	PlayerInfoContainer->SetVisibility(ESlateVisibility::Visible);
	EventTextBlock->SetText(LOCTEXT("PlayerInvitedToPlayOnServer", "invited you to play on server: "));
	EventTextDetailBlock->SetText(NotificationDataObject->GetServer()
			? NotificationDataObject->GetServer()->GetTitle()
			: UNKNOWN_SERVER_TITLE);
}

void UNotificationListItem::ConfigForModerationNotif(const UNotificationDataObject* NotificationDataObject)
{
	// TODO: Put something here for now, even though it seems like it won't be used
}

void UNotificationListItem::UpdateTimestamp(const FDateTime& Time)
{
	const FTimespan TimeSinceStartedPlaying = FDateTime::UtcNow() - Time;
	
	if (TimeSinceStartedPlaying.GetMinutes() < 1)
	{
		TimestampTextBlock->SetText(LOCTEXT("LessThanAMinuteAgo", "Less than a minute ago"));
	}
	else if (TimeSinceStartedPlaying.GetHours() < 1)
	{
		TimestampTextBlock->SetText(FText::Format(LOCTEXT("XMinutesAgo", "{0} minutes ago"), TimeSinceStartedPlaying.GetMinutes()));
	}
	else if (TimeSinceStartedPlaying.GetDays() < 1)
	{
		TimestampTextBlock->SetText(FText::Format(LOCTEXT("XHoursAndYMinutesAgo", "{0} hours and {1} minutes ago"),
			TimeSinceStartedPlaying.GetHours(), TimeSinceStartedPlaying.GetMinutes()));
	}
	else
	{
		TimestampTextBlock->SetText(FText::Format(LOCTEXT("XDaysAgo", "{0} days ago"), TimeSinceStartedPlaying.GetDays()));
	}
}

#undef LOCTEXT_NAMESPACE
#undef UNKNOWN_SERVER_TITLE
#undef AVATAR_IMAGE_DIMENSIONS
#undef ICON_IMAGE_DIMENSIONS
#undef LIST_ITEM_SIZE_SMALL
#undef LIST_ITEM_SIZE_LARGE
