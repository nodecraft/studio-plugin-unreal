// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Common/NodecraftBadgedButton.h"

#include "Subsystems/AssetStreamerSubsystem.h"

void UNodecraftBadgedButton::SetBadgeText(const FText& InText)
{
	BadgeText->SetText(InText);
}

void UNodecraftBadgedButton::SetIsBadged(const bool bIsBadged)
{
	BadgeContainer->SetVisibility(bIsBadged ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);

	TSoftClassPtr<UCommonButtonStyle> ButtonStyle = bIsBadged ? BadgedButtonStyle : UnbadgedButtonStyle;

	// Load the style and apply it to the button
	UAssetStreamerSubsystem::Get().LoadAssetAsync(ButtonStyle.ToSoftObjectPath(), FStreamableDelegate::CreateLambda([this, ButtonStyle]
	{
		SetStyle(ButtonStyle.Get());
		UpdateButtonStyle();
	}));
}

void UNodecraftBadgedButton::NativeConstruct()
{
	Super::NativeConstruct();
	SetIsBadged(false);
}
