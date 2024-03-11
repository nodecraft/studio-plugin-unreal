// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "DataTypes/ModerationTypes.h"
#include "ModerationLogEntryDataObject.generated.h"

class UPlayerDataObject;
/**
 * 
 */
UCLASS()
class NODECRAFTDISCOVERY_API UModerationLogEntryDataObject : public UObject
{
	GENERATED_BODY()

public:
	static UModerationLogEntryDataObject* FromJson(const TSharedRef<FJsonObject> Json);

	UPlayerDataObject* GetPlayer() const;
	FDateTime GetDateCreated() const;
	FDateTime GetDateExpires() const;
	EModerationDuration GetModerationDuration() const;
	

protected:
	UPROPERTY()
	UPlayerDataObject* Player;
	
	FString Type;
	FDateTime DateCreated;

	// only present for bans. if empty, then the ban is permanent.
	FDateTime DateExpires;

	EModerationDuration ModerationDuration;
};
