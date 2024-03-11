// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ServerDetails/ServerModerationConsolePlayerListSection.h"

#include "Models/PlayerServerDetails.h"
#include "Subsystems/AssetStreamerSubsystem.h"
#include "UI/ServerDetails/ServerModerationConsolePlayerListItem.h"

void UServerModerationConsolePlayerListSection::NativePreConstruct()
{
	Super::NativePreConstruct();
	UAssetStreamerSubsystem::Get().LoadAssetAsync(HeaderIconTexture, FStreamableDelegate::CreateWeakLambda(this, [this]()
	{
		SectionHeaderIcon->SetBrushFromTexture(HeaderIconTexture.Get());
		SectionHeaderIcon->SetBrushTintColor(FSlateColor(HeaderIconTint));
	}));
	SectionHeaderTextBlock->SetText(HeaderText);

	if (NoPlayersTextBlock)
	{
		NoPlayersTextBlock->SetText(NoPlayersText);
	}
}

void UServerModerationConsolePlayerListSection::SetPlayers(const TArray<UPlayerServerDetailsDataObject*>& Players)
{
	TArray<UPlayerServerDetailsViewModel*> PlayerViewModels;
	
	for (UPlayerServerDetailsDataObject* Player : Players)
	{
		UPlayerServerDetailsViewModel* PlayerViewModel = UPlayerServerDetailsViewModel::FromDataObject(Player);
		PlayerViewModel->OnPlayerSelectionChanged = OnPlayerSelectionChanged;
		PlayerViewModel->OnPlayerSelectedExclusive = OnPlayerSelectedExclusive;
		PlayerViewModel->bIsSelectable = bAreChildrenSelectable;
		PlayerViewModels.Add(PlayerViewModel);
	}
	PlayerList->SetListItems(PlayerViewModels);

	if (Players.Num() > 0)
	{
		PlayerListContainer->SetVisibility(ESlateVisibility::Visible);
		NoPlayersContainer->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		PlayerListContainer->SetVisibility(ESlateVisibility::Collapsed);
		NoPlayersContainer->SetVisibility(ESlateVisibility::Visible);
	}
}

void UServerModerationConsolePlayerListSection::ListenForSelectedPlayersChange(
	FOnSelectedPlayersChanged& OnSelectedPlayersChanged)
{
	OnSelectedPlayersChanged.AddWeakLambda(this, [this](const TArray<UPlayerServerDetailsDataObject*>& SelectedPlayers)
	{
		for (UObject* ListItem : PlayerList->GetListItems())
		{
			const UPlayerServerDetailsViewModel* VM = Cast<UPlayerServerDetailsViewModel>(ListItem);
			Cast<UServerModerationConsolePlayerListItem>(PlayerList->GetEntryWidgetFromItem(ListItem))->SetSelected(SelectedPlayers.Contains(VM->GetPlayerServerDetails()));
		}
	});
}

void UServerModerationConsolePlayerListSection::SelectAllPlayers()
{
	if (bAreChildrenSelectable)
	{
		for (const UObject* ListItem : PlayerList->GetListItems())
		{
			Cast<UServerModerationConsolePlayerListItem>(PlayerList->GetEntryWidgetFromItem(ListItem))->Select();
		}
	}
}
