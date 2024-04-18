// Nodecraft, Inc. © 2012-2024, All Rights Reserved.


#include "UI/ServerDetails/SelectedPlayerDetails.h"

#include "DeveloperSettings/NodecraftStudioStyleSettings.h"
#include "Models/PlayerServerDetails.h"

#define LOCTEXT_NAMESPACE "SelectedPlayerDetails"

void USelectedPlayerDetails::ConfigureForSelectedPlayers(const TArray<UPlayerServerDetailsDataObject*> SelectedPlayers)
{
	if (SelectedPlayers.Num() == 1)
	{
		AllTextDetails->SetVisibility(ESlateVisibility::Visible);
		UPlayerServerDetailsDataObject* Player = SelectedPlayers[0];

		PlayerAvatarImage->LoadPlayerAvatarAsync(Player->GetPlayer());
		
		UsernameLabel->SetText(FText::FromString(Player->GetPlayer()->GetUsername()));
		PlatformLabel->SetText(FText::FromString(Player->GetPlayer()->GetIdentTypeRaw()));
		IdLabel->SetText(FText::FromString(Player->GetPlayer()->GetId()));
		FirstJoinedLabel->SetText(FText::FromString(Player->GetPlayer()->GetDateFirstJoined().ToFormattedString(*UNodecraftStudioStyleSettings::GetDateWithTimeFormat())));

		if (Player->GetPlayer()->IsOnline())
		{
			OnlineSinceLabel->SetVisibility(ESlateVisibility::Visible);
			LastSeenLabel->SetVisibility(ESlateVisibility::Collapsed);
			OnlineSinceLabel->SetText(FText::FromString(Player->GetPlayer()->GetDateStart().ToFormattedString(*UNodecraftStudioStyleSettings::GetDateWithTimeFormat())));
		}
		else
		{
			OnlineSinceLabel->SetVisibility(ESlateVisibility::Collapsed);
			LastSeenLabel->SetVisibility(ESlateVisibility::Visible);
			LastSeenLabel->SetText(FText::FromString(Player->GetPlayer()->GetDateEnd().ToFormattedString(*UNodecraftStudioStyleSettings::GetDateWithTimeFormat())));
		}

		if (const UBanDataObject* Ban = Player->GetBan())
		{
			BanDetailsSection->SetVisibility(ESlateVisibility::Visible);
			BannedOnLabel->SetText(FText::FromString(Ban->GetDateCreated().ToFormattedString(*UNodecraftStudioStyleSettings::GetDateWithTimeFormat())));
			if (Ban->GetDateExpires().GetTicks() > 0)
			{
				BanExpiresLabel->SetText(FText::FromString(Ban->GetDateExpires().ToFormattedString(*UNodecraftStudioStyleSettings::GetDateWithTimeFormat())));
			}
			else
			{
				BanExpiresLabel->SetText(LOCTEXT("NeverExpires", "Never"));
			}

			BanCategoryLabel->SetText(Ban->GetPublicReason());
			BanReasonLabel->SetText(Ban->GetReason());
		}
		else
		{
			BanDetailsSection->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
	else
	{
		AllTextDetails->SetVisibility(ESlateVisibility::Collapsed);
		
	}
}

#undef LOCTEXT_NAMESPACE