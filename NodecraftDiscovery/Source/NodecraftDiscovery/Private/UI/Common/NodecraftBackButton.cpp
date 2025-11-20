// Nodecraft, Inc. © 2012-2024, All Rights Reserved.


#include "UI/Common/NodecraftBackButton.h"

#include "CommonTextBlock.h"
#include "Components/Image.h"
#include "Subsystems/AssetStreamerSubsystem.h"
#include "Subsystems/NodecraftUIManagerSubsystem.h"
#include "UI/Foundation/NodecraftButtonBase.h"

void UNodecraftBackButton::NativeConstruct()
{
	Super::NativeConstruct();

	Button->OnClicked().AddWeakLambda(this, [this]
	{
		UNodecraftUIManagerSubsystem::Get(GetGameInstance())->CloseNodecraftUI(true);
	});
	
	Button->OnHovered().AddWeakLambda(this, [this]
	{
		if (HoveredStyle.TextStyle.IsNull() == false)
		{
			UAssetStreamerSubsystem::Get().LoadAssetAsync(HoveredStyle.TextStyle.ToSoftObjectPath(),
				FStreamableDelegate::CreateWeakLambda(this, [this]
			{
				Text->SetStyle(HoveredStyle.TextStyle.Get());
			}));
		}
		BackgroundCircle->SetBrushFromMaterial(HoveredStyle.Material);
		BackArrow->SetBrushTintColor(HoveredStyle.Tint);
	});
	
	Button->OnUnhovered().AddWeakLambda(this, [this]
	{
		if (NormalStyle.TextStyle.IsNull() == false)
		{
			UAssetStreamerSubsystem::Get().LoadAssetAsync(NormalStyle.TextStyle.ToSoftObjectPath(),
				FStreamableDelegate::CreateWeakLambda(this, [this]
			{
				Text->SetStyle(NormalStyle.TextStyle.Get());
			}));
		}
		BackgroundCircle->SetBrushFromMaterial(NormalStyle.Material);
		BackArrow->SetBrushTintColor(NormalStyle.Tint);
	});
}
