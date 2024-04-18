// Nodecraft, Inc. © 2012-2024, All Rights Reserved.


#include "UI/ServerDetails/ModerationActionButton.h"

#include "Subsystems/AssetStreamerSubsystem.h"

void UModerationActionButton::ConfigureForAction(const EModerationAction InAction)
{
	// Find style for action
	if (FModerationActionButtonStyle* ActionStyle = ActionStyles.Find(InAction))
	{
		// apply styles
		// Load icon async
		UAssetStreamerSubsystem::Get().LoadAssetAsync(ActionStyle->Icon.ToSoftObjectPath(), FStreamableDelegate::CreateWeakLambda(this, [this, ActionStyle]
		{
			ActionIcon->SetBrushFromTexture(ActionStyle->Icon.Get());
		}));
	
		ActionLabel->SetText(ActionStyle->Label);
		ActionDescription->SetText(ActionStyle->Description);
	}
}

void UModerationActionButton::Refresh()
{
	ConfigureForAction(Action);
}
