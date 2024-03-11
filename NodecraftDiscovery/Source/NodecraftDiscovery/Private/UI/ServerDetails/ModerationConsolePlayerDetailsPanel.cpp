// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ServerDetails/ModerationConsolePlayerDetailsPanel.h"

#include "Models/PlayerServerDetails.h"

void UModerationConsolePlayerDetailsPanel::SetSelectedPlayers(const TArray<UPlayerServerDetailsDataObject*> SelectedPlayers)
{
	NoPlayerSelectedContent->SetVisibility(SelectedPlayers.Num() == 0 ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	PlayerSelectedContent->SetVisibility(SelectedPlayers.Num() > 0 ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);

	FFormatOrderedArguments Args;
	Args.Add(SelectedPlayers.Num());
	// Set NumPlayersSelectedTextLabel to the number of selected players
	NumPlayersSelectedTextLabel->SetText(FText::Format(FTextFormat::FromString(TEXT("{0} Players Selected")), Args));
}

void UModerationConsolePlayerDetailsPanel::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	SetSelectedPlayers({});
}
