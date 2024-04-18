// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#include "Dev/UI/SessionManagerUtilityWidget.h"

#include "API/NodecraftStudioSessionManagerSubsystem.h"

void USessionManagerUtilityWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SetNewPlayerTokenButton->OnClicked.AddUniqueDynamic(this, &USessionManagerUtilityWidget::SetPlayerToken);


	UNodecraftStudioSessionManager& SessionManager = UNodecraftStudioSessionManager::Get();

	CurrentPlayerTokenTextBlock->SetText(FText::FromString(SessionManager.GetPlayerAccessToken()));
}

void USessionManagerUtilityWidget::SetPlayerToken()
{
	// unimplemented()
	

	// This used to work but we've changed over from using raw tokens to user sessions. If you want to use this, you'll need to update it.
	
	UNodecraftStudioSessionManager& SessionManager = UNodecraftStudioSessionManager::Get();
	SessionManager.SetPlayerAccessToken(NewPlayerTokenTextBox->GetText().ToString());
	//
	// CurrentPlayerTokenTextBlock->SetText(FText::FromString(SessionManager.GetPlayerAccessToken()));
}

