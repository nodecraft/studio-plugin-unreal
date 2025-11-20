// Nodecraft, Inc. © 2012-2024, All Rights Reserved.


#include "Models/ModerationLogEntryDataObject.h"

#include "NodecraftLogCategories.h"
#include "Models/PlayerDataObject.h"

UModerationLogEntryDataObject* UModerationLogEntryDataObject::FromJson(const TSharedRef<FJsonObject> Json)
{
	UModerationLogEntryDataObject* ModerationLogEntryDataObject = NewObject<UModerationLogEntryDataObject>();

	ModerationLogEntryDataObject->Player = UPlayerDataObject::FromJson(Json->GetObjectField(TEXT("player")).ToSharedRef());

	// type = ban or kick
	if (const FString Type = Json->GetStringField(TEXT("type")); Type.Equals("ban", ESearchCase::IgnoreCase))
	{
		ModerationLogEntryDataObject->Type = EModerationAction::Ban;
	}
	else if (Type.Equals("kick", ESearchCase::IgnoreCase))
	{
		ModerationLogEntryDataObject->Type = EModerationAction::Kick;
	}
	else
	{
		ModerationLogEntryDataObject->Type = EModerationAction::Undefined;
		UE_LOG(LogNodecraft_Moderation, Verbose, TEXT("UModerationLogEntryDataObject::FromJson: Unknown moderation action type for moderation log entry."));
	}
	
	ModerationLogEntryDataObject->Reason = Json->GetStringField(TEXT("reason"));

	FString DateCreatedString;
	if (Json->TryGetStringField(TEXT("date_created"), DateCreatedString))
	{
		FDateTime::ParseIso8601(*DateCreatedString, ModerationLogEntryDataObject->DateCreated);
	}
	
	FString DateExpiresString;
	if (Json->TryGetStringField(TEXT("date_expires"), DateExpiresString))
	{
		FDateTime::ParseIso8601(*DateExpiresString, ModerationLogEntryDataObject->DateExpires);
	}

	if (ModerationLogEntryDataObject->DateExpires.GetTicks() == 0)
	{
		ModerationLogEntryDataObject->ModerationDuration = EModerationDuration::Permanent;
	}
	else if (ModerationLogEntryDataObject->DateExpires <= FDateTime::UtcNow())
	{
		ModerationLogEntryDataObject->ModerationDuration = EModerationDuration::Expired;
	}
	else if (ModerationLogEntryDataObject->DateExpires > FDateTime::UtcNow())
	{
		ModerationLogEntryDataObject->ModerationDuration = EModerationDuration::Temporary;
	}
	else
	{
		ModerationLogEntryDataObject->ModerationDuration = EModerationDuration::Undefined;
		UE_LOG(LogNodecraft_Moderation, Verbose, TEXT("UModerationLogEntryDataObject::FromJson: Unknown moderation duration for moderation log entry."));
	}
	
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

EModerationAction UModerationLogEntryDataObject::GetType() const
{
	return Type;
}

FString UModerationLogEntryDataObject::GetReason() const
{
	return Reason;
}
