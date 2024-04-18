// Nodecraft, Inc. © 2012-2024, All Rights Reserved.


#include "Models/ServerRegionDataObject.h"

UServerRegionDataObject* UServerRegionDataObject::FromJson(const TSharedRef<FJsonObject>& Json)
{
	UServerRegionDataObject* ServerRegionDataObject = NewObject<UServerRegionDataObject>();

	ServerRegionDataObject->Title = Json->GetStringField("title");
	ServerRegionDataObject->ID = Json->GetStringField("id");

	return ServerRegionDataObject;
}

FText UServerRegionDataObject::GetTitle() const
{
	return FText::FromString(Title);
}

FString UServerRegionDataObject::GetID() const
{
	return ID;
}
