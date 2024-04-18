// Nodecraft, Inc. © 2012-2024, All Rights Reserved.


#include "Models/PlayerConnectionDataObject.h"

#include "DataTypes/PlayerConnectionStatus.h"

UPlayerConnectionDataObject* UPlayerConnectionDataObject::FromJson(const TSharedRef<FJsonObject>& Json)
{
	UPlayerConnectionDataObject* PlayerConnectionDataObject = NewObject<UPlayerConnectionDataObject>();

	PlayerConnectionDataObject->Id = Json->GetStringField("id");
	PlayerConnectionDataObject->Url = Json->GetStringField("url");

	const FString StatusString = Json->GetStringField("status");
	PlayerConnectionDataObject->StatusRaw = StatusString;
	PlayerConnectionDataObject->Status = EPlayerConnectionStatusHelpers::FromString(StatusString);
	

	PlayerConnectionDataObject->DateExpires = FDateTime();
	if (FString DateExpiresString; Json->TryGetStringField("date_expires", DateExpiresString))
	{
		FDateTime::ParseIso8601(*DateExpiresString, PlayerConnectionDataObject->DateExpires);
	}

	return PlayerConnectionDataObject;
}

FString UPlayerConnectionDataObject::GetId() const
{
	return Id;
}

FString UPlayerConnectionDataObject::GetUrl() const
{
	return Url;
}

FString UPlayerConnectionDataObject::GetConnectionStatusRaw() const
{
	return StatusRaw;
}

EPlayerConnectionStatus UPlayerConnectionDataObject::GetConnectionStatus() const
{
	return Status;
}

FDateTime UPlayerConnectionDataObject::GetDateExpires() const
{
	return DateExpires;
}
