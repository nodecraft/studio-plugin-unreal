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

		PlayerAvatarImage_Bottom->LoadPlayerAvatarAsync(Player->GetPlayer());

		PlayerAvatarImage_Bottom->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		PlayerAvatarImage_Middle->SetVisibility(ESlateVisibility::Collapsed);
		PlayerAvatarImage_Top->SetVisibility(ESlateVisibility::Collapsed);
		
		UsernameLabel->SetText(FText::FromString(Player->GetPlayer()->GetUsername()));
		PlatformLabel->SetText(FText::FromString(Player->GetPlayer()->GetIdentTypeRaw()));
		IdLabel->SetText(FText::FromString(Player->GetPlayer()->GetId()));
		FirstJoinedLabel->SetText(FText::AsDateTime(Player->GetPlayer()->GetDateFirstJoined()));

		if (Player->GetPlayer()->IsOnline())
		{
			OnlineSinceLabel->SetVisibility(ESlateVisibility::Visible);
			LastSeenLabel->SetVisibility(ESlateVisibility::Collapsed);
			OnlineSinceLabel->SetText(FText::AsDateTime(Player->GetPlayer()->GetDateStart()));
		}
		else
		{
			OnlineSinceLabel->SetVisibility(ESlateVisibility::Collapsed);
			LastSeenLabel->SetVisibility(ESlateVisibility::Visible);
			LastSeenLabel->SetText(FText::AsDateTime(Player->GetPlayer()->GetDateEnd()));
		}

		if (const UBanDataObject* Ban = Player->GetBan())
		{
			BanDetailsSection->SetVisibility(ESlateVisibility::Visible);
			BannedOnLabel->SetText(FText::AsDateTime(Ban->GetDateCreated()));
			if (Ban->GetDateExpires().GetTicks() > 0)
			{
				BanExpiresLabel->SetText(FText::AsDateTime(Ban->GetDateExpires()));
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
	else if (SelectedPlayers.Num() == 2)
	{
		PlayerAvatarImage_Bottom->LoadPlayerAvatarAsync(SelectedPlayers[0]->GetPlayer());
		PlayerAvatarImage_Middle->LoadPlayerAvatarAsync(SelectedPlayers[1]->GetPlayer());
		
		PlayerAvatarImage_Bottom->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		PlayerAvatarImage_Middle->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		PlayerAvatarImage_Top->SetVisibility(ESlateVisibility::Collapsed);

		AllTextDetails->SetVisibility(ESlateVisibility::Collapsed);
	}
	else if (SelectedPlayers.Num() >= 3)
	{
		PlayerAvatarImage_Bottom->LoadPlayerAvatarAsync(SelectedPlayers[0]->GetPlayer());
		PlayerAvatarImage_Middle->LoadPlayerAvatarAsync(SelectedPlayers[1]->GetPlayer());
		PlayerAvatarImage_Top->LoadPlayerAvatarAsync(SelectedPlayers[2]->GetPlayer());

		PlayerAvatarImage_Bottom->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		PlayerAvatarImage_Middle->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		PlayerAvatarImage_Top->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

		AllTextDetails->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		AllTextDetails->SetVisibility(ESlateVisibility::Collapsed);
	}
}

#undef LOCTEXT_NAMESPACE