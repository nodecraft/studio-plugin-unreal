// Nodecraft, Inc. © 2012-2024, All Rights Reserved.


#include "UI/SocialLinks/SocialLinkItem.h"

#include "CommonTextBlock.h"
#include "Components/Image.h"
#include "DataTypes/LinkTypes.h"
#include "Models/SocialLinkDataObject.h"
#include "Services/LinksService.h"
#include "Subsystems/AssetStreamerSubsystem.h"
#include "Subsystems/MenuManagerSubsystem.h"
#include "UI/Foundation/NodecraftButtonBase.h"

void USocialLinkItem::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);

	const USocialLinkDataObject* SocialLink = Cast<USocialLinkDataObject>(ListItemObject);
	Label->SetText(SocialLink->GetLabel());
	URL = SocialLink->GetURL();

	if (const TSoftObjectPtr<UTexture2D> Icon = IconMap[SocialLink->GetLinkType()]; Icon.IsNull() == false)
	{
		UAssetStreamerSubsystem::Get().LoadAssetAsync(Icon.ToSoftObjectPath(),
			FStreamableDelegate::CreateWeakLambda(this, [this, Icon]
		{
			LinkIconLeft->SetBrushFromTexture(Icon.Get());
		}));
	}

	Button->OnClicked().AddWeakLambda(this, [this]
	{
		UMenuManagerSubsystem::Get().ShowRedirectModal(URL, ELinkType::External);
	});
}
