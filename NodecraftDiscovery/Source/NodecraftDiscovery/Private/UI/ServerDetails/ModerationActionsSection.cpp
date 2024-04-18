// Nodecraft, Inc. © 2012-2024, All Rights Reserved.


#include "UI/ServerDetails/ModerationActionsSection.h"

#include "Stores/ModerationStore.h"

DEFINE_LOG_CATEGORY_STATIC(LogModerationActionsSection, Log, All);

void UModerationActionsSection::ConfigureForPlayerServerDetails(TArray<UPlayerServerDetailsDataObject*> PlayerServerDetails)
{
	bool bAllPlayersOnline = true;
	bool bAllPlayersAreBanned = true;
	bool bNoPlayersAreBanned = true;
	bool bAllPlayersAreModerators = true;
	bool bNoPlayersAreModerators = true;
	
	if (UWorld* World = GetWorld())
	{
		UModerationStore* ModerationStore = UModerationStore::Get(World);
		if (ModerationStore)
		{
			for (UPlayerServerDetailsDataObject* PlayerServerDetail : PlayerServerDetails)
			{
				if (PlayerServerDetail->GetPlayer()->IsOnline() == false)
				{
					bAllPlayersOnline = false;
				}
		
				if (PlayerServerDetail->GetBan() == nullptr)
				{
					bAllPlayersAreBanned = false;
				}
				else
				{
					bNoPlayersAreBanned = false;
				}

				if (UModerationStore::Get(World)->IsPlayerModerator(PlayerServerDetail))
				{
					bNoPlayersAreModerators = false;
				}
				else
				{
					bAllPlayersAreModerators = false;
				}
			}
		}
	}
	else
	{
		UE_LOG(LogModerationActionsSection, Error, TEXT("UModerationActionsSection::ConfigureForPlayerServerDetails: Failed to get world"));
	}

	// Kick button is enabled if all players are online
	KickButton->SetIsEnabled(bAllPlayersOnline);
	// Ban button is enabled if all players are not banned (i.e. if the ban is null)
	BanButton->SetIsEnabled(bNoPlayersAreBanned);
	BanButton->SetVisibility(bNoPlayersAreBanned ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	// Unban button is enabled if all players are banned
	// If players are a mix of banned and unbanned, both buttons are hidden
	UnbanButton->SetIsEnabled(bAllPlayersAreBanned);
	UnbanButton->SetVisibility(bAllPlayersAreBanned ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);

	// Promote to mod button is enabled if all players are not moderators
	PromoteToModButton->SetIsEnabled(bNoPlayersAreModerators);
	PromoteToModButton->SetVisibility(bNoPlayersAreModerators ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	// Demote mod button is enabled if all players are moderators
	// If players are a mix of moderators and non-moderators, both buttons are hidden
	DemoteModButton->SetIsEnabled(bAllPlayersAreModerators);
	DemoteModButton->SetVisibility(bAllPlayersAreModerators ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
}

void UModerationActionsSection::SetIsLoading(const bool bIsLoading)
{
	LoadGuard->SetIsLoading(bIsLoading);
}

void UModerationActionsSection::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	KickButton->OnClicked().AddWeakLambda(this, [this]()
	{
		OnActionSelected.ExecuteIfBound(EModerationAction::Kick);
	});

	BanButton->OnClicked().AddWeakLambda(this, [this]()
	{
		OnActionSelected.ExecuteIfBound(EModerationAction::Ban);
	});

	// Implement other actions
	UnbanButton->OnClicked().AddWeakLambda(this, [this]()
	{
		OnActionSelected.ExecuteIfBound(EModerationAction::Unban);
	});

	PromoteToModButton->OnClicked().AddWeakLambda(this, [this]()
	{
		OnActionSelected.ExecuteIfBound(EModerationAction::PromoteToModerator);
	});

	DemoteModButton->OnClicked().AddWeakLambda(this, [this]()
	{
		OnActionSelected.ExecuteIfBound(EModerationAction::DemoteModerator);
	});
}
