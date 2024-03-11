// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SocialLinks/SocialLinkItem.h"

#include "CommonTextBlock.h"
#include "Components/Image.h"
#include "Models/SocialLinkDataObject.h"
#include "Subsystems/AssetStreamerSubsystem.h"

void USocialLinkItem::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);

	const USocialLinkDataObject* SocialLink = Cast<USocialLinkDataObject>(ListItemObject);
	Label->SetText(SocialLink->GetLabel());
	URL = SocialLink->GetURL();

	if (const TSoftObjectPtr<UTexture2D> Icon = IconMap[SocialLink->GetLinkType()]; Icon.IsNull() == false)
	{
		UAssetStreamerSubsystem::Get().LoadAssetAsync(Icon.ToSoftObjectPath(), FStreamableDelegate::CreateLambda([this, Icon]
			{
				LinkIconLeft->SetBrushFromTexture(Icon.Get());
			}));
	}
}
