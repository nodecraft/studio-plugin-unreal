// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Models/PlayerDataObject.h"
#include "UObject/Object.h"
#include "PlayerJoinDataObjects.generated.h"

USTRUCT()
struct NODECRAFTDISCOVERY_API FPlayerJoinRequestData
{
	GENERATED_BODY()

	FString Token;
	
	FString Ip;

	// This is up to your discretion as a developer as far as what to track.
	// This information will be available in the future for you to view in your analytics dashboard.
	// You can use this to track the platform of the player, any mod information if applicable, or whatever you see fit.
	FString Agent;

	// This is up to your discretion as a developer as far as what to track.
	// This information will be available in the future for you to view in your analytics dashboard.
	// It is recommended to track a Game & or Build version with this field.
	FString AgentVersion;

	TOptional<FString> AnalyticsToken;
};

USTRUCT()
struct NODECRAFTDISCOVERY_API FPlayerJoinResponseDataObject
{
	GENERATED_BODY()

	// Unique UUID for the Player's current server session.
	FString SessionId;;

	// Unique UUID for the Player's current analytics session. null, when the Player has opted out of analytics.
	TOptional<FString> AnalyticsSessionId;

	UPROPERTY()
	UPlayerDataObject* PlayerData = nullptr;

	static FPlayerJoinResponseDataObject FromJson(TSharedRef<FJsonObject> Json);
};
