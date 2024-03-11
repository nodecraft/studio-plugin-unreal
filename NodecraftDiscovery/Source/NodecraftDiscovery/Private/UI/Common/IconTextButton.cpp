// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Common/IconTextButton.h"

#include "CommonTextBlock.h"
#include "Components/Image.h"
#include "Subsystems/AssetStreamerSubsystem.h"

void UIconTextButton::NativePreConstruct()
{
	Super::NativePreConstruct();
	if (Icon == nullptr || Text == nullptr)
	{
		return;
	}
	if (bShowIcon)
	{
		if (IconImage.IsNull() == false)
		{
			FStreamableDelegate OnLoaded;
			OnLoaded.BindWeakLambda(this, [this]
			{
				Icon->SetBrushFromTexture(IconImage.Get());
			});
			UAssetStreamerSubsystem::Get().LoadAssetAsync(IconImage.ToSoftObjectPath(), OnLoaded);
		}
		else
		{
			Icon->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
	else
	{
		Icon->SetVisibility(ESlateVisibility::Collapsed);
	}
	Text->SetText(GetButtonText());
}
