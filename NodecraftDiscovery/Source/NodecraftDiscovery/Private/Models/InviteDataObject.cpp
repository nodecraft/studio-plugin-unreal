// Nodecraft, Inc. © 2012-2024, All Rights Reserved.


#include "Models/InviteDataObject.h"

#include "Models/InviteStatus.h"
#include "Models/ServerDataObject.h"

UInviteDataObject* UInviteDataObject::FromJson(const TSharedRef<FJsonObject>& Json)
{
	UInviteDataObject* InviteDataObject = NewObject<UInviteDataObject>();

	InviteDataObject->ID = Json->GetStringField("id");

	FString DateCreatedString;
	if (Json->TryGetStringField("date_created", DateCreatedString))
	{
		FDateTime::ParseIso8601(*DateCreatedString, InviteDataObject->DateCreated);
	}
	
	FString DateExpiresString;
	Json->TryGetStringField("date_expires", DateExpiresString);
	FDateTime::ParseIso8601(*DateExpiresString, InviteDataObject->DateExpires);

	FString StatusString;
	if (Json->TryGetStringField("status", StatusString))
	{
		if (StatusString.Equals("pending", ESearchCase::IgnoreCase))
		{
			InviteDataObject->Status = ENodecraftInviteStatus::Pending;
		}
		else if (StatusString.Equals("accepted", ESearchCase::IgnoreCase))
		{
			InviteDataObject->Status = ENodecraftInviteStatus::Accepted;
		}
		else if (StatusString.Equals("declined", ESearchCase::IgnoreCase))
		{
			InviteDataObject->Status = ENodecraftInviteStatus::Declined;
		}
		else if (StatusString.Equals("revoked", ESearchCase::IgnoreCase))
		{
			InviteDataObject->Status = ENodecraftInviteStatus::Revoked;
		}
	}

	if (const TSharedPtr<FJsonObject>* ServerObject; Json->TryGetObjectField("server", ServerObject))
	{
		InviteDataObject->Server = UServerDataObject::FromJson(ServerObject->ToSharedRef());
	}
	if (const TSharedPtr<FJsonObject>* PlayerObject; Json->TryGetObjectField("player", PlayerObject))
	{
		InviteDataObject->Player = UPlayerDataObject::FromJson(PlayerObject->ToSharedRef());
	}
	
	return InviteDataObject;
}
