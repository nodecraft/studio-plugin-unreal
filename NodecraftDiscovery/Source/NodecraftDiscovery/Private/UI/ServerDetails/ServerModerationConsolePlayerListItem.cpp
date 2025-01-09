// Nodecraft, Inc. © 2012-2024, All Rights Reserved.


#include "UI/ServerDetails/ServerModerationConsolePlayerListItem.h"

#include "API/NodecraftStudioSessionManagerSubsystem.h"
#include "Models/PlayerServerDetails.h"
#include "Subsystems/AssetStreamerSubsystem.h"
#include "UI/Foundation/NodecraftButtonBase.h"
#include "Utility/NodecraftUtility.h"

#define LOCTEXT_NAMESPACE "ServerModerationConsolePlayerListItem"

void UServerModerationConsolePlayerListItem::NativePreConstruct()
{
	UnselectedStyle.BorderStyle = bUseKeyboardAndMouseStyles ? BorderStyleDefault_MouseAndKeyboard : BorderStyleDefault_Gamepad;
	SelectedStyle.BorderStyle = bUseKeyboardAndMouseStyles ? BorderStyleFocused_MouseAndKeyboard : BorderStyleFocused_Gamepad;
	Super::NativePreConstruct();
}

void UServerModerationConsolePlayerListItem::OnCheckboxStateChanged(bool bIsChecked)
{
	ViewModel->OnPlayerSelectionChanged.ExecuteIfBound(ViewModel->GetPlayerServerDetails(), bIsChecked);
}

void UServerModerationConsolePlayerListItem::StyleForCheckedStatus(bool bIsChecked)
{
	const FPlayerListItemSelectionStyle Style = bIsChecked ? SelectedStyle : UnselectedStyle;
	Checkbox->SetCheckedState(bIsChecked ? ECheckBoxState::Checked : ECheckBoxState::Unchecked);
	Hairline->SetColorAndOpacity(Style.HairlineColor);
	Border->SetStyle(Style.BorderStyle);
}

void UServerModerationConsolePlayerListItem::StyleOnlineStatus(EPlayerServerStatus Status)
{
	const FPlayerListItemStatusStyle Style = *StatusStyles.Find(Status);
	OnlineStatusTextBlock->SetColorAndOpacity(FSlateColor(Style.StatusTextColor));
	OnlineStatusDateTextBlock->SetColorAndOpacity(FSlateColor(Style.DateTextColor));
	OnlineIndicator->SetBrushTintColor(FSlateColor(Style.OnlineIndicatorColor));
	// Async load the texture
	UAssetStreamerSubsystem::Get().LoadAssetAsync(Style.OnlineIndicatorTexture.ToSoftObjectPath(), FStreamableDelegate::CreateWeakLambda(this, [this, Style]()
	{
		OnlineIndicator->SetBrushFromTexture(Style.OnlineIndicatorTexture.Get());
	}));
}

void UServerModerationConsolePlayerListItem::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);

	ViewModel = Cast<UPlayerServerDetailsViewModel>(ListItemObject);
	if (ViewModel)
	{
		if (const UPlayerDataObject* Player = ViewModel->GetPlayer())
		{
			// Begin Player Section
			PlayerProfileImage->LoadPlayerAvatarAsync(Player);
			PlayerNameTextBlock->SetText(FText::FromString(Player->GetUsername()));
			PlayerIdTextBlock->SetText(FText::FromString(Player->GetIdent()));
			PlatformIcon->SetIdentity(Player->GetIdentType());

			const bool bIsYou = UNodecraftStudioSessionManager::Get().GetPlayerId() == Player->GetId();
			YouIndicator->SetVisibility(bIsYou ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
			// End Player Section

			// Begin Status Section
			if (const UBanDataObject* Ban = ViewModel->GetBan())
			{
				StyleOnlineStatus(EPlayerServerStatus::Banned);
				OnlineStatusTextBlock->SetText(LOCTEXT("BannedStatus", "Banned"));
				OnlineStatusDateTextBlock->SetText(UNodecraftUtility::TimeLeft(Ban->GetDateExpires()));
			}
			else if (ViewModel->GetPlayer()->IsOnline())
			{
				StyleOnlineStatus(EPlayerServerStatus::Online);
				OnlineStatusTextBlock->SetText(LOCTEXT("OnlineStatus", "Online"));
				if (Player->GetDateStart().GetTicks() > 0)
				{
					const FTimespan TimeSinceStartedPlaying = FDateTime::UtcNow() - Player->GetDateStart();
					if (TimeSinceStartedPlaying.GetMinutes() < 1)
					{
						OnlineStatusDateTextBlock->SetText(LOCTEXT("JoinedLessThanAMinuteAgo", "Joined less than a minute ago"));
					}
					else if (TimeSinceStartedPlaying.GetHours() < 1)
					{
						OnlineStatusDateTextBlock->SetText(FText::Format(LOCTEXT("JoinedXMinutesAgo", "Joined {0} minutes ago"), TimeSinceStartedPlaying.GetMinutes()));
					}
					else if (TimeSinceStartedPlaying.GetDays() < 1)
					{
						OnlineStatusDateTextBlock->SetText(FText::Format(LOCTEXT("JoinedXHoursAgo", "Joined {0} hours ago"),
							TimeSinceStartedPlaying.GetHours()));
					}
					else
					{
						OnlineStatusDateTextBlock->SetText(FText::Format(LOCTEXT("JoinedXDaysAgo", "Joined {0} days ago"), TimeSinceStartedPlaying.GetDays()));
					}
				}
				else
				{
					OnlineStatusDateTextBlock->SetText(LOCTEXT("HasNeverJoinedServer", "never"));
				}
			}
			else
			{
				StyleOnlineStatus(EPlayerServerStatus::Offline);
				OnlineStatusTextBlock->SetText(LOCTEXT("OfflineStatus", "Offline"));
				if (Player->GetDateEnd().GetTicks() > 0)
				{
					const FTimespan TimeSinceSeenOnServer = FDateTime::UtcNow() - Player->GetDateEnd();
					if (TimeSinceSeenOnServer.GetMinutes() < 1)
					{
						OnlineStatusDateTextBlock->SetText(LOCTEXT("LastSeenLessThanAMinuteAgo", "Seen less than a minute ago"));
					}
					else if (TimeSinceSeenOnServer.GetHours() < 1)
					{
						OnlineStatusDateTextBlock->SetText(FText::Format(LOCTEXT("LastSeenXMinutesAgo", "Seen {0} minutes ago"), TimeSinceSeenOnServer.GetMinutes()));
					}
					else if (TimeSinceSeenOnServer.GetDays() < 1)
					{
						OnlineStatusDateTextBlock->SetText(FText::Format(LOCTEXT("LastSeenXHoursAgo", "Seen {0} hours ago"),
							TimeSinceSeenOnServer.GetHours()));
					}
					else
					{
						OnlineStatusDateTextBlock->SetText(FText::Format(LOCTEXT("LastSeenXDaysAgo", "Seen {0} days ago"), TimeSinceSeenOnServer.GetDays()));
					}
				}
				else
				{
					OnlineStatusDateTextBlock->SetText(LOCTEXT("HasNeverJoinedServer", "never"));
				}
			}
			// End Status Section

			// Begin Ban status section
			if (const UBanDataObject* Ban = ViewModel->GetBan())
			{
				BanReasonSection->SetVisibility(ESlateVisibility::Visible);
				BanReasonTextBlock->SetText(FText::Format(LOCTEXT("BanReason_Reason", "{0}"),
							Ban->GetPublicReason()));
			}
			else
			{
				BanReasonSection->SetVisibility(ESlateVisibility::Hidden);
			}
			// End Ban status section

			// Begin Playtime Section
			if (Player->GetPlaytime() > 0.0f)
			{
				TotalPlaytimeTextBlock->SetText(FText::Format(LOCTEXT("PlaytimeFormat", "{0} hours"), Player->GetPlaytime()));
			}
			else
			{
				TotalPlaytimeTextBlock->SetText(LOCTEXT("HasNeverPlayed", "never"));
				// FirstJoinedTextBlock->SetText(LOCTEXT("HasNeverJoinedServer", "never"));
			}

			if (Player->GetDateFirstJoined().GetTicks() > 0)
			{
				const FTimespan TimeSinceStartedPlaying = FDateTime::UtcNow() - Player->GetDateFirstJoined();
				if (TimeSinceStartedPlaying.GetMinutes() < 1)
				{
					FirstJoinedTextBlock->SetText(LOCTEXT("FirstJoinedLessThanAMinuteAgo", "first joined less than a minute ago"));
				}
				else if (TimeSinceStartedPlaying.GetHours() < 1)
				{
					FirstJoinedTextBlock->SetText(FText::Format(LOCTEXT("FirstJoinedXMinutesAgo", "first joined {0} minutes ago"), TimeSinceStartedPlaying.GetMinutes()));
				}
				else if (TimeSinceStartedPlaying.GetDays() < 1)
				{
					FirstJoinedTextBlock->SetText(FText::Format(LOCTEXT("FirstJoinedXHoursAgo", "first joined {0} hours ago"),
						TimeSinceStartedPlaying.GetHours()));
				}
				else
				{
					FirstJoinedTextBlock->SetText(FText::Format(LOCTEXT("FirstJoinedXDaysAgo", "first joined {0} days ago"), TimeSinceStartedPlaying.GetDays()));
				}
			}
			else
			{
				FirstJoinedTextBlock->SetText(LOCTEXT("HasNeverJoinedServer", "never"));
			}

			// End Playtime Section

			
		}

		if (ViewModel->bIsSelectable)
		{
			Checkbox->SetVisibility(ESlateVisibility::Visible);
			PrimaryActionButton->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			Checkbox->SetVisibility(ESlateVisibility::Hidden);
			PrimaryActionButton->SetVisibility(ESlateVisibility::Hidden);
		}
	}

	Checkbox->OnCheckStateChanged.Clear();
	Checkbox->OnCheckStateChanged.AddDynamic(this, &UServerModerationConsolePlayerListItem::OnCheckboxStateChanged);

	PrimaryActionButton->OnClicked().AddWeakLambda(this, [this]()
	{
		ViewModel->OnPlayerSelectedExclusive.ExecuteIfBound(ViewModel->GetPlayerServerDetails());
	});
}

void UServerModerationConsolePlayerListItem::ClearSelection()
{
	Checkbox->OnCheckStateChanged.Broadcast(false);
	StyleForCheckedStatus(false);
}

void UServerModerationConsolePlayerListItem::Deselect()
{
	StyleForCheckedStatus(false);
	Checkbox->OnCheckStateChanged.Broadcast(false);
}

void UServerModerationConsolePlayerListItem::Select()
{
	StyleForCheckedStatus(true);
	Checkbox->OnCheckStateChanged.Broadcast(true);
}

bool UServerModerationConsolePlayerListItem::IsSelected()
{
	return Checkbox->IsChecked();
}


void UServerModerationConsolePlayerListItem::SetSelected(const bool bSelected)
{
	StyleForCheckedStatus(bSelected);
}

void UServerModerationConsolePlayerListItem::NativeDestruct()
{
	Checkbox->OnCheckStateChanged.RemoveAll(this);
	Super::NativeDestruct();
}

FReply UServerModerationConsolePlayerListItem::NativeOnFocusReceived(const FGeometry& InGeometry,
	const FFocusEvent& InFocusEvent)
{
	ViewModel->OnPlayerItemReceivedFocus.ExecuteIfBound(this);
	return Super::NativeOnFocusReceived(InGeometry, InFocusEvent);
}

FNavigationReply UServerModerationConsolePlayerListItem::NativeOnNavigation(const FGeometry& MyGeometry,
	const FNavigationEvent& InNavigationEvent, const FNavigationReply& InDefaultReply)
{
	if (OnNavDelegate.IsBound())
	{
		return OnNavDelegate.Execute(MyGeometry, InNavigationEvent, InDefaultReply);
	}
	
	return Super::NativeOnNavigation(MyGeometry, InNavigationEvent, InDefaultReply);
}


#undef LOCTEXT_NAMESPACE
