// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Settings/UserSettingsLegal.h"

#include "Services/ConsentsService.h"
#include "Subsystems/AssetStreamerSubsystem.h"
#include "Subsystems/MenuManagerSubsystem.h"
#include "UI/Foundation/NodecraftButtonBase.h"
#include "UI/Settings/TermsOfServiceModal.h"

void UUserSettingsLegal::NativeConstruct()
{
	Super::NativeConstruct();

	TermsOfService->OnClicked().AddWeakLambda(this, [this]
	{
		// open modal
		UAssetStreamerSubsystem::Get().LoadAssetAsync(TermsOfServiceModal.ToSoftObjectPath(),
			FStreamableDelegate::CreateWeakLambda(this, [this]
		{
			FGetPlayerConsentsResponseDelegate OnGetPlayerConsents;
			OnGetPlayerConsents.BindWeakLambda(this, [this](const FPlayerConsents& Consents, bool bSuccess, TOptional<FText> Error)
			{
				if (bSuccess)
				{
					UMenuManagerSubsystem::Get().ShowPlayerConsents(TermsOfServiceModal.Get(), Consents);
				}
			});
			UConsentsService::Get().GetPlayerLegalConsents(OnGetPlayerConsents);
		}));
	});
	
	PrivacyPolicy->OnClicked().AddWeakLambda(this, [this]
	{
		// TODO: redirect to privacy policy page
	});
	
	SoftwareLicenses->OnClicked().AddWeakLambda(this, [this]
	{
		// TODO: show software licenses
	});
}
