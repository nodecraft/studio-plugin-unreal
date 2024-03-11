// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "PlayerSettings.generated.h"

/**
 * 
 */
UCLASS()
class NODECRAFTDISCOVERY_API UPlayerSettings : public UObject
{
	GENERATED_BODY()

public:
	static UPlayerSettings* FromJson(const TSharedRef<FJsonObject>& Json);

	FString GetDefaultServerRegionID() const;
	bool GetAnalyticsOptOut() const;

protected:
	bool AnalyticsOptOut;
	FString DefaultServerRegionID;
};
