// Nodecraft, Inc. © 2012-2024, All Rights Reserved.


#include "UI/Alerts/AlertMessage.h"

#include "Engine/Texture2D.h"
#include "CommonBorder.h"
#include "CommonTextBlock.h"
#include "Components/Image.h"
#include "Subsystems/AssetStreamerSubsystem.h"

void UAlertMessage::Show(const FText& Message, const EAlertType AlertType)
{
	MessageText->SetText(Message);

	FAlertStyle Style = AlertStyles.FindChecked(AlertType);
	UAssetStreamerSubsystem::Get().LoadAssetAsync(Style.Icon.ToSoftObjectPath(), FStreamableDelegate::CreateWeakLambda(this, [this, Style]()
	{
		Icon->SetBrushFromTexture(Style.Icon.Get());
	}));
	UAssetStreamerSubsystem::Get().LoadAssetAsync(Style.OuterBorderStyle.ToSoftObjectPath(), FStreamableDelegate::CreateWeakLambda(this, [this, Style]()
	{
		OuterBorder->SetStyle(Style.OuterBorderStyle.Get());
	}));
	UAssetStreamerSubsystem::Get().LoadAssetAsync(Style.InnerBorderStyle.ToSoftObjectPath(), FStreamableDelegate::CreateWeakLambda(this, [this, Style]()
	{
		InnerBorder->SetStyle(Style.InnerBorderStyle.Get());
	}));
	
	SetVisibility(ESlateVisibility::HitTestInvisible);
}

void UAlertMessage::Hide()
{
	SetVisibility(ESlateVisibility::Collapsed);
}
