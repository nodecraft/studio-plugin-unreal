// Fill out your copyright notice in the Description page of Project Settings.


#include "Models/PlayerDataObject.h"

#include "DataTypes/IdentityType.h"


DEFINE_LOG_CATEGORY_STATIC(LogPlayerDataObject, All, All);

FString UPlayerDataObject::GetId() const
{
	return Id;
}

FString UPlayerDataObject::GetUsername() const
{
	return Username;
}

FString UPlayerDataObject::GetIdent() const
{
	return Ident;
}

EIdentityType UPlayerDataObject::GetIdentType() const
{
	if (IdentType.Equals("steam", ESearchCase::IgnoreCase))
    {
    	return EIdentityType::Steam;
    }
	if (IdentType.Equals("xbox", ESearchCase::IgnoreCase))
	{
		return EIdentityType::Xbox;
	}
	if (IdentType.Equals("PSN", ESearchCase::IgnoreCase))
	{
		return EIdentityType::PSN;
	}
	if (IdentType.Equals("Nintendo", ESearchCase::IgnoreCase))
	{
		return EIdentityType::Nintendo;
	}
	if (IdentType.Equals("Android", ESearchCase::IgnoreCase))
	{
		return EIdentityType::Android;
	}
	if (IdentType.Equals("Apple", ESearchCase::IgnoreCase))
	{
		return EIdentityType::Apple;
	}
	if (IdentType.Equals("sms", ESearchCase::IgnoreCase))
	{
		return EIdentityType::SMS;
	}
	if (IdentType.Equals("email", ESearchCase::IgnoreCase))
	{
		return EIdentityType::Email;
	}
	return EIdentityType::Email;
}

FString UPlayerDataObject::GetImageAvatarUrl() const
{
	return ImageUrl;
}

FString UPlayerDataObject::GetImageAvatarFilenameForCache() const
{
	return Id + "_" + "img";
}

bool UPlayerDataObject::IsOnline() const
{
	return DateStart.IsEmpty() == false;
}

FDateTime UPlayerDataObject::GetDateStart() const
{
	FDateTime Datetime;
	const bool bSuccess = FDateTime::ParseIso8601(*DateStart, Datetime);
	if (bSuccess == false)
	{
		UE_LOG(LogPlayerDataObject, Error, TEXT("UPlayerDataObject::GetDateStart(): Failed to parse date: %s"), *DateStart);
	}
	return Datetime;
}

FDateTime UPlayerDataObject::GetDateEnd() const
{
	FDateTime Datetime;
	const bool bSuccess = FDateTime::ParseIso8601(*DateEnd, Datetime);
	if (bSuccess == false)
	{
		UE_LOG(LogPlayerDataObject, Error, TEXT("UPlayerDataObject::GetDateEnd(): Failed to parse date: %s"), *DateEnd);
	}
	return Datetime;
}

FDateTime UPlayerDataObject::GetDateFirstJoined() const
{
	FDateTime DateJoined;
	const bool bSuccess = FDateTime::ParseIso8601(*DateFirstJoined, DateJoined);
	if (bSuccess == false)
	{
		UE_LOG(LogPlayerDataObject, Error, TEXT("UPlayerDataObject::GetDateSigned(): Failed to parse date: %s"), *DateFirstJoined);
	}
	return DateJoined;
}

float UPlayerDataObject::GetPlaytime() const
{
	return Playtime;
}

UPlayerDataObject* UPlayerDataObject::FromJson(const TSharedRef<FJsonObject> Json)
{
	// Generate a new ServerDataObject from Json
	UPlayerDataObject* PlayerDataObject = NewObject<UPlayerDataObject>();

	Json->TryGetStringField("id", PlayerDataObject->Id);
	Json->TryGetStringField("ident", PlayerDataObject->Ident);
	Json->TryGetStringField("ident_type", PlayerDataObject->IdentType);
	Json->TryGetStringField("username", PlayerDataObject->Username);
	Json->TryGetStringField("image_tile", PlayerDataObject->ImageUrl);

	// In many cases, neither date start, nor date end will be present
	if (Json->TryGetStringField("date_start", PlayerDataObject->DateStart) == false)
	{
		Json->TryGetStringField("date_end", PlayerDataObject->DateEnd);
	}

	// Present in the case of Server Player Details
	Json->TryGetNumberField("playtime", PlayerDataObject->Playtime);
	Json->TryGetStringField("date_first_joined", PlayerDataObject->DateFirstJoined);
	// End of Server Player Details

	return PlayerDataObject;
}
