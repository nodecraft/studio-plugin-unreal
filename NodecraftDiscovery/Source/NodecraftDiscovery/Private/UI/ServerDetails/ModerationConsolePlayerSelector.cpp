// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ServerDetails/ModerationConsolePlayerSelector.h"

DEFINE_LOG_CATEGORY_STATIC(LogModerationConsolePlayerSelector, All, All);

void UModerationConsolePlayerSelector::SetOwner(UPlayerServerDetailsDataObject* Owner)
{
	TArray<UPlayerServerDetailsDataObject*> OwnerArray;
	OwnerArray.Add(Owner);
	OwnerSection->SetPlayers(OwnerArray);
}

void UModerationConsolePlayerSelector::SetModerators(const TArray<UPlayerServerDetailsDataObject*>& Players)
{
	ModeratorsSection->SetPlayers(Players);
}

void UModerationConsolePlayerSelector::SetOnlinePlayers(const TArray<UPlayerServerDetailsDataObject*>& Players)
{
	OnlinePlayersSection->SetPlayers(Players);
}

void UModerationConsolePlayerSelector::SetOfflinePlayers(const TArray<UPlayerServerDetailsDataObject*>& Players)
{
	OfflinePlayersSection->SetPlayers(Players);
}

void UModerationConsolePlayerSelector::SetBannedPlayers(const TArray<UPlayerServerDetailsDataObject*>& Players)
{
	BannedPlayersSection->SetPlayers(Players);
}

void UModerationConsolePlayerSelector::SetDisplayMode(EPlayerSelectorMode Mode)
{
	if (Mode != DisplayMode)
	{
		DisplayMode = Mode;
		OwnerSection->SetVisibility(Mode == EPlayerSelectorMode::Staff || Mode == EPlayerSelectorMode::All ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
		ModeratorsSection->SetVisibility(Mode == EPlayerSelectorMode::Staff || Mode == EPlayerSelectorMode::All ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
		OnlinePlayersSection->SetVisibility(Mode == EPlayerSelectorMode::Players || Mode == EPlayerSelectorMode::All ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
		OfflinePlayersSection->SetVisibility(Mode == EPlayerSelectorMode::Players || Mode == EPlayerSelectorMode::All ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
		BannedPlayersSection->SetVisibility(Mode == EPlayerSelectorMode::Banned || Mode == EPlayerSelectorMode::All ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
		SelectedPlayers.Empty();
		HeaderCheckbox->SetIsChecked(false);
		OnSelectedPlayersChanged.Broadcast(SelectedPlayers);
	}
}

void UModerationConsolePlayerSelector::SelectAllPlayers()
{
	if (DisplayMode == EPlayerSelectorMode::All || DisplayMode == EPlayerSelectorMode::Staff)
	{
		OwnerSection->SelectAllPlayers();
		ModeratorsSection->SelectAllPlayers();
	}
	if (DisplayMode == EPlayerSelectorMode::All || DisplayMode == EPlayerSelectorMode::Players)
	{
		OnlinePlayersSection->SelectAllPlayers();
		OfflinePlayersSection->SelectAllPlayers();
	}
	if (DisplayMode == EPlayerSelectorMode::All || DisplayMode == EPlayerSelectorMode::Banned)
	{
		BannedPlayersSection->SelectAllPlayers();
	}
}

void UModerationConsolePlayerSelector::OnHeaderCheckboxStateChanged(bool bIsChecked)
{
	if (bIsChecked)
	{
		SelectAllPlayers();
	}
	else
	{
		SelectedPlayers.Empty();
		OnSelectedPlayersChanged.Broadcast(SelectedPlayers);
	}
}

void UModerationConsolePlayerSelector::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	auto SelectionChangedCallback = [this](UPlayerServerDetailsDataObject* Player, bool Selected)
	{
		if (Selected)
		{
			SelectedPlayers.Add(Player);
		}
		else
		{
			SelectedPlayers.Remove(Player);
		}

		OnSelectedPlayersChanged.Broadcast(SelectedPlayers);
	};

	auto SelectedExclusiveCallback = [this](UPlayerServerDetailsDataObject* Player)
	{
		SelectedPlayers.Empty();
		SelectedPlayers.Add(Player);
		OnSelectedPlayersChanged.Broadcast(SelectedPlayers);
	};
	
	OwnerSection->OnPlayerSelectionChanged.BindWeakLambda(this, SelectionChangedCallback);
	ModeratorsSection->OnPlayerSelectionChanged.BindWeakLambda(this, SelectionChangedCallback);
	OnlinePlayersSection->OnPlayerSelectionChanged.BindWeakLambda(this, SelectionChangedCallback);
	OfflinePlayersSection->OnPlayerSelectionChanged.BindWeakLambda(this, SelectionChangedCallback);
	BannedPlayersSection->OnPlayerSelectionChanged.BindWeakLambda(this, SelectionChangedCallback);

	OwnerSection->OnPlayerSelectedExclusive.BindWeakLambda(this, SelectedExclusiveCallback);
	ModeratorsSection->OnPlayerSelectedExclusive.BindWeakLambda(this, SelectedExclusiveCallback);
	OnlinePlayersSection->OnPlayerSelectedExclusive.BindWeakLambda(this, SelectedExclusiveCallback);
	OfflinePlayersSection->OnPlayerSelectedExclusive.BindWeakLambda(this, SelectedExclusiveCallback);
	BannedPlayersSection->OnPlayerSelectedExclusive.BindWeakLambda(this, SelectedExclusiveCallback);

	OwnerSection->ListenForSelectedPlayersChange(OnSelectedPlayersChanged);
	ModeratorsSection->ListenForSelectedPlayersChange(OnSelectedPlayersChanged);
	OnlinePlayersSection->ListenForSelectedPlayersChange(OnSelectedPlayersChanged);
	OfflinePlayersSection->ListenForSelectedPlayersChange(OnSelectedPlayersChanged);
	BannedPlayersSection->ListenForSelectedPlayersChange(OnSelectedPlayersChanged);

	HeaderCheckbox->OnCheckStateChanged.AddDynamic(this, &UModerationConsolePlayerSelector::OnHeaderCheckboxStateChanged);
}
