// Nodecraft, Inc. © 2012-2024, All Rights Reserved.


#include "UI/Settings/UserSettings.h"

#include "CommonActivatableWidgetSwitcher.h"
#include "CommonInputSubsystem.h"
#include "Input/CommonUIInputTypes.h"
#include "UI/Alerts/AlertMessage.h"
#include "UI/Foundation/NodecraftButtonBase.h"
#include "UI/Settings/UserSettingsAccountReputation.h"
#include "UI/Settings/UserSettingsDataPrivacy.h"
#include "UI/Settings/UserSettingsLegal.h"
#include "UI/Settings/UserSettingsMainPage.h"
#include "UI/Settings/UserSettingsServerRegion.h"
#include "Utility/NodecraftMacros.h"

void UUserSettings::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	ON_INPUT_METHOD_CHANGED(UpdateActionBindings)
}

void UUserSettings::NativeConstruct()
{
	Super::NativeConstruct();

	BackButton->OnClicked().AddWeakLambda(this, [this]()
	{
		GoToMainPage();
		ServerRegionPage->GetAlertMessage()->Hide();
	});
	
	MainPage->OnButtonClicked.BindWeakLambda(this, [this](int32 PageIndex)
	{
		CurrentPageIndex = PageIndex;
		WidgetSwitcher->SetActiveWidgetIndex(PageIndex);
		ON_INPUT_METHOD_CHANGED(UpdateActionBindings)

		if (PageIndex != 0 && BackButtonInputActionHandle.IsValid() == false)
		{
			BackButtonInputActionHandle = RegisterUIActionBinding(
				FBindUIActionArgs(BackButtonInputActionData, bDisplayInActionBar,
					FSimpleDelegate::CreateWeakLambda(this, [this]
					{
						BackButton->OnClicked().Broadcast();
					})));
		}
	});
}

void UUserSettings::NativeDestruct()
{
	Super::NativeDestruct();
	if (BackButtonInputActionHandle.IsValid())
	{
		BackButtonInputActionHandle.Unregister();
	}
}

UWidget* UUserSettings::NativeGetDesiredFocusTarget() const
{
	if (MainPage->IsVisible())
	{
		return MainPage->GetDesiredFocusTarget();
	}
	if (ServerRegionPage->IsVisible())
	{
		return ServerRegionPage->GetDesiredFocusTarget();
	}
	if (AccountReputationPage->IsVisible())
	{
		return AccountReputationPage->GetDesiredFocusTarget();
	}
	if (DataPrivacyPage->IsVisible())
	{
		return DataPrivacyPage->GetDesiredFocusTarget();
	}
	if (LegalPage->IsVisible())
	{
		return LegalPage->GetDesiredFocusTarget();
	}
	return Super::NativeGetDesiredFocusTarget();
}

void UUserSettings::GoToMainPage()
{
	WidgetSwitcher->SetActiveWidgetIndex(0);
	CurrentPageIndex = 0;
	BackButton->SetVisibility(ESlateVisibility::Collapsed);
	if (BackButtonInputActionHandle.IsValid())
	{
		BackButtonInputActionHandle.Unregister();
	}
}

void UUserSettings::UpdateActionBindings(ECommonInputType CurrentInputType)
{
	if (CurrentInputType != ECommonInputType::Gamepad)
	{
		BackButton->SetVisibility(CurrentPageIndex == 0 ? ESlateVisibility::Collapsed : ESlateVisibility::Visible);
	}
}
