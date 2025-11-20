// Nodecraft, Inc. © 2012-2024, All Rights Reserved.


#include "UI/ServerDetails/ModerationConsolePlayerSelector.h"

#include "CommonInputSubsystem.h"
#include "Utility/NodecraftMacros.h"

DEFINE_LOG_CATEGORY_STATIC(LogModerationConsolePlayerSelector, All, All);

UWidget* UModerationConsolePlayerSelector::NativeGetDesiredFocusTarget() const
{
	return PlayersList->GetFirstFocusableItem();
}

void UModerationConsolePlayerSelector::SetOwner(UPlayerServerDetailsDataObject* Owner)
{
	TArray<UPlayerServerDetailsDataObject*> OwnerArray;
	if (Owner)
	{
		OwnerArray.Add(Owner);
	}
	PlayersList->SetOwner(Owner);
}

void UModerationConsolePlayerSelector::SetModerators(const TArray<UPlayerServerDetailsDataObject*>& Players)
{
	PlayersList->SetModerators(Players);
}

void UModerationConsolePlayerSelector::SetOnlinePlayers(const TArray<UPlayerServerDetailsDataObject*>& Players)
{
	PlayersList->SetOnlinePlayers(Players);
}

void UModerationConsolePlayerSelector::SetOfflinePlayers(const TArray<UPlayerServerDetailsDataObject*>& Players)
{
	PlayersList->SetOfflinePlayers(Players);
}

void UModerationConsolePlayerSelector::SetBannedPlayers(const TArray<UPlayerServerDetailsDataObject*>& Players)
{
	PlayersList->SetBannedPlayers(Players);
}

void UModerationConsolePlayerSelector::SetDisplayMode(EPlayerSelectorMode Mode)
{
	if (Mode != DisplayMode)
	{
		DisplayMode = Mode;

		PlayersList->SetDisplayMode(Mode);
		
		SelectedPlayers.Empty();
		HeaderCheckbox->SetIsChecked(false);
		OnSelectedPlayersChanged.Broadcast(SelectedPlayers);
	}
}

void UModerationConsolePlayerSelector::SelectAllPlayers()
{
	PlayersList->SelectAllPlayers();
}

void UModerationConsolePlayerSelector::ClearSelection()
{
	SelectedPlayers.Empty();
	OnSelectedPlayersChanged.Broadcast(SelectedPlayers);
	PlayersList->ClearSelectedPlayers();
}

FReply UModerationConsolePlayerSelector::NativeOnFocusReceived(const FGeometry& InGeometry,
	const FFocusEvent& InFocusEvent)
{
	FReply Reply = FReply::Handled();
	Reply.SetUserFocus(GetFirstFocusablePlayerWidget()->TakeWidget());
	return Reply;
}

void UModerationConsolePlayerSelector::NativeOnActivated()
{
	Super::NativeOnActivated();
	ON_INPUT_METHOD_CHANGED([this](ECommonInputType InputType)
	{
		SetFocusOnFirstPlayer();
	});
}

void UModerationConsolePlayerSelector::NativeOnDeactivated()
{
	if (ULocalPlayer* LocalPlayer = GetOwningLocalPlayer())
	{
		UCommonInputSubsystem::Get(LocalPlayer)->OnInputMethodChangedNative.RemoveAll(this);
	}
	Super::NativeOnDeactivated();
}

void UModerationConsolePlayerSelector::OnHeaderCheckboxStateChanged(bool bIsChecked)
{
	if (bIsChecked)
	{
		SelectAllPlayers();
	}
	else
	{
		ClearSelection();
	}
}

void UModerationConsolePlayerSelector::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	PlayersList->OnEntryWidgetGenerated().AddWeakLambda(this, [this](UWidget& EntryWidget)
	{
		ULocalPlayer* LocalPlayer = GetOwningLocalPlayer();
		if (UCommonInputSubsystem::Get(LocalPlayer)->GetCurrentInputType() == ECommonInputType::Gamepad)
		{
			if (PlayersList->GetNumItems() > 1 && &EntryWidget == PlayersList->GetFirstFocusableItem())
			{
				EntryWidget.SetFocus();
				PlayersList->OnEntryWidgetGenerated().RemoveAll(this);
			}
		}
	});

	auto SelectionChangedCallback = [this](UPlayerServerDetailsDataObject* Player, bool Selected)
	{
		if (Selected)
		{
			SelectedPlayers.AddUnique(Player);
		}
		else
		{
			SelectedPlayers.Remove(Player);
		}

		OnSelectedPlayersChanged.Broadcast(SelectedPlayers);
	};

	auto SelectedExclusiveCallback = [this](UPlayerServerDetailsDataObject* Player)
	{
		// TODO: Uncheck select all checkbox if it is checked
		HeaderCheckbox->SetIsChecked(false);
		SelectedPlayers.Empty();
		SelectedPlayers.Add(Player);
		OnSelectedPlayersChanged.Broadcast(SelectedPlayers);
	};

	auto FocusSetToPlayerCallback = [this](UWidget* PlayerWidget)
	{
		OnFocusSetToPlayer.ExecuteIfBound(PlayerWidget);
	};

	PlayersList->OnPlayerItemReceivedFocus.BindWeakLambda(this, FocusSetToPlayerCallback);
	PlayersList->OnPlayerSelectionChanged.BindWeakLambda(this, SelectionChangedCallback);
	PlayersList->OnPlayerSelectedExclusive.BindWeakLambda(this, SelectedExclusiveCallback);
	PlayersList->ListenForSelectedPlayersChange(OnSelectedPlayersChanged);

	PlayersList->AreAnyPlayersSelectedDelegate.BindWeakLambda(this, [this]() -> bool
	{
		return HasSelectedPlayers();
	});
	
	PlayersList->GetRightNavFocusDestination.BindWeakLambda(this, [this]() -> UUserWidget*
	{
		return this;
	});
	
	HeaderCheckbox->OnCheckStateChanged.AddDynamic(this, &UModerationConsolePlayerSelector::OnHeaderCheckboxStateChanged);
}

void UModerationConsolePlayerSelector::SetFocusOnFirstPlayer()
{
	PlayersList->SetFocusOnFirstPlayer();
}

bool UModerationConsolePlayerSelector::HasSelectedPlayers()
{
	return SelectedPlayers.Num() > 0;
}

void UModerationConsolePlayerSelector::SetupNavigation(const FGetFocusDestination& Delegate)
{
	PlayersList->SetupNavigation(Delegate);
}

UWidget* UModerationConsolePlayerSelector::GetFirstFocusablePlayerWidget()
{
	return PlayersList->GetFirstFocusableItem();
}
