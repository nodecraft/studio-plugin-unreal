﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "Models/ExpirableDataObject.h"

FString FExpirableDataObject::GetId() const
{
	return Id;
}

FDateTime FExpirableDataObject::GetDateExpires() const
{
	return DateExpires;
}

bool FExpirableDataObject::Expires() const
{
	return bExpires;
}

FExpirableDataObject FExpirableDataObject::FromJson(const TSharedRef<FJsonObject>& Json)
{
	FExpirableDataObject ServerAllowDataObject;

	ServerAllowDataObject.Id = Json->GetStringField("id");

	if (FString DateExpiresString; Json->TryGetStringField("date_expires", DateExpiresString))
	{
		FDateTime::ParseIso8601(*DateExpiresString, ServerAllowDataObject.DateExpires);
		ServerAllowDataObject.bExpires = true;
	}
	
	return ServerAllowDataObject;
}
