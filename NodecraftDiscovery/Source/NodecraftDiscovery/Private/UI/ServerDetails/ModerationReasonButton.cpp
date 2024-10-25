// Nodecraft, Inc. © 2012-2024, All Rights Reserved.


#include "UI/ServerDetails/ModerationReasonButton.h"

#include "Models/ModerationReasonDataObject.h"
#include "Subsystems/AssetStreamerSubsystem.h"
#include "UI/ServerDetails/ViewModels/ModerationReasonTileViewModel.h"

void UModerationReasonButton::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	if (UModerationReasonTileViewModel* VM = Cast<UModerationReasonTileViewModel>(ListItemObject))
	{
		UModerationReasonDataObject* Reason = VM->GetReason();
		Label->SetText(Reason->GetTitle());
		Description->SetText(Reason->GetDescription());
	}
}

void UModerationReasonButton::SetUseSelectedStyle(const bool bUseSelectedStyle)
{
	TSoftClassPtr<UCommonButtonStyle> ButtonStyle = bUseSelectedStyle ? SelectedButtonStyle : NormalButtonStyle;

	// Load the style and apply it to the button
	UAssetStreamerSubsystem::Get().LoadAssetAsync(ButtonStyle.ToSoftObjectPath(), FStreamableDelegate::CreateWeakLambda(
		this, [this, ButtonStyle]
	{
		SetStyle(ButtonStyle.Get());
		UpdateButtonStyle();
	}));
}

