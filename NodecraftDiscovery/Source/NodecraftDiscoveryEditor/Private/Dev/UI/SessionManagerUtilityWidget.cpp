// Fill out your copyright notice in the Description page of Project Settings.

#include "Dev/UI/SessionManagerUtilityWidget.h"

#include "API/DiscoverySessionManagerSubsystem.h"

void USessionManagerUtilityWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SetNewPlayerTokenButton->OnClicked.AddUniqueDynamic(this, &USessionManagerUtilityWidget::SetPlayerToken);


	UDiscoverySessionManager& SessionManager = UDiscoverySessionManager::Get();

	CurrentPlayerTokenTextBlock->SetText(FText::FromString(SessionManager.GetPlayerAccessToken()));
}

void USessionManagerUtilityWidget::SetPlayerToken()
{
	// unimplemented()
	

	// This used to work but we've changed over from using raw tokens to user sessions. If you want to use this, you'll need to update it.
	
	UDiscoverySessionManager& SessionManager = UDiscoverySessionManager::Get();
	SessionManager.SetPlayerAccessToken(NewPlayerTokenTextBox->GetText().ToString());
	//
	// CurrentPlayerTokenTextBlock->SetText(FText::FromString(SessionManager.GetPlayerAccessToken()));
}

