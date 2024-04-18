// Nodecraft, Inc. © 2012-2024, All Rights Reserved.


#include "Models/KickDataObject.h"

FString FPlayerKickDataObject::GetKickedPlayerId() const
{
	return Player ? Player->GetId() : FString();
}

FPlayerKickDataObject FPlayerKickDataObject::FromJson(const TSharedRef<FJsonObject>& Json)
{
	FPlayerKickDataObject KickDataObject = FPlayerKickDataObject();

	// parse id
	KickDataObject.Id = Json->GetStringField("id");

	// parse date created
	FString DateString;
	if (Json->TryGetStringField("date_created", DateString))
	{
		FDateTime::ParseIso8601(*DateString, KickDataObject.DateCreated);
	}

	// parse reason
	FString ReasonStr;
	Json->TryGetStringField("reason", ReasonStr);
	if (ReasonStr.IsEmpty() == false)
	{
		KickDataObject.Reason = FText::FromString(ReasonStr);
	}
	
	// parse player
	if (const TSharedPtr<FJsonObject>* PlayerObject; Json->TryGetObjectField("player", PlayerObject))
	{
		KickDataObject.Player = UPlayerDataObject::FromJson(PlayerObject->ToSharedRef());
	}
	return KickDataObject;
}












