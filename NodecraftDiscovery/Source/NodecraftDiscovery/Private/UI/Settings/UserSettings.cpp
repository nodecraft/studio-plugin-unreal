// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Settings/UserSettings.h"

#include "CommonActivatableWidgetSwitcher.h"
#include "UI/Alerts/AlertMessage.h"
#include "UI/Foundation/NodecraftButtonBase.h"
#include "UI/Settings/UserSettingsMainPage.h"
#include "UI/Settings/UserSettingsServerRegion.h"

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
		WidgetSwitcher->SetActiveWidgetIndex(PageIndex);
        BackButton->SetVisibility(PageIndex == 0 ? ESlateVisibility::Collapsed : ESlateVisibility::Visible);
	});
}

void UUserSettings::GoToMainPage() const
{
	WidgetSwitcher->SetActiveWidgetIndex(0);
	BackButton->SetVisibility(ESlateVisibility::Collapsed);
}

