// Nodecraft, Inc. © 2012-2024, All Rights Reserved.


#include "UI/Common/TagButton.h"

#include "Services/ServersService.h"
#include "Subsystems/AssetStreamerSubsystem.h"
#include "Subsystems/MenuManagerSubsystem.h"

void UTagButton::Configure(UServerDataObject* ServerDataObject, const FString& TagText, const ETagType TagType)
{
	SetButtonText(FText::FromString(TagText));
	switch (TagType)
	{
	case ETagType::TagButton:
		SetStyle(Style);
		break;
	case ETagType::TagText:
		if (TextTagButtonStyle.IsNull() == false)
		{
			UAssetStreamerSubsystem::Get().LoadAssetAsync(TextTagButtonStyle.ToSoftObjectPath(),
				FStreamableDelegate::CreateWeakLambda(this, [this]
			{
				SetStyle(TextTagButtonStyle.Get());
			}));
		}
		break;
	case ETagType::TagExtra:
		if (ExtraTagButtonStyle.IsNull() == false)
		{
			UAssetStreamerSubsystem::Get().LoadAssetAsync(ExtraTagButtonStyle.ToSoftObjectPath(),
				FStreamableDelegate::CreateWeakLambda(this, [this]
			{
				SetStyle(ExtraTagButtonStyle.Get());
			}));
		}
		break;
	default:
		SetStyle(Style);
		break;
	}

	OnClicked().Clear();
	OnClicked().AddWeakLambda(this, [this, TagType, ServerDataObject]
   {
		if (TagType == ETagType::TagExtra)
		{
			TMap<FString, FText> Argument;
			Argument.Add("Tags", FText::GetEmpty());
			UMenuManagerSubsystem::Get().ShowServerDetails(ServerDataObject, Argument);
		}
		else
		{
			UServersService::Get().SearchServers(GetWorld(), GetButtonText().ToString(), 1, false);
			UMenuManagerSubsystem::Get().ClearPopupStack();
		}
   });
}

FNavigationReply UTagButton::NativeOnNavigation(const FGeometry& MyGeometry, const FNavigationEvent& InNavigationEvent,
	const FNavigationReply& InDefaultReply)
{
	if (OnNavDelegate.IsBound())
	{
		return OnNavDelegate.Execute(MyGeometry, InNavigationEvent, InDefaultReply);
	}

	if (InNavigationEvent.GetNavigationType() == EUINavigation::Right || InNavigationEvent.GetNavigationType() == EUINavigation::Left)
	{
		return FNavigationReply::Escape();
	}
	
	return Super::NativeOnNavigation(MyGeometry, InNavigationEvent, InDefaultReply);
}
