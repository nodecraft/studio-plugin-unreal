// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "PlayerLeaveDataObjects.generated.h"

USTRUCT()
struct NODECRAFTDISCOVERY_API FPlayerLeaveRequestData
{
	GENERATED_BODY()

	FString SessionId;

	FString Ip;

	TOptional<FString> AnalyticsSessionId;

	TOptional<FString> SessionEndMessage;
	FString ToString() const;

	// Set to true if the player is expected to reconnect more or less immediately.
	// Used when transitioning between maps
	bool bIsAwaitingReconnection = false;
};
