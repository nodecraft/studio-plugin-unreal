// Nodecraft, Inc. © 2012-2024, All Rights Reserved.


#include "UI/Settings/UserSettingsServerRegion.h"

#include "CommonInputSubsystem.h"
#include "API/NodecraftStudioApi.h"
#include "Components/CheckBox.h"
#include "Input/CommonUIInputTypes.h"
#include "Models/PlayerSettings.h"
#include "Models/ServerRegionDataObject.h"
#include "Services/GameService.h"
#include "Services/PlayerSettingsService.h"
#include "UI/Alerts/AlertMessage.h"
#include "UI/Common/NodecraftLoadGuard.h"
#include "UI/Common/NodecraftRadioButtonGroup.h"
#include "UI/Foundation/NodecraftButtonBase.h"
#include "UI/Settings/UserSettings.h"
#include "Utility/NodecraftMacros.h"


#define LOCTEXT_NAMESPACE "UserSettingsServerRegion"
#define LOCTEXT_CURRENT_REGION LOCTEXT("CurrentRegion", "Current")
#define LOCTEXT_DEFAULT_REGION LOCTEXT("DefaultRegion", "Default")

void UUserSettingsServerRegion::NativeConstruct()
{
	Super::NativeConstruct();

	LoadGuard->SetIsLoading(true);
	AlertMessage->Hide();

	// get current server region ID from cache
	if (FPlayerSession PlayerSession = UNodecraftStudioSessionManager::Get().GetPlayerSession(); PlayerSession.IsValid())
	{
		CurrentRegion = PlayerSession.ServerRegionId;
		SelectedRegion = CurrentRegion;
	}
	
	// get default server region ID
	FGetPlayerSettingsDelegate OnGetPlayerSettings;
	OnGetPlayerSettings.BindWeakLambda(this, [this](UPlayerSettings* PlayerSettings, bool bSuccess, FText Error)
	{
		if (bSuccess)
		{
			DefaultRegion = PlayerSettings->GetDefaultServerRegionID();

			// create rows of server regions
			if (const TArray<UServerRegionDataObject*> Regions = UGameService::Get().GetCachedServerRegions(); Regions.Num() > 0)
			{
				CreateServerRegionRows(Regions);
			}
			LoadGuard->SetIsLoading(false);
		}
	});
	UPlayerSettingsService::Get().GetPlayerSettings(OnGetPlayerSettings);

	// disable save button unless we select a different region than currently selected region
	UpdateChangeRegionButtonEnabledState();

	// update the Change Region Button state when checkbox changes
	SetAsDefaultCheckBox->OnCheckStateChanged.AddDynamic(this, &ThisClass::OnDefaultRegionCheckboxStateChanged);

	// on Change Region button clicked: update the current region. check for setting a region as Default.
	ChangeRegionButton->OnClicked().AddWeakLambda(this, [this]()
	{
		FString PreviousDefaultRegion = DefaultRegion;
		
		if (SetAsDefaultCheckBox->IsChecked())
		{
			DefaultRegion = SelectedRegion;
		}

		// todo: we should try to do these calls in parallel
		// takes 2 api calls to update current and default player region
		FPlayerSessionResponseDelegate OnUpdateCurrentRegionComplete;
		OnUpdateCurrentRegionComplete.BindWeakLambda(this, [this, PreviousDefaultRegion](FPlayerSession PlayerSession, bool bConnectedSuccessfully, FText Error)
		{
			if (bConnectedSuccessfully)
			{
				FText Text = LOCTEXT("SessionUpdateSuccess", "Successfully updated session.");
				AlertMessage->Show(Text, EAlertType::Success);
				
				SelectedRegion = PlayerSession.ServerRegionId;
				if (UNodecraftRadioButton* RadioButton = Cast<UNodecraftRadioButton>(RadioButtonGroup->GetChildById(CurrentRegion)))
				{
					RadioButton->SetSecondaryLabelText(FText::GetEmpty());
				}
				CurrentRegion = SelectedRegion;
				if (UNodecraftRadioButton* RadioButton = Cast<UNodecraftRadioButton>(RadioButtonGroup->GetChildById(CurrentRegion)))
				{
					RadioButton->SetSecondaryLabelText(LOCTEXT_CURRENT_REGION);
				}
				
				UpdateChangeRegionButtonEnabledState();

				// update default region if default region checkbox is checked
				FGetPlayerSettingsDelegate OnUpdateDefaultRegionSettings;
				OnUpdateDefaultRegionSettings.BindWeakLambda(this, [this, PreviousDefaultRegion](UPlayerSettings* PlayerSettings, bool bSuccess, FText Error)
				{
					if (bSuccess)
					{
						if (UNodecraftRadioButton* RadioButton = Cast<UNodecraftRadioButton>(RadioButtonGroup->GetChildById(PreviousDefaultRegion)))
						{
							RadioButton->SetTertiaryLabelText(FText::GetEmpty());
						}
						if (UNodecraftRadioButton* RadioButton = Cast<UNodecraftRadioButton>(RadioButtonGroup->GetChildById(DefaultRegion)))
						{
							RadioButton->SetTertiaryLabelText(LOCTEXT_DEFAULT_REGION);
						}
					}
				});
				if (SetAsDefaultCheckBox->IsChecked())
				{
					UPlayerSettingsService::Get().UpdateDefaultPlayerRegion(DefaultRegion, OnUpdateDefaultRegionSettings);
				}
			}
			else
			{
				FText Text = LOCTEXT("SessionUpdateFailure", "Failed to update session.");
				AlertMessage->Show(Text, EAlertType::Error);
			}
		});
		UPlayerSettingsService::Get().UpdateCurrentPlayerRegion(SelectedRegion, OnUpdateCurrentRegionComplete);
	});
}

void UUserSettingsServerRegion::CreateServerRegionRows(const TArray<UServerRegionDataObject*>& Regions)
{
	for (UServerRegionDataObject* Region : Regions)
	{
		FNodecraftRadioButtonConfiguration Descriptor = FNodecraftRadioButtonConfiguration();
		Descriptor.Id = FName(Region->GetID());
		Descriptor.PrimaryLabel = Region->GetTitle();
		Descriptor.SecondaryLabel = Region->GetID() == CurrentRegion ? LOCTEXT_CURRENT_REGION : FText::GetEmpty();
		Descriptor.TertiaryLabel = Region->GetID() == DefaultRegion ? LOCTEXT_DEFAULT_REGION : FText::GetEmpty();
		RadioButtonGroup->AddChildRadioButton(Descriptor, Region->GetID() == CurrentRegion);
	}
	if (RadioButtonGroup->HasAnyChildren())
	{
		RadioButtonGroup->OnRadioButtonSelected.AddUniqueDynamic(this, &ThisClass::UpdateSelectedRegion);
	}
}

void UUserSettingsServerRegion::OnDefaultRegionCheckboxStateChanged(bool bIsChecked)
{
	UpdateChangeRegionButtonEnabledState();
}

void UUserSettingsServerRegion::UpdateChangeRegionButtonEnabledState()
{
	ChangeRegionButton->SetIsEnabled((CurrentRegion != SelectedRegion) || SetAsDefaultCheckBox->IsChecked());
	
	if (ULocalPlayer* LocalPlayer = GetOwningLocalPlayer())
	{
		UpdateActionBindings(UCommonInputSubsystem::Get(LocalPlayer)->GetCurrentInputType());
	}
}

UWidget* UUserSettingsServerRegion::NavRule_NavigateToLastServerRegion(EUINavigation InNavigation)
{
	if (RadioButtonGroup->HasAnyChildren())
	{
		return RadioButtonGroup->GetChildAt(RadioButtonGroup->GetChildrenCount() - 1);
	}
	return nullptr;
}

void UUserSettingsServerRegion::UpdateActionBindings(ECommonInputType CurrentInputType)
{
	ChangeRegionButton->SetVisibility(CurrentInputType == ECommonInputType::MouseAndKeyboard ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	
	if (ChangeRegionButton->GetIsEnabled() == false && ConfirmUIActionHandle.IsValid())
	{
		ConfirmUIActionHandle.Unregister();
	}
	else if (ChangeRegionButton->GetIsEnabled() && ConfirmInputActionData.IsNull() == false && ConfirmUIActionHandle.IsValid() == false)
	{
		ConfirmUIActionHandle = RegisterUIActionBinding(
			FBindUIActionArgs(ConfirmInputActionData, bDisplayInActionBar,
				FSimpleDelegate::CreateWeakLambda(this, [this]
				{
					ChangeRegionButton->OnClicked().Broadcast();
				})));
	}
}

void UUserSettingsServerRegion::UpdateSelectedRegion(int32 Index)
{
	SelectedRegion = RadioButtonGroup->GetSelectedId();
	UpdateChangeRegionButtonEnabledState();
}

UAlertMessage* UUserSettingsServerRegion::GetAlertMessage()
{
	return AlertMessage;
}

void UUserSettingsServerRegion::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	ON_INPUT_METHOD_CHANGED(UpdateActionBindings)
}

void UUserSettingsServerRegion::NativeDestruct()
{
	Super::NativeDestruct();
	if (ConfirmUIActionHandle.IsValid())
	{
		ConfirmUIActionHandle.Unregister();
	}
	RadioButtonGroup->OnRadioButtonSelected.RemoveAll(this);
}

UWidget* UUserSettingsServerRegion::NativeGetDesiredFocusTarget() const
{
	return RadioButtonGroup ? RadioButtonGroup : Super::NativeGetDesiredFocusTarget();
}

#undef LOCTEXT_NAMESPACE
#undef LOCTEXT_CURRENT_REGION
#undef LOCTEXT_DEFAULT_REGION
