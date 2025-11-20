// Nodecraft, Inc. © 2012-2024, All Rights Reserved.


#include "UI/Common/ConsentLabelButton.h"

#include "Engine/Texture2D.h"
#include "Subsystems/AssetStreamerSubsystem.h"

void UConsentLabelButton::ShowTextDecoration(const bool bShow)
{
	TextIconDecorator->SetVisibility(bShow ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
}

void UConsentLabelButton::NativeConstruct()
{
	Super::NativeConstruct();
	ShowTextDecoration(false);
	SetIsLoading(false);
}

void UConsentLabelButton::SetIsEnabled(const bool bInIsEnabled)
{
	Super::SetIsEnabled(bInIsEnabled);
	UpdateButtonStyle();

	const TSoftObjectPtr<UTexture2D> TextDecorator = bInIsEnabled ? EnabledTextDecorator : DisasbledTextDecorator;
	if (TextDecorator.IsNull())
	{
		ShowTextDecoration(false);
	}
	else
	{
		UAssetStreamerSubsystem::Get().LoadAssetAsync(TextDecorator.ToSoftObjectPath(), FStreamableDelegate::CreateWeakLambda(this, [this, TextDecorator]
		{
			ShowTextDecoration(true);
			TextIconDecorator->SetBrushFromTexture(TextDecorator.Get());
		}));
	}
}

void UConsentLabelButton::SetIsLoading(const bool bIsLoading)
{
	LoadingSpinnerContainer->SetVisibility(bIsLoading ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	ConsentStatusContainer->SetVisibility(bIsLoading ? ESlateVisibility::Hidden : ESlateVisibility::Visible);
}
