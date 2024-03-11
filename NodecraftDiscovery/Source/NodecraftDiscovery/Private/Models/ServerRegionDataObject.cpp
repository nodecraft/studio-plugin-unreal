// Fill out your copyright notice in the Description page of Project Settings.


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
