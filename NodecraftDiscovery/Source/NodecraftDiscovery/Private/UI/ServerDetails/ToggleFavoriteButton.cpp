// Nodecraft, Inc. © 2012-2024, All Rights Reserved.


#include "UI/ServerDetails/ToggleFavoriteButton.h"

#include "Subsystems/AssetStreamerSubsystem.h"

void UToggleFavoriteButton::SetIsFavorited(bool bIsFavorited)
{
	TSoftClassPtr<UCommonButtonStyle> ButtonStyle = bIsFavorited ? ButtonStyleFavorited : ButtonStyleNotFavorited;

	// Load the style and apply it to the button
	UAssetStreamerSubsystem::Get().LoadAssetAsync(ButtonStyle.ToSoftObjectPath(), FStreamableDelegate::CreateWeakLambda(
		this, [this, ButtonStyle]
	{
		SetStyle(ButtonStyle.Get());
		UpdateButtonStyle();
	}));
}
