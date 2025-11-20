// Nodecraft, Inc. © 2012-2024, All Rights Reserved.


#include "Models/ServerQueueTokenDataObject.h"

UServerQueueTokenDataObject* UServerQueueTokenDataObject::FromJson(const TSharedRef<FJsonObject>& Json)
{
	UServerQueueTokenDataObject* ServerQueueToken = NewObject<UServerQueueTokenDataObject>();

	ServerQueueToken->Token = Json->GetStringField(TEXT("token"));

	FString ExpirationString;
	if (Json->TryGetStringField(TEXT("date_queue_expires"), ExpirationString))
	{
		FDateTime::ParseIso8601(*ExpirationString, ServerQueueToken->QueueExpires);
	}
	
	FString RenewString;
	if (Json->TryGetStringField(TEXT("date_queue_renews"), RenewString))
	{
		FDateTime::ParseIso8601(*RenewString, ServerQueueToken->QueueRenews);
	}
	
	return ServerQueueToken;
}

FString UServerQueueTokenDataObject::GetToken() const
{
	return Token;
}

FDateTime UServerQueueTokenDataObject::GetQueueExpires() const
{
	return QueueExpires;
}

FDateTime UServerQueueTokenDataObject::GetQueueRenews() const
{
	return QueueRenews;
}
