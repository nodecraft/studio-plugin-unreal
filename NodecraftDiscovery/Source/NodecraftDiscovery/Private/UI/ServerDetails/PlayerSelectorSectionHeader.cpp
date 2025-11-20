// Nodecraft, Inc. © 2012-2024, All Rights Reserved.


#include "UI/ServerDetails/PlayerSelectorSectionHeader.h"

#include "CommonUIRichTextData.h"
#include "Engine/Texture2D.h"
#include "Subsystems/AssetStreamerSubsystem.h"
#include "UI/ServerDetails/ModerationConsolePlayerList.h"

void UPlayerSelectorSectionHeader::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	if (UPlayerListSectionHeaderViewModel* ViewModel = Cast<UPlayerListSectionHeaderViewModel>(ListItemObject))
	{
		SectionHeaderTextBlock->SetText(ViewModel->HeaderText);
		if (ViewModel->IconData.Icon.IsNull() == false)
		{
			UAssetStreamerSubsystem::Get().LoadAssetAsync(ViewModel->IconData.Icon.ToSoftObjectPath(),
	  FStreamableDelegate::CreateWeakLambda(this, [this, ViewModel]()
			{
	  			if (ViewModel->IconData.Icon.IsValid())
	  			{
	  				SectionHeaderIcon->SetBrushFromTexture(ViewModel->IconData.Icon.Get());
	  				SectionHeaderIcon->SetColorAndOpacity(ViewModel->IconData.IconColor);
	  			}
			}));
		}
	}
}
