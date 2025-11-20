// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ServerStatus.h"
#include "Models/KickDataObject.h"
#include "UObject/Object.h"
#include "Dom/JsonObject.h"
#include "HeartbeatDataObjects.generated.h"

USTRUCT()
struct NODECRAFTDISCOVERY_API FHeartbeatRequestDataObject
{
	GENERATED_BODY()

	// The current status of the server.
	EServerStatus Status;

	// Array of active session IDs.
	TArray<FString> Sessions;

	// The last time the server sent a heartbeat.
	// Will be used to show unactioned kicks, etc. since this date.
	// Most likely you want to set this to the current time;
	FDateTime Since;

	FString ToString() const;
};

USTRUCT(BlueprintType)
struct NODECRAFTDISCOVERY_API FHeartbeatResponseDataObject
{
	GENERATED_BODY()

	// The time that this data was fetched, and to use for the next heartbeat.
	TOptional<FDateTime> Since;

	// An array of active session IDs for validation.
	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft Data")
	TArray<FString> SessionsActive;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft Data")
	TArray<FString> SessionsStale;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft Data")
	TArray<FPlayerKickDataObject> Kicks;
	
	static FHeartbeatResponseDataObject FromJson(const TSharedRef<FJsonObject>& Json);
};
