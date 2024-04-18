// Nodecraft, Inc. © 2012-2024, All Rights Reserved.


#include "UI/Settings/UserSettingsServerRegion.h"

#include "API/NodecraftStudioApi.h"
#include "Components/CheckBox.h"
#include "Components/VerticalBox.h"
#include "Models/PlayerSettings.h"
#include "Models/ServerRegionDataObject.h"
#include "Services/GameService.h"
#include "Services/PlayerSettingsService.h"
#include "Subsystems/AssetStreamerSubsystem.h"
#include "UI/Alerts/AlertMessage.h"
#include "UI/Common/NodecraftLoadGuard.h"
#include "UI/Foundation/NodecraftButtonBase.h"
#include "UI/Settings/ServerRegionRow.h"
#include "UI/Settings/UserSettings.h"


#define LOCTEXT_NAMESPACE "UserSettingsServerRegion"

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
		if (SetAsDefaultCheckBox->IsChecked())
		{
			DefaultRegion = SelectedRegion;
		}

		// todo: we should try to do these calls in parallel
		// takes 2 api calls to update current and default player region
		FPlayerSessionResponseDelegate OnUpdateCurrentRegionComplete;
		OnUpdateCurrentRegionComplete.BindWeakLambda(this, [this](FPlayerSession PlayerSession, bool bSuccess, FText Error)
		{
			if (bSuccess)
			{
				FText Text = LOCTEXT("SessionUpdateSuccess", "Successfully updated session.");
				AlertMessage->Show(Text, EAlertType::Success);
				
				SelectedRegion = PlayerSession.ServerRegionId;
				CurrentRegion = SelectedRegion;
				
				UpdateChangeRegionButtonEnabledState();

				for (UWidget* Child : RegionsVerticalBox->GetAllChildren())
				{
					if (UServerRegionRow* ServerRegionRow = Cast<UServerRegionRow>(Child))
					{
						// todo: would be better with store pattern
						ServerRegionRow->SetIsCurrent(ServerRegionRow->GetRegionID() == SelectedRegion, true);
					}
				}

				// update default region if default region checkbox is checked
				FGetPlayerSettingsDelegate OnUpdateDefaultRegionSettings;
				OnUpdateDefaultRegionSettings.BindWeakLambda(this, [this](UPlayerSettings* PlayerSettings, bool bSuccess, FText Error)
				{
					if (bSuccess)
					{
						for (UWidget* Child : RegionsVerticalBox->GetAllChildren())
						{
						   if (UServerRegionRow* ServerRegionRow = Cast<UServerRegionRow>(Child))
						   {
						   		ServerRegionRow->SetIsCurrent(ServerRegionRow->GetRegionID() == SelectedRegion, true);
								ServerRegionRow->SetIsDefault(ServerRegionRow->GetRegionID() == DefaultRegion);
						   }
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
	UAssetStreamerSubsystem::Get().LoadAssetAsync(RegionRowWidget.ToSoftObjectPath(), FStreamableDelegate::CreateWeakLambda(this, [this, Regions]
	{
		for (UServerRegionDataObject* Region : Regions)
		{
			if (UServerRegionRow* Row = CreateWidget<UServerRegionRow>(GetOwningPlayer(), RegionRowWidget.Get()); IsValid(Row))
			{
				Row->Configure(Region, Region->GetID() == DefaultRegion, Region->GetID() == SelectedRegion);
				RegionsVerticalBox->AddChild(Row);

				// On Region Row clicked: update visuals to set selected server region
				Row->OnServerRegionButtonClicked.BindWeakLambda(this, [this](FString RegionID)
				{
					SelectedRegion = RegionID;
					UpdateChangeRegionButtonEnabledState();
					
					for (UWidget* Child : RegionsVerticalBox->GetAllChildren())
					{
						if (UServerRegionRow* ServerRegionRow = Cast<UServerRegionRow>(Child))
						{
							ServerRegionRow->SetIsCurrent(ServerRegionRow->GetRegionID() == RegionID, false);
						}
					}
				});
			}
		}
	}));
}

void UUserSettingsServerRegion::OnDefaultRegionCheckboxStateChanged(bool bIsChecked)
{
	UpdateChangeRegionButtonEnabledState();
}

void UUserSettingsServerRegion::UpdateChangeRegionButtonEnabledState()
{
	ChangeRegionButton->SetIsEnabled((CurrentRegion != SelectedRegion) || SetAsDefaultCheckBox->IsChecked());
}

UAlertMessage* UUserSettingsServerRegion::GetAlertMessage()
{
	return AlertMessage;
}

#undef LOCTEXT_NAMESPACE
