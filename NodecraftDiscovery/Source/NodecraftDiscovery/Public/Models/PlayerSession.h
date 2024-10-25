// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "PlayerDataObject.h"
#include "UObject/Object.h"
#include "Dom/JsonObject.h"
#include "PlayerSession.generated.h"

USTRUCT()
struct NODECRAFTDISCOVERY_API FPlayerSession
{
	GENERATED_BODY()

	FPlayerSession() = default;

	UPROPERTY()
	FString Token;

	UPROPERTY()
	FDateTime DateExpires;

	UPROPERTY()
	FString ServerRegionId;
	
	UPROPERTY()
	UPlayerDataObject* PlayerData = nullptr;

	bool IsValid() const;
	
	static FPlayerSession FromJson(const TSharedPtr<FJsonObject>& Json);
};