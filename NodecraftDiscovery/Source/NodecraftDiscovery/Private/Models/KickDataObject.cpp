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
	KickDataObject.Id = Json->GetStringField(TEXT("id"));

	// parse date created
	FString DateString;
	if (Json->TryGetStringField(TEXT("date_created"), DateString))
	{
		FDateTime::ParseIso8601(*DateString, KickDataObject.DateCreated);
	}

	// parse reason
	FString ReasonStr;
	Json->TryGetStringField(TEXT("reason"), ReasonStr);
	if (ReasonStr.IsEmpty() == false)
	{
		KickDataObject.Reason = FText::FromString(ReasonStr);
	}
	
	// parse player
	if (const TSharedPtr<FJsonObject>* PlayerObject; Json->TryGetObjectField(TEXT("player"), PlayerObject))
	{
		KickDataObject.Player = UPlayerDataObject::FromJson(PlayerObject->ToSharedRef());
	}
	return KickDataObject;
}












