// Fill out your copyright notice in the Description page of Project Settings.


#include "Models/ModerationLogEntryDataObject.h"

#include "Models/PlayerDataObject.h"

UModerationLogEntryDataObject* UModerationLogEntryDataObject::FromJson(const TSharedRef<FJsonObject> Json)
{
	UModerationLogEntryDataObject* ModerationLogEntryDataObject = NewObject<UModerationLogEntryDataObject>();

	ModerationLogEntryDataObject->Player = UPlayerDataObject::FromJson(Json->GetObjectField("player").ToSharedRef());

	// type = ban or kick. not used at the moment.
	ModerationLogEntryDataObject->Type = Json->GetStringField("type");

	const FString DateCreatedString = Json->GetStringField("date_created");
	FDateTime::ParseIso8601(*DateCreatedString, ModerationLogEntryDataObject->DateCreated);

	FString DateExpiresString;
	if (Json->TryGetStringField("date_expires", DateExpiresString))
	{
		FDateTime::ParseIso8601(*DateExpiresString, ModerationLogEntryDataObject->DateExpires);
	}
	ModerationLogEntryDataObject->ModerationDuration = ModerationLogEntryDataObject->DateExpires.GetTicks() == 0 ?
		EModerationDuration::Permanent : EModerationDuration::Temporary;
	
	return ModerationLogEntryDataObject;
}

UPlayerDataObject* UModerationLogEntryDataObject::GetPlayer() const
{
	return Player;
}

FDateTime UModerationLogEntryDataObject::GetDateCreated() const
{
	return DateCreated;
}

FDateTime UModerationLogEntryDataObject::GetDateExpires() const
{
	return DateExpires;
}

EModerationDuration UModerationLogEntryDataObject::GetModerationDuration() const
{
	return ModerationDuration;
}
