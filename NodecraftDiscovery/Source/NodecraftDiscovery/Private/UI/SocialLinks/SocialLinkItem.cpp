// Nodecraft, Inc. © 2012-2024, All Rights Reserved.


#include "UI/SocialLinks/SocialLinkItem.h"

#include "Engine/Texture2D.h"
#include "CommonInputSubsystem.h"
#include "CommonTextBlock.h"
#include "Components/Image.h"
#include "DataTypes/LinkTypes.h"
#include "Models/SocialLinkDataObject.h"
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

	PrimaryActionButton->OnClicked().AddWeakLambda(this, [this]
	{
		UMenuManagerSubsystem::Get().ShowRedirectModal(URL, ELinkType::External);
	});
}

FNavigationReply USocialLinkItem::NativeOnNavigation(const FGeometry& MyGeometry,
	const FNavigationEvent& InNavigationEvent, const FNavigationReply& InDefaultReply)
{
	if (OnNavDelegate.IsBound())
	{
		return OnNavDelegate.Execute(MyGeometry, InNavigationEvent, InDefaultReply);
	}
	
	return Super::NativeOnNavigation(MyGeometry, InNavigationEvent, InDefaultReply);
}

FReply USocialLinkItem::NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent)
{
	if (const ULocalPlayer* LocalPlayer = GetOwningLocalPlayer())
	{
		if (UCommonInputSubsystem::Get(LocalPlayer)->GetCurrentInputType() == ECommonInputType::Gamepad)
		{
			RegisterActionBinding(EUIActionBindingType::Primary);
		}
	}
	return Super::NativeOnFocusReceived(InGeometry, InFocusEvent);
}

void USocialLinkItem::NativeOnFocusLost(const FFocusEvent& InFocusEvent)
{
	UnregisterUIActionBindings();
	Super::NativeOnFocusLost(InFocusEvent);
}
