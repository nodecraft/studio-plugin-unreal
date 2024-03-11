// Fill out your copyright notice in the Description page of Project Settings.


#include "Models/ServerSessionDataObject.h"

UServerSessionDataObject* UServerSessionDataObject::FromJson(const TSharedPtr<FJsonObject>& Json)
{
	UServerSessionDataObject* Session = NewObject<UServerSessionDataObject>();

	Session->Token = Json->GetStringField("token");
	Json->TryGetStringField("server_connection", Session->ServerConnection);
	Session->PlayerAccessTokenID = Json->GetStringField("player_access_token_id");

	const FString DateString = Json->GetStringField("date_expires");
	FDateTime::ParseIso8601(*DateString, Session->DateExpires);
	
	return Session;
}

FString UServerSessionDataObject::GetToken() const
{
	return Token;
}

FDateTime UServerSessionDataObject::GetDateExpires() const
{
	return DateExpires;
}

FString UServerSessionDataObject::GetPlayerAccessTokenID() const
{
	return PlayerAccessTokenID;
}

FString UServerSessionDataObject::GetServerConnection() const
{
	return ServerConnection;
}
