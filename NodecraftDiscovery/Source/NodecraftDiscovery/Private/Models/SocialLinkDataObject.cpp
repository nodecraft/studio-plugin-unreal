// Nodecraft, Inc. © 2012-2024, All Rights Reserved.


#include "Models/SocialLinkDataObject.h"

#define LOCTEXT_NAMESPACE "SocialLink"

USocialLinkDataObject* USocialLinkDataObject::FromJson(const TSharedRef<FJsonObject> Json, ESocialLinkType InLinkType)
{
	USocialLinkDataObject* SocialLink = NewObject<USocialLinkDataObject>();
	
	SocialLink->LinkType = InLinkType;

	switch (SocialLink->LinkType)
	{
	case ESocialLinkType::Twitter:
		if (Json->TryGetStringField(TEXT("social_twitter"), SocialLink->URL))
		{
			SocialLink->Label = LOCTEXT("ServerTwitterPageButtonText", "Twitter");
		}
		break;
	case ESocialLinkType::YouTube:
		if (Json->TryGetStringField(TEXT("social_youtube"), SocialLink->URL))
		{
			SocialLink->Label = LOCTEXT("ServerYouTubePageButtonText", "YouTube");
		}
		break;
	case ESocialLinkType::Twitch:
		if (Json->TryGetStringField(TEXT("social_twitch"), SocialLink->URL))
		{
			SocialLink->Label = LOCTEXT("ServerTwitchPageButtonText", "Twitch");
		}
		break;
	case ESocialLinkType::Discord:
		if (Json->TryGetStringField(TEXT("social_discord"), SocialLink->URL))
		{
			SocialLink->Label = LOCTEXT("ServerDiscordPageButtonText", "Discord");
		}
		break;
	case ESocialLinkType::Store:
		if (Json->TryGetStringField(TEXT("social_store"), SocialLink->URL))
		{
			SocialLink->Label = LOCTEXT("ServerStorePageButtonText", "Store");
		}
		break;
	case ESocialLinkType::Website:
		if (Json->TryGetStringField(TEXT("social_website"), SocialLink->URL))
		{
			SocialLink->Label = LOCTEXT("ServerWebsitePageButtonText", "Website");
		}
		break;
	default:
		SocialLink->URL = "";
		SocialLink->Label = FText::FromString(SocialLink->URL);
		break;
	}
	
	return SocialLink;
}

USocialLinkDataObject* USocialLinkDataObject::Create(ESocialLinkType Type, FString URL, FText Label /* Optional */)
{
	USocialLinkDataObject* SocialLink = NewObject<USocialLinkDataObject>();
	SocialLink->LinkType = Type;
	SocialLink->URL = URL;
	SocialLink->Label = Label.IsEmpty() ? FText::FromString(URL) : Label;
	return SocialLink;
}

ESocialLinkType USocialLinkDataObject::GetLinkType() const
{
	return LinkType;
}

FString USocialLinkDataObject::GetURL() const
{
	return URL;
}

FText USocialLinkDataObject::GetLabel() const
{
	return Label;
}

#undef LOCTEXT_NAMESPACE