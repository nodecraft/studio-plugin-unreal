// Nodecraft, Inc. © 2012-2024, All Rights Reserved.


#include "UI/ServerDetails/ModerationActionsSection.h"

#include "Input/CommonUIActionRouterBase.h"
#include "Input/CommonUIInputTypes.h"
#include "Stores/ModerationStore.h"

#define LOCTEXT_NAMESPACE "ModerationActionsSection"

DEFINE_LOG_CATEGORY_STATIC(LogModerationActionsSection, Log, All);

void UModerationActionsSection::ScrollToTop()
{
	OnScrollToTopDelegate.ExecuteIfBound();
}

void UModerationActionsSection::ConfigureForPlayerServerDetails(TArray<UPlayerServerDetailsDataObject*> PlayerServerDetails, EPlayerRole PlayerRole)
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

	// Promote to mod button is enabled if all players are not moderators and the player is the owner
	const bool bPromoteToModIsAvailable = bNoPlayersAreModerators && PlayerRole == EPlayerRole::Owner;
	PromoteToModButton->SetIsEnabled(bPromoteToModIsAvailable);
	PromoteToModButton->SetVisibility(bPromoteToModIsAvailable ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	// Demote mod button is enabled if all players are moderators
	// If players are a mix of moderators and non-moderators, both buttons are hidden
	const bool bDemoteModIsAvailable = bAllPlayersAreModerators && PlayerRole == EPlayerRole::Owner;
	DemoteModButton->SetIsEnabled(bDemoteModIsAvailable);
	DemoteModButton->SetVisibility(bDemoteModIsAvailable ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
}

void UModerationActionsSection::SetIsLoading(const bool bIsLoading)
{
	LoadGuard->SetIsLoading(bIsLoading);
}

void UModerationActionsSection::RefreshActions(EModerationAction Action)
{
	SelectActionBindingHandle.Unregister();
	if (Action == EModerationAction::Undefined)
	{
		return;
	}
	if (UCommonUIActionRouterBase* ActionRouter = UCommonUIActionRouterBase::Get(*this))
	{
		FBindUIActionArgs BindActionArgs = FBindUIActionArgs(SelectionModActionData, true,
			FSimpleDelegate::CreateWeakLambda(this, [this, Action]()
		{
			// Actually do the action
			switch (Action)
			{
			case EModerationAction::Kick:
				KickButton->OnClicked().Broadcast();
				break;
			case EModerationAction::Ban:
				BanButton->OnClicked().Broadcast();
				break;
			case EModerationAction::Unban:
				UnbanButton->OnClicked().Broadcast();
				break;
			case EModerationAction::PromoteToModerator:
				PromoteToModButton->OnClicked().Broadcast();
				break;
			case EModerationAction::DemoteModerator:
				DemoteModButton->OnClicked().Broadcast();
				break;
			default:
				break;
			}
		}));

		switch (Action)
		{
		case EModerationAction::Kick:
			BindActionArgs.OverrideDisplayName = LOCTEXT("KickPlayer", "Kick Player");
			break;
		case EModerationAction::Ban:
			BindActionArgs.OverrideDisplayName = LOCTEXT("BanPlayer", "Ban Player");
			break;
		case EModerationAction::Unban:
			BindActionArgs.OverrideDisplayName = LOCTEXT("UnbanPlayer", "Unban Player");
			break;
		case EModerationAction::PromoteToModerator:
			BindActionArgs.OverrideDisplayName = LOCTEXT("PromoteToMod", "Promote to Moderator");
			break;
		case EModerationAction::DemoteModerator:
			BindActionArgs.OverrideDisplayName = LOCTEXT("DemoteMod", "Demote Moderator");
			break;
		default:
			break;
		}

		SelectActionBindingHandle = ActionRouter->RegisterUIActionBinding(*this, BindActionArgs);
		ActionRouter->OnBoundActionsUpdated().Broadcast();
	}
}

UWidget* UModerationActionsSection::GetInitialFocusTarget()
{
	if (KickButton->GetIsEnabled())
	{
		return KickButton;
	}
	else if (BanButton->IsVisible())
	{
		return BanButton;
	}
	else
	{
		return UnbanButton;
	}
}

void UModerationActionsSection::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	KickButton->OnFocusReceived().AddWeakLambda(this, [this]()
	{
		OnWidgetReceivedFocus.ExecuteIfBound(KickButton);
		RefreshActions(EModerationAction::Kick);
	});

	BanButton->OnFocusReceived().AddWeakLambda(this, [this]()
	{
		OnWidgetReceivedFocus.ExecuteIfBound(BanButton);
		RefreshActions(EModerationAction::Ban);
	});

	UnbanButton->OnFocusReceived().AddWeakLambda(this, [this]()
	{
		OnWidgetReceivedFocus.ExecuteIfBound(UnbanButton);
		RefreshActions(EModerationAction::Unban);
	});

	PromoteToModButton->OnFocusReceived().AddWeakLambda(this, [this]()
	{
		OnWidgetReceivedFocus.ExecuteIfBound(PromoteToModButton);
		RefreshActions(EModerationAction::PromoteToModerator);
	});

	DemoteModButton->OnFocusReceived().AddWeakLambda(this, [this]()
	{
		OnWidgetReceivedFocus.ExecuteIfBound(DemoteModButton);
		RefreshActions(EModerationAction::DemoteModerator);
	});

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

	SetupButtonNavigation();
}

UWidget* UModerationActionsSection::HandleMoveLeftFromNonBanButtons(EUINavigation Direction)
{
	if (ensure(NavToPlayerListDelegate.IsBound()))
	{
		return NavToPlayerListDelegate.Execute();
	}
	else
	{
		return nullptr;
	}
}

UWidget* UModerationActionsSection::HandleMoveLeftFromBanButtons(EUINavigation Direction)
{
	if (KickButton->GetIsEnabled())
	{
		return KickButton;
	}
	else if (ensure(NavToPlayerListDelegate.IsBound()))
	{
		return NavToPlayerListDelegate.Execute();
	}
	else
	{
		return nullptr;
	}
}

void UModerationActionsSection::SetupButtonNavigation()
{
	// Set up nav'ing left from the buttons
	FCustomWidgetNavigationDelegate MoveLeftFromBanAndUnban = FCustomWidgetNavigationDelegate();
	MoveLeftFromBanAndUnban.BindDynamic(this, &UModerationActionsSection::HandleMoveLeftFromBanButtons);
	FCustomWidgetNavigationDelegate MoveLeftFromAllOtherButtons = FCustomWidgetNavigationDelegate();
	MoveLeftFromAllOtherButtons.BindDynamic(this, &UModerationActionsSection::HandleMoveLeftFromNonBanButtons);

	
	BanButton->SetNavigationRuleCustom(EUINavigation::Left, MoveLeftFromBanAndUnban);
	UnbanButton->SetNavigationRuleCustom(EUINavigation::Left, MoveLeftFromBanAndUnban);

	KickButton->SetNavigationRuleCustom(EUINavigation::Left, MoveLeftFromAllOtherButtons);
	PromoteToModButton->SetNavigationRuleCustom(EUINavigation::Left, MoveLeftFromAllOtherButtons);
	DemoteModButton->SetNavigationRuleCustom(EUINavigation::Left, MoveLeftFromAllOtherButtons);

	// Stop if we nav up from top buttons
	KickButton->SetNavigationRuleBase(EUINavigation::Up, EUINavigationRule::Stop);
	BanButton->SetNavigationRuleBase(EUINavigation::Up, EUINavigationRule::Stop);
	UnbanButton->SetNavigationRuleBase(EUINavigation::Up, EUINavigationRule::Stop);

	// Stop if we nav down from bottom buttons
	PromoteToModButton->SetNavigationRuleBase(EUINavigation::Down, EUINavigationRule::Stop);
	DemoteModButton->SetNavigationRuleBase(EUINavigation::Down, EUINavigationRule::Stop);
	
}

#undef LOCTEXT_NAMESPACE
