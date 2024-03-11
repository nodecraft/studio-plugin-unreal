// Fill out your copyright notice in the Description page of Project Settings.


#include "Models/AccountReputationEntryDataObject.h"

#include "DataTypes/ModerationTypes.h"
#include "Models/ServerDataObject.h"

UAccountReputationEntryDataObject* UAccountReputationEntryDataObject::FromJson(const TSharedRef<FJsonObject>& Json)
{
	UAccountReputationEntryDataObject* AccountReputationEntryDataObject = NewObject<UAccountReputationEntryDataObject>();

	const TSharedPtr<FJsonObject>* Server;
	if (Json->TryGetObjectField("server", Server))
	{
		AccountReputationEntryDataObject->ServerDataObject = UServerDataObject::FromJson(Server->ToSharedRef());
	}

	const TSharedPtr<FJsonObject>* Community;
	if (Json->TryGetObjectField("community", Community))
	{
		AccountReputationEntryDataObject->CommunityDataObject = UCommunityDataObject::FromJson(Community->ToSharedRef());
	}
	
	const FString DateCreatedString = Json->GetStringField("date_created");
	FDateTime::ParseIso8601(*DateCreatedString, AccountReputationEntryDataObject->DateCreated);

	FString DateExpiresString;
	if (Json->TryGetStringField("date_expires", DateExpiresString))
	{
		FDateTime::ParseIso8601(*DateExpiresString, AccountReputationEntryDataObject->DateExpires);
	}

	AccountReputationEntryDataObject->ModerationDuration = AccountReputationEntryDataObject->DateExpires.GetTicks() == 0 ?
		EModerationDuration::Permanent : EModerationDuration::Temporary;
	if (AccountReputationEntryDataObject->ModerationDuration == EModerationDuration::Temporary &&
		AccountReputationEntryDataObject->DateExpires < FDateTime::UtcNow())
	{
		AccountReputationEntryDataObject->ModerationDuration = EModerationDuration::Expired;
	}

	const FString Scope = Json->GetStringField("scope");
	if (Scope.Equals("server", ESearchCase::IgnoreCase))
	{
		AccountReputationEntryDataObject->ModerationScope = EModerationScope::Server;
	}
	else if (Scope.Equals("community", ESearchCase::IgnoreCase))
	{
		AccountReputationEntryDataObject->ModerationScope = EModerationScope::Community;
	}
	else if (Scope.Equals("game", ESearchCase::IgnoreCase))
	{
		AccountReputationEntryDataObject->ModerationScope = EModerationScope::Game;
	}
	else if (Scope.Equals("global", ESearchCase::IgnoreCase))
	{
		AccountReputationEntryDataObject->ModerationScope = EModerationScope::Global;
	}
	
	return AccountReputationEntryDataObject;
}

UServerDataObject* UAccountReputationEntryDataObject::GetServer() const
{
	return ServerDataObject;
}

UCommunityDataObject* UAccountReputationEntryDataObject::GetCommunity() const
{
	return CommunityDataObject;
}

FDateTime UAccountReputationEntryDataObject::GetDateCreated() const
{
	return DateCreated;
}

FDateTime UAccountReputationEntryDataObject::GetDateExpires() const
{
	return DateExpires;
}

EModerationDuration UAccountReputationEntryDataObject::GetModerationDuration() const
{
	return ModerationDuration;
}

EModerationScope UAccountReputationEntryDataObject::GetModerationScope() const
{
	return ModerationScope;
}
