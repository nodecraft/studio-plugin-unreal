// Nodecraft, Inc. © 2012-2024, All Rights Reserved.


#include "UI/ServerDetails/ModerationConsolePlayerDetailsPanel.h"

#include "Models/PlayerServerDetails.h"
#include "Services/ModerationService.h"

#define LOCTEXT_NAMESPACE "ModerationConsolePlayerDetailsPanel"

void UModerationConsolePlayerDetailsPanel::SetSelectedPlayers(const TArray<UPlayerServerDetailsDataObject*> SelectedPlayers)
{
	ViewModel->SelectedPlayers = SelectedPlayers;
	ViewModel->SelectedAction = EModerationAction::Undefined;
	ViewModel->SelectedReason = nullptr;
	RefreshUI();
}

void UModerationConsolePlayerDetailsPanel::SetServerId(const FString& InServerId)
{
	ServerId = InServerId;
}

void UModerationConsolePlayerDetailsPanel::RefreshUI()
{
	NoPlayerSelectedContent->SetVisibility(ViewModel->SelectedPlayers.Num() == 0 ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	PlayerSelectedContent->SetVisibility(ViewModel->SelectedPlayers.Num() > 0 ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);

	ModerationReasonsSection->SetIsLoading(ViewModel->bIsLoading);
	ModerationActionsSection->SetIsLoading(ViewModel->bIsLoading);

	PlayerDetails->ConfigureForSelectedPlayers(ViewModel->SelectedPlayers);

	if (ViewModel->SelectedPlayers.Num() >= 1)
	{
		if (ViewModel->SelectedPlayers.Num() == 1)
		{
			UsernameLabel->SetText(FText::FromString(ViewModel->SelectedPlayers[0]->GetPlayer()->GetUsername()));
			PlatformIcon->SetIdentity(ViewModel->SelectedPlayers[0]->GetPlayer()->GetIdentType());
		}
		else if (ViewModel->SelectedPlayers.Num() > 1)
		{
			UsernameLabel->SetText(FText::Format(LOCTEXT("NumPlayersSelected", "{0} Players Selected"), ViewModel->SelectedPlayers.Num()));
			PlatformIcon->SetIdentity(EIdentityType::MULTIPLE);
		}

		// If the selected action is Kick or Ban, show the reasons section
		// Otherwise, show the actions section
		if (ViewModel->SelectedAction == EModerationAction::Kick || ViewModel->SelectedAction == EModerationAction::Ban)
		{
			ModerationReasonsSection->SetVisibility(ESlateVisibility::Visible);
			ModerationReasonsSection->SetSelectedAction(ViewModel->SelectedAction);
			// Need a reason for confirm button to be active
			// Also, if we are banning, we need private notes
			const bool bPrivateNotesRequirementMet = ViewModel->SelectedAction != EModerationAction::Ban || ViewModel->PrivateNotes.IsEmpty() == false;
			ModerationReasonsSection->SetConfirmButtonEnabled(ViewModel->SelectedReason != nullptr && bPrivateNotesRequirementMet);
			ModerationActionsSection->SetVisibility(ESlateVisibility::Collapsed);
		}
		else
		{
			ModerationReasonsSection->SetVisibility(ESlateVisibility::Collapsed);
			ModerationActionsSection->SetVisibility(ESlateVisibility::Visible);
			ModerationActionsSection->ConfigureForPlayerServerDetails(ViewModel->SelectedPlayers);
		}
	}
	else
	{
		ModerationActionsSection->SetVisibility(ESlateVisibility::Collapsed);
		ModerationReasonsSection->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UModerationConsolePlayerDetailsPanel::PerformKickAction()
{
	ViewModel->bIsLoading = true;
	TArray<FString> PlayerIds;
	for (UPlayerServerDetailsDataObject* Player : ViewModel->SelectedPlayers)
	{
		PlayerIds.Add(Player->GetPlayer()->GetId());
	}
	UModerationService::Get().KickPlayers(PlayerIds, ServerId, ViewModel->SelectedReason, ViewModel->PrivateNotes.ToString(),
		FPlayersKickedDelegate::CreateWeakLambda(this, [this](TArray<FPlayerKickDataObject>, bool bSuccess, TOptional<FText> Error)
		{
			if (bSuccess)
			{
				ModerationReasonsSection->ClearPrivateNotes();
				OnCompletedModerationActionDelegate.Broadcast();
			}
			ViewModel->bIsLoading = false;
			RefreshUI();
		}));
	RefreshUI();
}

void UModerationConsolePlayerDetailsPanel::PerformBanAction()
{
	ViewModel->bIsLoading = true;
	TArray<FString> PlayerIds;
	for (UPlayerServerDetailsDataObject* Player : ViewModel->SelectedPlayers)
	{
		PlayerIds.Add(Player->GetPlayer()->GetId());
	}
	TOptional<FDateTime> BanExpirationDate;
	if (ViewModel->BanDuration.IsSet())
	{
		BanExpirationDate = FDateTime::UtcNow() + ViewModel->BanDuration.GetValue();
	}
	UModerationService::Get().BanPlayers(PlayerIds, ServerId, BanExpirationDate, ViewModel->SelectedReason, ViewModel->PrivateNotes.ToString(),
		FPlayersBannedDelegate::CreateWeakLambda(this, [this](TArray<UBanDataObject*> Bans, bool bSuccess, TOptional<FText> Error)
		{
			if (bSuccess)
			{
				OnCompletedModerationActionDelegate.Broadcast();
			}
			ViewModel->bIsLoading = false;
			RefreshUI();
		}));
	RefreshUI();
}

void UModerationConsolePlayerDetailsPanel::PerformUnbanAction()
{
	ViewModel->bIsLoading = true;
	TArray<FString> BanIds = {};
	for (const UPlayerServerDetailsDataObject* Player : ViewModel->SelectedPlayers)
	{
		BanIds.Add(Player->GetBan()->GetId());
	}
	UModerationService::Get().UnbanPlayers(BanIds, FPlayersBannedDelegate::CreateWeakLambda(this, [this](TArray<UBanDataObject*> Bans, bool bSuccess, TOptional<FText> Error)
		{
			if (bSuccess)
			{
				OnCompletedModerationActionDelegate.Broadcast();
			}
			ViewModel->bIsLoading = false;
			RefreshUI();
		}));
	RefreshUI();
}

void UModerationConsolePlayerDetailsPanel::PerformUpdateModStatusAction(const EModerationAction Action)
{
	if (Action != EModerationAction::PromoteToModerator && Action != EModerationAction::DemoteModerator)
	{
		return;
	}
	ViewModel->bIsLoading = true;
	bool bIsMod = Action == EModerationAction::PromoteToModerator;
	UModerationService::Get().UpdatePlayerModStatus(GetWorld(), ViewModel->SelectedPlayers, ServerId, bIsMod,
FSimpleServiceResponseDelegate::CreateWeakLambda(this, [this](bool bSuccess, TOptional<FText> Error)
		{
			if (bSuccess)
			{
				OnCompletedModerationActionDelegate.Broadcast();
			}
			ViewModel->bIsLoading = false;
			RefreshUI();
		}));
}

void UModerationConsolePlayerDetailsPanel::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	ViewModel = NewObject<UModerationConsolePlayerDetailsPanelViewModel>();

	OnCompletedModerationActionDelegate.AddWeakLambda(this, [this]()
	{
		ViewModel->PrivateNotes = FText::GetEmpty();
		RefreshUI();
	});

	ModerationReasonsSection->OnReasonSelectedDelegate.BindWeakLambda(ViewModel, [this](UModerationReasonDataObject* Reason)
	{
		ViewModel->SelectedReason = Reason;
		RefreshUI();
	});

	ModerationReasonsSection->OnCancelDelegate.BindWeakLambda(ViewModel, [this]()
	{
		ViewModel->SelectedAction = EModerationAction::Undefined;
		ViewModel->SelectedReason = nullptr;
		ModerationReasonsSection->ClearPrivateNotes();
		ScrollBox->ScrollToStart();
		RefreshUI();
	});

	ModerationReasonsSection->OnConfirmActionDelegate.BindWeakLambda(ViewModel, [this]()
	{
		if (ViewModel->bIsLoading)
		{
			return;
		}
		if (ViewModel->SelectedAction == EModerationAction::Kick)
		{
			PerformKickAction();
		}
		else if (ViewModel->SelectedAction == EModerationAction::Ban)
		{
			PerformBanAction();
		}
	});

	ModerationActionsSection->OnActionSelected.BindWeakLambda(ViewModel, [this](EModerationAction Action)
	{
		// Don't do anything if we're already loading
		if (ViewModel->bIsLoading)
		{
			return;
		}
		ViewModel->SelectedAction = Action;
		// If we are promoting or unpromoting moderators, set isLoading on the viewmodel to true and call the moderation service
		if (Action == EModerationAction::PromoteToModerator || Action == EModerationAction::DemoteModerator)
		{
			PerformUpdateModStatusAction(Action);
		}
		else if (ViewModel->SelectedAction == EModerationAction::Unban)
		{
			PerformUnbanAction();
		}
		RefreshUI();
	});

	ModerationReasonsSection->OnBanDurationChangedDelegate.BindWeakLambda(this, [this](TOptional<FTimespan> BanDuration)
	{
		ViewModel->BanDuration = BanDuration;
	});

	ModerationReasonsSection->OnUserEditedPrivateNotesDelegate.BindWeakLambda(this, [this](const FText& Notes)
	{
		ViewModel->PrivateNotes = Notes;
		RefreshUI();
	});
	
	RefreshUI();
}

#undef LOCTEXT_NAMESPACE