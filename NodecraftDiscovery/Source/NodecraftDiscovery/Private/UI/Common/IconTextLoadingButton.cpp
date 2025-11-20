// Nodecraft, Inc. © 2012-2024, All Rights Reserved.


#include "UI/Common/IconTextLoadingButton.h"

#include "Engine/Texture2D.h"
#include "CommonTextBlock.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/Image.h"
#include "Components/OverlaySlot.h"
#include "Engine/StreamableManager.h"
#include "Subsystems/AssetStreamerSubsystem.h"


void UIconTextLoadingButton::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (bShowIcon)
	{
		if (bIconOnRight)
		{
			// swap the 2 widgets inside IconTextContainer box
			IconTextContainer->ClearChildren();
			IconTextContainer->AddChild(Text);
			IconTextContainer->AddChild(Icon);
			if (UHorizontalBoxSlot* IconSlot = Cast<UHorizontalBoxSlot>(Icon->Slot))
			{
				// swap left and right padding
				IconSlot->SetPadding(FMargin(IconPadding.Right, IconPadding.Top, IconPadding.Left, IconPadding.Bottom));
			}
		}
		else
		{
			// default order
			IconTextContainer->ClearChildren();
			IconTextContainer->AddChild(Icon);
			IconTextContainer->AddChild(Text);
			if (UHorizontalBoxSlot* IconSlot = Cast<UHorizontalBoxSlot>(Icon->Slot))
			{
				IconSlot->SetPadding(IconPadding);
			}
		}
		
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

	// setup alignments
	if (UHorizontalBoxSlot* IconSlot = Cast<UHorizontalBoxSlot>(Icon->Slot); IconSlot && Icon->IsVisible())
	{
		IconSlot->SetHorizontalAlignment(IconHorizontalAlignment);
		IconSlot->SetVerticalAlignment(IconVerticalAlignment);
	}
	if (UHorizontalBoxSlot* TextSlot = Cast<UHorizontalBoxSlot>(Text->Slot))
	{
		TextSlot->SetHorizontalAlignment(TextHorizontalAlignment);
		TextSlot->SetVerticalAlignment(TextVerticalAlignment);
	}
	if (UOverlaySlot* ContainerSlot = Cast<UOverlaySlot>(IconTextContainer->Slot))
	{
		ContainerSlot->SetHorizontalAlignment(ContainerHorizontalAlignment);
		ContainerSlot->SetVerticalAlignment(ContainerVerticalAlignment);
	}
}
