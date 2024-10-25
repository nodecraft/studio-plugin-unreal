// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Dom/JsonObject.h"
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
	EModerationAction GetType() const;
	FString GetReason() const;
	

protected:
	UPROPERTY()
	UPlayerDataObject* Player;
	
	EModerationAction Type;
	FString Reason;
	FDateTime DateCreated;

	// only present for bans. if empty, then the ban is permanent.
	FDateTime DateExpires;

	EModerationDuration ModerationDuration;
};
