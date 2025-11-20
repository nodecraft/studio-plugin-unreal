// Nodecraft, Inc. © 2012-2024, All Rights Reserved.


#include "UI/Settings/UserSettingsDataPrivacy.h"

#include "CommonInputSubsystem.h"
#include "Input/CommonUIInputTypes.h"
#include "Models/PlayerSettings.h"
#include "Services/PlayerSettingsService.h"
#include "UI/Alerts/AlertMessage.h"
#include "UI/Common/NodecraftLoadGuard.h"
#include "UI/Common/NodecraftRadioButton.h"
#include "UI/Common/NodecraftRadioButtonGroup.h"
#include "UI/Foundation/NodecraftButtonBase.h"
#include "Utility/NodecraftMacros.h"


#define LOCTEXT_NAMESPACE "UserSettingsDataPrivacy"
#define OPT_IN_ID "opt-in"
#define OPT_OUT_ID "opt-out"

void UUserSettingsDataPrivacy::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	ON_INPUT_METHOD_CHANGED(UpdateActionBindings)
}

void UUserSettingsDataPrivacy::NativeConstruct()
{
	Super::NativeConstruct();

	SaveSettingsButton->OnClicked().AddWeakLambda(this, [this]
	{
		FGetPlayerSettingsDelegate OnUpdatePlayerSettings;
		OnUpdatePlayerSettings.BindWeakLambda(this, [this](UPlayerSettings* Settings, bool bSuccess, FText Error)
		{
			if (bSuccess)
			{
				FText Message = LOCTEXT("SuccessUpdatingSettings", "Successfully updated settings.");
				AlertMessage->Show(Message, EAlertType::Success);
				RadioButtonGroup->SetSelectedId(Settings->GetAnalyticsOptOut() ? OPT_OUT_ID : OPT_IN_ID);
			}
			else
			{
				AlertMessage->Show(Error, EAlertType::Error);
			}
		});
		UPlayerSettingsService::Get().UpdatePlayerAnalyticsOptOut(RadioButtonGroup->GetSelectedId() == OPT_OUT_ID, OnUpdatePlayerSettings);
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
			RadioButtonGroup->SetSelectedId(PlayerSettings->GetAnalyticsOptOut() ? OPT_OUT_ID : OPT_IN_ID);
		}
		LoadGuard->SetIsLoading(false);
	});
	UPlayerSettingsService::Get().GetPlayerSettings(OnGetPlayerSettings);
}

UWidget* UUserSettingsDataPrivacy::NativeGetDesiredFocusTarget() const
{
	return RadioButtonGroup ? RadioButtonGroup : Super::NativeGetDesiredFocusTarget();
}

void UUserSettingsDataPrivacy::UpdateActionBindings(ECommonInputType CurrentInputType)
{
	SaveSettingsButton->SetVisibility(CurrentInputType == ECommonInputType::MouseAndKeyboard ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);

	if (SaveUIActionHandle.IsValid() && CurrentInputType == ECommonInputType::MouseAndKeyboard)
	{
		SaveUIActionHandle.Unregister();
	}
	else if (SaveInputActionData.IsNull() == false && SaveUIActionHandle.IsValid() == false && CurrentInputType != ECommonInputType::MouseAndKeyboard)
	{
		SaveUIActionHandle = RegisterUIActionBinding(
			FBindUIActionArgs(SaveInputActionData, bDisplayInActionBar,
				FSimpleDelegate::CreateWeakLambda(this, [this]
			{
				SaveSettingsButton->OnClicked().Broadcast();
			})));
	}
}

#undef LOCTEXT_NAMESPACE
#undef OPT_IN_ID
#undef OPT_OUT_ID
