// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Settings/UserSettingsDataPrivacy.h"

#include "CommonTextBlock.h"
#include "Models/PlayerSettings.h"
#include "Services/PlayerSettingsService.h"
#include "Subsystems/AssetStreamerSubsystem.h"
#include "UI/Alerts/AlertMessage.h"
#include "UI/Common/NodecraftLoadGuard.h"
#include "UI/Common/NodecraftRadioButton.h"
#include "UI/Foundation/NodecraftButtonBase.h"


#define LOCTEXT_NAMESPACE "UserSettingsDataPrivacy"

void UUserSettingsDataPrivacy::NativeConstruct()
{
	Super::NativeConstruct();

	AnalyticsOptIn->OnNodecraftRadioButtonClicked.BindWeakLambda(this, [this]()
	{
		AnalyticsOptOut->SetIsChecked(false);
		UpdateStyles();
	});
	AnalyticsOptOut->OnNodecraftRadioButtonClicked.BindWeakLambda(this, [this]()
	{
		AnalyticsOptIn->SetIsChecked(false);
		UpdateStyles();
	});

	SaveSettingsButton->OnClicked().AddWeakLambda(this, [this]
	{
		FGetPlayerSettingsDelegate OnUpdatePlayerSettings;
		OnUpdatePlayerSettings.BindWeakLambda(this, [this](UPlayerSettings* Settings, bool bSuccess, FText Error)
		{
			if (bSuccess)
			{
				FText Message = LOCTEXT("SuccessUpdatingSettings", "Successfully updated settings.");
				AlertMessage->Show(Message, EAlertType::Success);
				AnalyticsOptIn->SetIsChecked(Settings->GetAnalyticsOptOut() == false);
				AnalyticsOptOut->SetIsChecked(Settings->GetAnalyticsOptOut());
			}
			else
			{
				AlertMessage->Show(Error, EAlertType::Error);
			}
		});
		UPlayerSettingsService::Get().UpdatePlayerAnalyticsOptOut(AnalyticsOptOut->GetIsChecked(), OnUpdatePlayerSettings);
	});
}

void UUserSettingsDataPrivacy::NativeOnActivated()
{
	Super::NativeOnActivated();

	AlertMessage->Hide();
	LoadGuard->SetIsLoading(true);

	FGetPlayerSettingsDelegate OnGetPlayerSettings;
	OnGetPlayerSettings.BindWeakLambda(this, [this](UPlayerSettings* PlayerSettings, bool bSuccess, FText Error)
	{
		if (bSuccess)
		{
			AnalyticsOptIn->SetIsChecked(PlayerSettings->GetAnalyticsOptOut() == false);
			AnalyticsOptOut->SetIsChecked(PlayerSettings->GetAnalyticsOptOut());

			UpdateStyles();
		}
		LoadGuard->SetIsLoading(false);
	});
	UPlayerSettingsService::Get().GetPlayerSettings(OnGetPlayerSettings);
}

void UUserSettingsDataPrivacy::UpdateStyles()
{
	if (SelectedTextStyle.IsNull() == false)
	{
		FStreamableDelegate OnLoaded;
		OnLoaded.BindWeakLambda(this, [this]
		{
			if (AnalyticsOptIn->GetIsChecked())
			{
				OptInText->SetStyle(SelectedTextStyle.Get());
			}
			else
			{
				OptOutText->SetStyle(SelectedTextStyle.Get());
			}
		});
		UAssetStreamerSubsystem::Get().LoadAssetAsync(SelectedTextStyle.ToSoftObjectPath(), OnLoaded);
	}

	if (NormalTextStyle.IsNull() == false)
	{
		FStreamableDelegate OnLoaded;
		OnLoaded.BindWeakLambda(this, [this]
		{
			if (AnalyticsOptIn->GetIsChecked())
			{
				OptOutText->SetStyle(NormalTextStyle.Get());
			}
			else
			{
				OptInText->SetStyle(NormalTextStyle.Get());
			}
		});
		UAssetStreamerSubsystem::Get().LoadAssetAsync(NormalTextStyle.ToSoftObjectPath(), OnLoaded);
	}
}

#undef LOCTEXT_NAMESPACE
