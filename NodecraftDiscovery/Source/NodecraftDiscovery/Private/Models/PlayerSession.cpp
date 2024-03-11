// Fill out your copyright notice in the Description page of Project Settings.


#include "Models/PlayerSession.h"

bool FPlayerSession::IsValid() const
{
	return Token.IsEmpty() == false && DateExpires > FDateTime::UtcNow();
}

FPlayerSession FPlayerSession::FromJson(const TSharedPtr<FJsonObject>& Json)
{
	FPlayerSession Session = FPlayerSession();
	Json.Get()->TryGetStringField("token", Session.Token);
	Json.Get()->TryGetStringField("server_region_id", Session.ServerRegionId);

	if (FString ExpiresStr; Json.Get()->TryGetStringField("date_expires", ExpiresStr))
	{
		FDateTime::ParseIso8601(*ExpiresStr, Session.DateExpires);
	}
	
	if (const TSharedPtr<FJsonObject>& PlayerData = Json.Get()->GetObjectField("player"); PlayerData.IsValid())
	{
		Session.PlayerData = UPlayerDataObject::FromJson(PlayerData.ToSharedRef());
	}

	return Session;
}
