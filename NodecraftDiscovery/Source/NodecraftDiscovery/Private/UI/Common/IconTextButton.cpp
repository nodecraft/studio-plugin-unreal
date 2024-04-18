// Nodecraft, Inc. © 2012-2024, All Rights Reserved.


#include "UI/Common/IconTextButton.h"

#include "CommonTextBlock.h"
#include "Components/HorizontalBoxSlot.h"
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
				if (Text->GetText().IsEmpty())
				{
					if (UHorizontalBoxSlot* Slot = Cast<UHorizontalBoxSlot>(Icon->Slot))
					{
						Slot->SetPadding(FMargin(0));
					}
				}
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
