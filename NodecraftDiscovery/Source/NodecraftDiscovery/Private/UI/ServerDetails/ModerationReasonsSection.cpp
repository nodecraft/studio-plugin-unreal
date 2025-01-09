// Nodecraft, Inc. © 2012-2024, All Rights Reserved.


#include "UI/ServerDetails/ModerationReasonsSection.h"

#include "Services/ModerationService.h"
#include "Stores/ModerationStore.h"
#include "UI/Foundation/NodecraftButtonBase.h"
#include "UI/ServerDetails/ModerationReasonButton.h"
#include "UI/ServerDetails/ViewModels/ModerationReasonTileViewModel.h"

#define LOCTEXT_NAMESPACE "ModerationReasonsSection"

void UModerationReasonsSection::SetSelectedAction(const EModerationAction InSelectedReason)
{
	SelectedReason = InSelectedReason;
	switch (InSelectedReason)
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

	if (InSelectedReason == EModerationAction::Ban)
	{
		BanInfoContainer->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		BanInfoContainer->SetVisibility(ESlateVisibility::Collapsed);
	}
	
}

UWidget* UModerationReasonsSection::HandleTileNavUpFromFirstRow(EUINavigation Direction)
{
	OnScrollToTopDelegate.ExecuteIfBound();
	if (SelectedReason == EModerationAction::Ban)
	{
		return BanDurationComboBox;
	}
	else
	{
		return nullptr;
	}
}

UWidget* UModerationReasonsSection::HandleTileNavLeftFromLeftColumn(EUINavigation Direction)
{
	if (NavToPlayerListDelegate.IsBound())
	{
		return NavToPlayerListDelegate.Execute();
	}
	else
	{
		return nullptr;
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

UWidget* UModerationReasonsSection::HandleTileNavDownFromLastRow(EUINavigation Direction)
{
	return CancelButton;
}

void UModerationReasonsSection::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	ReasonButtonsTileView->OnEntryWidgetGenerated().AddWeakLambda(this, [this](UUserWidget& Widget)
	{
		// Set focus to the first item generated and then remove the delegate
		Widget.SetFocus();
		ReasonButtonsTileView->OnEntryWidgetGenerated().RemoveAll(this);
	});

	ReasonButtonsTileView->BottomRowDownNavigationDelegate.BindDynamic(
		this, &UModerationReasonsSection::HandleTileNavDownFromLastRow);

	ReasonButtonsTileView->LeftColumnNavLeftDelegate.BindDynamic(this, &UModerationReasonsSection::HandleTileNavLeftFromLeftColumn);

	ReasonButtonsTileView->TopRowUpNavigationDelegate.BindDynamic(this, &UModerationReasonsSection::HandleTileNavUpFromFirstRow);

	ReasonButtonsTileView->OnItemSelectionChanged().AddWeakLambda(this, [this](UObject* SelectedItem)
	{
		if (UWidget* Widget = ReasonButtonsTileView->GetEntryWidgetFromItem(SelectedItem))
		{
			OnWidgetReceivedFocus.ExecuteIfBound(Widget);
		}
	});

	CancelButton->OnFocusReceived().AddWeakLambda(this, [this]()
	{
		OnWidgetReceivedFocus.ExecuteIfBound(CancelButton);
	});

	ConfirmActionButton->OnFocusReceived().AddWeakLambda(this, [this]()
	{
		OnWidgetReceivedFocus.ExecuteIfBound(ConfirmActionButton);
	});

	CancelButton->OnClicked().AddWeakLambda(this, [this]()
	{
		OnCancelDelegate.ExecuteIfBound();
	});

	ConfirmActionButton->OnClicked().AddWeakLambda(this, [this]()
	{
		OnConfirmActionDelegate.ExecuteIfBound();
	});

	ReasonButtonsTileView->OnItemClicked().AddWeakLambda(this, [this](UObject* ReasonObj)
	{
		if (UModerationReasonTileViewModel* ClickedViewModel = Cast<UModerationReasonTileViewModel>(ReasonObj))
		{
			OnReasonSelectedDelegate.ExecuteIfBound(ClickedViewModel->GetReason());

			// Whenever we click an item, loop through the widgets and set their style
			for (UObject* Reason : ReasonButtonsTileView->GetListItems())
			{
				if (UWidget* EntryWidget = ReasonButtonsTileView->GetEntryWidgetFromItem(Reason))
				{
					UModerationReasonButton* ReasonButton = Cast<UModerationReasonButton>(EntryWidget);
					if (ReasonButton)
					{
						ReasonButton->SetUseSelectedStyle(Reason == ClickedViewModel);
					}
				}
			}
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
						TArray<UModerationReasonTileViewModel*> ReasonTileViewModels;
						TArray<UModerationReasonDataObject*> Reasons = Store->GetModerationReasons();
						for (int i = 0; i < Reasons.Num(); ++i)
						{
							UModerationReasonTileViewModel* ViewModel = NewObject<UModerationReasonTileViewModel>(this);
							ViewModel->SetReason(Reasons[i]);
							ViewModel->SetDisplayIndex(i);
							volatile bool IsBottom = i == Reasons.Num() - 1 || i == Reasons.Num() - 2;
							ViewModel->SetIsBottomRow(IsBottom);
							ReasonTileViewModels.Add(ViewModel);
						}
						ReasonButtonsTileView->SetListItems(ReasonTileViewModels);
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
	}));
}

void UModerationReasonsSection::NativeConstruct()
{
	Super::NativeConstruct();

	// The following code cannot be in NativeOnInitialized because the combobox is not yet created,
	// which causes default values not to be set correctly and the event not to be fired.
	
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

void UModerationReasonsSection::NativeDestruct()
{
	BanDurationComboBox->OnSelectionChanged.RemoveAll(this);
	BanDurationComboBox->OnSelectionChanged.Clear();
	PrivateNotesBlock->OnTextChanged.RemoveAll(this);
	
	Super::NativeDestruct();
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

UWidget* UModerationReasonsSection::GetFirstReasonWidget()
{
	return ReasonButtonsTileView->GetEntryWidgetFromItem(ReasonButtonsTileView->GetItemAt(0));
}

#undef LOCTEXT_NAMESPACE
