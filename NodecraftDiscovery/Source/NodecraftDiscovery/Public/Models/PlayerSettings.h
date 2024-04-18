// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

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
