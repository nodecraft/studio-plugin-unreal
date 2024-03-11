// Fill out your copyright notice in the Description page of Project Settings.


#include "Models/ServerQueueTokenDataObject.h"

UServerQueueTokenDataObject* UServerQueueTokenDataObject::FromJson(const TSharedRef<FJsonObject>& Json)
{
	UServerQueueTokenDataObject* ServerQueueToken = NewObject<UServerQueueTokenDataObject>();

	ServerQueueToken->Token = Json->GetStringField("token");

	const FString ExpirationString = Json->GetStringField("date_queue_expires");
	FDateTime::ParseIso8601(*ExpirationString, ServerQueueToken->QueueExpires);

	const FString RenewString = Json->GetStringField("date_queue_renews");
	FDateTime::ParseIso8601(*RenewString, ServerQueueToken->QueueRenews);

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
