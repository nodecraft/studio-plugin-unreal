// Nodecraft, Inc. © 2012-2024, All Rights Reserved.


#include "Models/ServerSessionDataObject.h"

UServerSessionDataObject* UServerSessionDataObject::FromJson(const TSharedPtr<FJsonObject>& Json)
{
	UServerSessionDataObject* Session = NewObject<UServerSessionDataObject>();

	Session->Token = Json->GetStringField("token");
	Json->TryGetStringField("server_connection", Session->ServerConnection);
	Session->PlayerAccessTokenID = Json->GetStringField("player_access_token_id");

	FString DateString;
	if (Json->TryGetStringField("date_expires", DateString))
	{
		FDateTime::ParseIso8601(*DateString, Session->DateExpires);
	}
	
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
