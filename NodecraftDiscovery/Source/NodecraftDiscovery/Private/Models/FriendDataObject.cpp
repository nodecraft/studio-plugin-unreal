// Fill out your copyright notice in the Description page of Project Settings.


#include "Models/FriendDataObject.h"

#include "Models/PlayerDataObject.h"
#include "Models/ServerDataObject.h"

UFriendDataObject* UFriendDataObject::FromJson(const TSharedRef<FJsonObject> Json)
{
	UFriendDataObject* FriendDataObject = NewObject<UFriendDataObject>();

	// parse the player object
	if (const TSharedPtr<FJsonObject> PlayerObject = Json->GetObjectField("player"))
	{
		FriendDataObject->Player = UPlayerDataObject::FromJson(PlayerObject.ToSharedRef());
	}

	FriendDataObject->Status = Json->GetStringField("status");
	Json->TryGetStringField("date_active", FriendDataObject->DateActive);

	// parse the server object
	const TSharedPtr<FJsonObject>* ServerObject;
	if (Json->TryGetObjectField("server", ServerObject))
	{
		FriendDataObject->ServerDataObject = UServerDataObject::FromJson(ServerObject->ToSharedRef());
	}

	return FriendDataObject;
}

UPlayerDataObject* UFriendDataObject::GetPlayer() const
{
	return Player;
}

UServerDataObject* UFriendDataObject::GetServer() const
{
	return ServerDataObject;
}

FText UFriendDataObject::GetStatus() const
{
	return FText::FromString(Status);
}

FText UFriendDataObject::GetDateActive() const
{
	return FText::FromString(DateActive);
}
