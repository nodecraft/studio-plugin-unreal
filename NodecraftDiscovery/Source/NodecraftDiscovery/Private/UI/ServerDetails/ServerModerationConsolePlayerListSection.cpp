// Nodecraft, Inc. © 2012-2024, All Rights Reserved.


#include "UI/ServerDetails/ServerModerationConsolePlayerListSection.h"

#include "Engine/Texture2D.h"
#include "Models/PlayerServerDetails.h"
#include "Subsystems/AssetStreamerSubsystem.h"
#include "UI/ServerDetails/ServerModerationConsolePlayerListItem.h"

void UServerModerationConsolePlayerListSection::NativePreConstruct()
{
	Super::NativePreConstruct();
	UAssetStreamerSubsystem::Get().LoadAssetAsync(HeaderIconTexture.ToSoftObjectPath(), FStreamableDelegate::CreateWeakLambda(this, [this]()
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
	TArray<UPlayerServerDetailsViewModel*> PlayerViewModels = {};
	
	for (UPlayerServerDetailsDataObject* Player : Players)
	{
		UPlayerServerDetailsViewModel* PlayerViewModel = UPlayerServerDetailsViewModel::FromDataObject(Player);
		PlayerViewModel->OnPlayerSelectionChanged = OnPlayerSelectionChanged;
		PlayerViewModel->OnPlayerSelectedExclusive = OnPlayerSelectedExclusive;
		PlayerViewModel->OnPlayerItemReceivedFocus = OnPlayerItemReceivedFocus;
		PlayerViewModel->bIsSelectable = bAreChildrenSelectable;
		PlayerViewModels.Add(PlayerViewModel);
	}
	PlayerList->ClearListItems();

	if (Players.Num() > 0)
	{
		PlayerList->SetListItems(PlayerViewModels);
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
			if (VM)
			{
				if (UServerModerationConsolePlayerListItem* PlayerListItem = Cast<UServerModerationConsolePlayerListItem>(PlayerList->GetEntryWidgetFromItem(ListItem)))
				{
					PlayerListItem->SetSelected(SelectedPlayers.Contains(VM->GetPlayerServerDetails()));
				}
			}
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

void UServerModerationConsolePlayerListSection::ClearSelection()
{
	for (UUserWidget* DisplayedEntryWidget : PlayerList->GetDisplayedEntryWidgets())
	{
		if (UServerModerationConsolePlayerListItem* PlayerListItem = Cast<UServerModerationConsolePlayerListItem>(DisplayedEntryWidget))
		{
			PlayerListItem->ClearSelection();
		}
	}
}