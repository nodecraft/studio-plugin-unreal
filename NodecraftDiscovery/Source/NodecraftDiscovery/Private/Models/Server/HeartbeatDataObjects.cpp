// Fill out your copyright notice in the Description page of Project Settings.

#include "Models/Server/HeartbeatDataObjects.h"

FString FHeartbeatRequestDataObject::ToString() const
{
	FString RequestString = "";
	// Convert the EServerStatus enum to a string
	switch (Status)
	{
	case EServerStatus::Undefined:
		RequestString += "Status: Undefined\n";
		break;
	case EServerStatus::Online:
		RequestString += "Status: Online\n";
		break;
	case EServerStatus::Offline:
		RequestString += "Status: Offline\n";
		break;
	}
	// Convert the array of session IDs to a string
	RequestString += "Sessions: ";
	for (FString Session : Sessions)
	{
		RequestString += Session + ", ";
	}
	RequestString += "\n";
	// Convert the FDateTime to a string
	RequestString += "Since: " + Since.ToString() + "\n";
	return RequestString;
}

FHeartbeatResponseDataObject FHeartbeatResponseDataObject::FromJson(const TSharedRef<FJsonObject>& Json)
{
	FHeartbeatResponseDataObject HeartbeatData = FHeartbeatResponseDataObject();

	// Parse SessionActive, an array of strings
	const TArray<TSharedPtr<FJsonValue>> SessionsActive = Json->GetArrayField(TEXT("sessions_active"));
	for (TSharedPtr<FJsonValue> Session : SessionsActive)
	{
		HeartbeatData.SessionsActive.Add(Session->AsString());
	}

	// Parse SessionStale
	const TArray<TSharedPtr<FJsonValue>> SessionsStale = Json->GetArrayField(TEXT("sessions_stale"));
	for (TSharedPtr<FJsonValue> Session : SessionsStale)
	{
		HeartbeatData.SessionsStale.Add(Session->AsString());
	}

	// Parse Kicks
	const TArray<TSharedPtr<FJsonValue>> Kicks = Json->GetArrayField(TEXT("kicks"));
	for (TSharedPtr<FJsonValue> Kick : Kicks)
	{
		HeartbeatData.Kicks.Add(FPlayerKickDataObject::FromJson(Kick->AsObject().ToSharedRef()));
	}

	// Parse Since, which comes in as a string if at all
	if (FString SinceString; Json->TryGetStringField(TEXT("since"), SinceString))
	{
		FDateTime SinceDateTime;
		if (FDateTime::ParseIso8601(*SinceString, SinceDateTime))
		{
			HeartbeatData.Since = SinceDateTime;
		}
	}

	return HeartbeatData;
}
