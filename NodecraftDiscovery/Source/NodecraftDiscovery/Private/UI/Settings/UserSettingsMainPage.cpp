﻿// Nodecraft, Inc. © 2012-2024, All Rights Reserved.


#include "UI/Settings/UserSettingsMainPage.h"

#include "CommonTextBlock.h"
#include "API/NodecraftStudioSessionManagerSubsystem.h"
#include "DeveloperSettings/NodecraftStudioIdentitySettings.h"
#include "Services/IdentService.h"
#include "UI/Common/AsyncImage.h"
#include "UI/Foundation/NodecraftButtonBase.h"

void UUserSettingsMainPage::NativeConstruct()
{
	Super::NativeConstruct();

	// navigate to different tabs
	ChangeServerRegionButton->OnClicked().AddWeakLambda(this, [this]()
	{
		OnButtonClicked.ExecuteIfBound(1);
	});
	PlayerReputationButton->OnClicked().AddWeakLambda(this, [this]()
	{
		OnButtonClicked.ExecuteIfBound(2);
	});
	DataPrivacyButton->OnClicked().AddWeakLambda(this, [this]()
	{
		OnButtonClicked.ExecuteIfBound(3);
	});
	LegalStuffButton->OnClicked().AddWeakLambda(this, [this]()
	{
		OnButtonClicked.ExecuteIfBound(4);
	});

	// get player name, avatar and ident type
	const FPlayerSession PlayerSession = UNodecraftStudioSessionManager::Get().GetPlayerSession();
	const FText User = FText::FromString(PlayerSession.PlayerData->GetUsername().IsEmpty() ?
		PlayerSession.PlayerData->GetIdent() : PlayerSession.PlayerData->GetUsername());
    Username->SetText(User);
    PlayerImage->LoadPlayerAvatarAsync(PlayerSession.PlayerData);
	UTexture2D* IdentTexture = UNodecraftStudioIdentitySettings::Get().GetIconForType(PlayerSession.PlayerData->GetIdentType());
	IdentTypeImage->SetBrushFromTexture(IdentTexture);
}
