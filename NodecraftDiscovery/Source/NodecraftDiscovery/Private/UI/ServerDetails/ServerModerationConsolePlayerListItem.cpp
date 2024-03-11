// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ServerDetails/ServerModerationConsolePlayerListItem.h"

#include "API/DiscoverySessionManagerSubsystem.h"
#include "Models/PlayerServerDetails.h"
#include "Subsystems/AssetStreamerSubsystem.h"
#include "Utility/NodecraftUtility.h"

#define LOCTEXT_NAMESPACE "ServerModerationConsolePlayerListItem"

void UServerModerationConsolePlayerListItem::OnCheckboxStateChanged(bool bIsChecked)
{
	ViewModel->OnPlayerSelectionChanged.ExecuteIfBound(ViewModel->GetPlayerServerDetails(), bIsChecked);
}

void UServerModerationConsolePlayerListItem::StyleForCheckedStatus(bool bIsChecked)
{
	const FPlayerListItemSelectionStyle Style = bIsChecked ? SelectedStyle : UnselectedStyle;
	Checkbox->SetCheckedState(bIsChecked ? ECheckBoxState::Checked : ECheckBoxState::Unchecked);
	Hairline->SetColorAndOpacity(Style.HairlineColor);
	Border->SetBrushColor(Style.BackgroundColor);
}

void UServerModerationConsolePlayerListItem::StyleOnlineStatus(EPlayerServerStatus Status)
{
	const FPlayerListItemStatusStyle Style = *StatusStyles.Find(Status);
	OnlineStatusTextBlock->SetColorAndOpacity(FSlateColor(Style.StatusTextColor));
	OnlineStatusDateTextBlock->SetColorAndOpacity(FSlateColor(Style.DateTextColor));
	OnlineIndicator->SetBrushTintColor(FSlateColor(Style.OnlineIndicatorColor));
	OnlineStatusDateTextBlock->SetVisibility(Status == EPlayerServerStatus::Offline ? ESlateVisibility::Collapsed : ESlateVisibility::Visible);
	// Async load the texture
	UAssetStreamerSubsystem::Get().LoadAssetAsync(Style.OnlineIndicatorTexture, FStreamableDelegate::CreateWeakLambda(this, [this, Style]()
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
			PlayerNameTextBlock->SetText(FText::FromString(Player->GetUsername()));
			PlayerIdTextBlock->SetText(FText::FromString(Player->GetIdent()));
			PlatformIcon->SetIdentity(Player->GetIdentType());

			const bool bIsYou = UDiscoverySessionManager::Get().GetPlayerId() == Player->GetId();
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
				BanReasonTextBlock->SetText(FText::Format(LOCTEXT("BanReason_Reason", "Reason: {0}"),
							Ban->GetReason()));
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
				FirstJoinedTextBlock->SetText(LOCTEXT("HasNeverJoinedServer", "never"));
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
			SelectButton->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			Checkbox->SetVisibility(ESlateVisibility::Hidden);
			SelectButton->SetVisibility(ESlateVisibility::Hidden);
		}
	}

	Checkbox->OnCheckStateChanged.AddDynamic(this, &UServerModerationConsolePlayerListItem::OnCheckboxStateChanged);

	SelectButton->OnClicked().AddWeakLambda(this, [this]()
	{
		ViewModel->OnPlayerSelectedExclusive.ExecuteIfBound(ViewModel->GetPlayerServerDetails());
	});
}

void UServerModerationConsolePlayerListItem::ClearSelection()
{
	Checkbox->OnCheckStateChanged.Broadcast(false);
	StyleForCheckedStatus(false);
}

void UServerModerationConsolePlayerListItem::Select()
{
	StyleForCheckedStatus(true);
	Checkbox->OnCheckStateChanged.Broadcast(true);
}

void UServerModerationConsolePlayerListItem::SetSelected(const bool bSelected)
{
	StyleForCheckedStatus(bSelected);
}

#undef LOCTEXT_NAMESPACE