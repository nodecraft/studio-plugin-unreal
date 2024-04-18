// Nodecraft, Inc. © 2012-2024, All Rights Reserved.


#include "UI/ServerDetails/ModerationReasonsSection.h"

#include "Services/ModerationService.h"
#include "Stores/ModerationStore.h"

#define LOCTEXT_NAMESPACE "ModerationReasonsSection"

void UModerationReasonsSection::SetSelectedAction(const EModerationAction SelectedReason)
{
	switch (SelectedReason)
	{
	case EModerationAction::Ban:
		ConfirmActionButton->SetButtonText(LOCTEXT("confirm_ban", "Confirm Ban"));
		break;
	case EModerationAction::Kick:
		ConfirmActionButton->SetButtonText(LOCTEXT("confirm_kick", "Confirm Kick"));
		break;
	case EModerationAction::Unban:
		ConfirmActionButton->SetButtonText(LOCTEXT("confirm_unban", "Confirm Unban"));
		break;
	case EModerationAction::PromoteToModerator:
		ConfirmActionButton->SetButtonText(LOCTEXT("confirm_promote", "Confirm Promotion"));
		break;
	case EModerationAction::DemoteModerator:
		ConfirmActionButton->SetButtonText(LOCTEXT("confirm_demote", "Confirm Demotion"));
		break;
	default:
		ConfirmActionButton->SetButtonText(LOCTEXT("confirm_action", "Confirm"));
		break;
	}

	if (SelectedReason == EModerationAction::Ban)
	{
		BanInfoContainer->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		BanInfoContainer->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UModerationReasonsSection::OnBanDurationChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	FBanDuration SelectedBanDuration;
	GetSelectedBanDuration(SelectedBanDuration);
	OnBanDurationChangedDelegate.ExecuteIfBound(SelectedBanDuration.GetDuration());
}

void UModerationReasonsSection::OnPrivateNotesChanged(const FText& Text)
{
	OnUserEditedPrivateNotesDelegate.ExecuteIfBound(Text);
}

void UModerationReasonsSection::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	CancelButton->OnClicked().AddWeakLambda(this, [this]()
	{
		OnCancelDelegate.ExecuteIfBound();
	});

	ConfirmActionButton->OnClicked().AddWeakLambda(this, [this]()
	{
		OnConfirmActionDelegate.ExecuteIfBound();
	});

	ReasonButtonsTileView->OnItemSelectionChanged().AddWeakLambda(this, [this](UObject* ReasonObj)
	{
		if (UModerationReasonDataObject* Reason = Cast<UModerationReasonDataObject>(ReasonObj))
		{
				OnReasonSelectedDelegate.ExecuteIfBound(Reason);
		}
	});

	// Load reasons
	if (UWorld* World = GetWorld())
	{
		if (UModerationStore* Store = UModerationStore::Get(World))
		{
			Store->AddModerationReasonsUpdatedListener(FSimpleMulticastDelegate::FDelegate::CreateWeakLambda(this, [this, Store]()
			{
				if (Store)
				{
					if (Store->GetModerationReasons().Num() > 0)
					{
						ReasonButtonsTileView->SetListItems(Store->GetModerationReasons());
					}
				}
			}));
		}
	}

	LoadGuard->SetIsLoading(true);
	UModerationService::Get().GetModerationActionReasons(GetWorld(), FGetModActionReasonsDelegate::CreateWeakLambda(
		this, [this](const TArray<UModerationReasonDataObject*>& Reasons, bool bSuccess, TOptional<FText> Error)
	{
		LoadGuard->SetIsLoading(false);
		if (bSuccess == false)
		{
			// TODO: Show error here
		}
	}));

	// Setup Ban Duration combobox
	BanDurationComboBox->ClearOptions();
	BanDurationComboBox->ClearSelection();
	for (FBanDuration BanDuration : BanDurations)
	{
		BanDurationComboBox->AddOption(BanDuration.DisplayText.ToString());
	}

	BanDurationComboBox->OnSelectionChanged.AddDynamic(this, &UModerationReasonsSection::OnBanDurationChanged);
	
	BanDurationComboBox->SetSelectedIndex(DefaultBanDurationIndex);
	// Make sure event is fired
	BanDurationComboBox->OnSelectionChanged.Broadcast(BanDurationComboBox->GetSelectedOption(), ESelectInfo::Direct);

	PrivateNotesBlock->OnTextChanged.AddDynamic(this, &UModerationReasonsSection::OnPrivateNotesChanged);
}

void UModerationReasonsSection::GetSelectedBanDuration(FBanDuration& OutBanDuration)
{
	const int32 SelectedIndex = BanDurationComboBox->GetSelectedIndex();
	if (SelectedIndex >= 0 && SelectedIndex < BanDurations.Num())
	{
		OutBanDuration = BanDurations[SelectedIndex];
	}
}

void UModerationReasonsSection::SetConfirmButtonEnabled(const bool bActive)
{
	ConfirmActionButton->SetIsEnabled(bActive);
}

void UModerationReasonsSection::ClearPrivateNotes()
{
	PrivateNotesBlock->SetText(FText::GetEmpty());
}

void UModerationReasonsSection::SetIsLoading(const bool bIsLoading)
{
	LoadGuard->SetIsLoading(bIsLoading);
}

#undef LOCTEXT_NAMESPACE
