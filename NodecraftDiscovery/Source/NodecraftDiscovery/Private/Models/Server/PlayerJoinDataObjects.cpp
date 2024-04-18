// Fill out your copyright notice in the Description page of Project Settings.

#include "Models/Server/PlayerJoinDataObjects.h"

FPlayerJoinResponseDataObject FPlayerJoinResponseDataObject::FromJson(TSharedRef<FJsonObject> Json)
{
	FPlayerJoinResponseDataObject Data = FPlayerJoinResponseDataObject();
	
	Data.SessionId = Json->GetStringField("session_id");
	
	if (Json->HasField("analytics_session_id"))
	{
		Data.AnalyticsSessionId = Json->GetStringField("analytics_session_id");
	}
	
	if (const TSharedPtr<FJsonObject> PlayerObject = Json->GetObjectField("player"))
	{
		Data.PlayerData = UPlayerDataObject::FromJson(PlayerObject.ToSharedRef());
	}

	return Data;
}
