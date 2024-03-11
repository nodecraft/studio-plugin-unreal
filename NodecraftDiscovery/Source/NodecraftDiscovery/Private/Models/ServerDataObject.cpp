// Fill out your copyright notice in the Description page of Project Settings.


#include "Models/ServerDataObject.h"

#include "Models/SocialLinkDataObject.h"

FString UServerDataObject::GetId() const
{
	return Id;
}

EServerType UServerDataObject::GetType() const
{
	return Type;
}

FText UServerDataObject::GetTitle() const
{
	return FText::FromString(Title);
}

FText UServerDataObject::GetSummary() const
{
	return FText::FromString(Summary);
}

FString UServerDataObject::GetImageUrl() const
{
	return ImageUrl;
}

FString UServerDataObject::GetImageFilenameForCache() const
{
	return Id + "_" + "img";
}

FText UServerDataObject::GetRegionTitle() const
{
	return RegionTitle;
}

FText UServerDataObject::GetGameVersion() const
{
	return GameVersion;
}

int32 UServerDataObject::GetPlayersCount() const
{
	return PlayersCount;
}

int32 UServerDataObject::GetPlayersMax() const
{
	return PlayersMax;
}

bool UServerDataObject::IsFavorite() const
{
	return bIsFavorite;
}

bool UServerDataObject::HasPassword() const
{
	return bHasPassword;
}

TArray<FString> UServerDataObject::GetTags() const
{
	return Tags;
}

TArray<USocialLinkDataObject*> UServerDataObject::GetSocialLinks() const
{
	return SocialLinks;
}

UCommunityDataObject* UServerDataObject::GetCommunity() const
{
	return Community;
}

void UServerDataObject::SetRules(URulesDataObject* InRules)
{
	Rules = InRules;
}

void UServerDataObject::SetIsFavorite(bool InIsFavorite)
{
	bIsFavorite = InIsFavorite;
}

void UServerDataObject::UpdateRules(URulesDataObject* InRules)
{
	if (InRules)
	{
		if (Rules == nullptr)
		{
			Rules = InRules;
		}
		else
		{
			Rules->Update(InRules);
		}
	}
}

UServerDataObject* UServerDataObject::FromJson(const TSharedRef<FJsonObject> Json)
{
	// Generate a new ServerDataObject from Json
	UServerDataObject* ServerDataObject = NewObject<UServerDataObject>();
	ServerDataObject->Id = Json->GetStringField("id");
	FString ServerType;
	FString OwnerType;

	ServerDataObject->Type = EServerType::Unknown; // default
	if (Json->TryGetStringField("type", ServerType))
	{
		if (ServerType == "community")
		{
			ServerDataObject->Type = EServerType::Community;
		}
		else if (ServerType == "private")
		{
			ServerDataObject->Type = EServerType::Private;
		}
	}

	ServerDataObject->Role = EPlayerRole::Unknown; // default
	if (Json->TryGetStringField("role", OwnerType))
	{
		if (OwnerType == "owner")
		{
			ServerDataObject->Role = EPlayerRole::Owner;
		}
		else if (OwnerType == "moderator")
		{
			ServerDataObject->Role = EPlayerRole::Moderator;
		}
		else if (OwnerType == "player")
		{
			ServerDataObject->Role = EPlayerRole::Player;
		}
	}
	
	ServerDataObject->Title = Json->GetStringField("title");
	Json->TryGetStringField("summary", ServerDataObject->Summary);
	Json->TryGetStringField("image_tile", ServerDataObject->ImageUrl);
	Json->TryGetNumberField("players_count", ServerDataObject->PlayersCount);
	Json->TryGetNumberField("players_max", ServerDataObject->PlayersMax);
	Json->TryGetBoolField("is_favorite", ServerDataObject->bIsFavorite);
	Json->TryGetBoolField("has_password", ServerDataObject->bHasPassword);
	
	FString Version;
	Json->TryGetStringField("game_version", Version);
	ServerDataObject->GameVersion = FText::FromString(Version);

	const TSharedPtr<FJsonObject>* RegionData;
	if(Json->TryGetObjectField("server_region", RegionData))
	{
		FString RegionTitle;
		if (RegionData->Get()->TryGetStringField("title", RegionTitle))
		{
			ServerDataObject->RegionTitle = FText::FromString(RegionTitle);
		}
	}

	Json->TryGetNumberField("friend_playing", ServerDataObject->NumFriendsPlaying);
	Json->TryGetNumberField("friend_recently_played", ServerDataObject->NumFriendsRecentlyPlayed);


	// parse community data
	const TSharedPtr<FJsonObject>* CommunityJson;
	if (Json->TryGetObjectField("community", CommunityJson))
	{
		ServerDataObject->Community = UCommunityDataObject::FromJson(CommunityJson->ToSharedRef());
	}
	
	const TSharedPtr<FJsonObject>* PlayerData;
	if (Json->TryGetObjectField("player", PlayerData))
	{
		ServerDataObject->PlayerDataObject = UPlayerDataObject::FromJson(PlayerData->ToSharedRef());
	}


	const TArray<TSharedPtr<FJsonValue>>* TagList;
	if (Json->TryGetArrayField("tags", TagList))
	{
		for (const TSharedPtr<FJsonValue> Tag : *TagList)
		{
			ServerDataObject->Tags.Add(Tag->AsString());
		}
	}

	// parse social links
	if (USocialLinkDataObject* Link = USocialLinkDataObject::FromJson(Json, ESocialLinkType::Twitter); Link->GetURL().IsEmpty() == false)
	{
		ServerDataObject->SocialLinks.Add(Link);
	}
	if (USocialLinkDataObject* Link = USocialLinkDataObject::FromJson(Json, ESocialLinkType::YouTube); Link->GetURL().IsEmpty() == false)
	{
		ServerDataObject->SocialLinks.Add(Link);
	}
	if (USocialLinkDataObject* Link = USocialLinkDataObject::FromJson(Json, ESocialLinkType::Twitch); Link->GetURL().IsEmpty() == false)
	{
		ServerDataObject->SocialLinks.Add(Link);
	}
	if (USocialLinkDataObject* Link = USocialLinkDataObject::FromJson(Json, ESocialLinkType::Discord); Link->GetURL().IsEmpty() == false)
	{
		ServerDataObject->SocialLinks.Add(Link);
	}
	if (USocialLinkDataObject* Link = USocialLinkDataObject::FromJson(Json, ESocialLinkType::Store); Link->GetURL().IsEmpty() == false)
	{
		ServerDataObject->SocialLinks.Add(Link);
	}
	if (USocialLinkDataObject* Link = USocialLinkDataObject::FromJson(Json, ESocialLinkType::Website); Link->GetURL().IsEmpty() == false)
	{
		ServerDataObject->SocialLinks.Add(Link);
	}

	const TArray<TSharedPtr<FJsonValue>>* SocialCustom;
	if (Json->TryGetArrayField("social_custom", SocialCustom))
	{
		for (const TSharedPtr<FJsonValue> Value : *SocialCustom)
		{
			USocialLinkDataObject* Link = USocialLinkDataObject::Create(ESocialLinkType::Custom, Value->AsString());
			ServerDataObject->SocialLinks.Add(Link);
		}
	}
	
	return ServerDataObject;
}

EPlayerRole UServerDataObject::GetRole() const
{
	return Role;
}

URulesDataObject* UServerDataObject::GetRules() const
{
	return Rules;
}

UPlayerDataObject* UServerDataObject::GetPlayer() const
{
	return PlayerDataObject;
}
