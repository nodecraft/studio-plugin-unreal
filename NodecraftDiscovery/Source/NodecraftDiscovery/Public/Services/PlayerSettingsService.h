// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "IdentService.h"
#include "Subsystems/EngineSubsystem.h"
#include "PlayerSettingsService.generated.h"

DECLARE_DELEGATE_ThreeParams(FGetPlayerSettingsDelegate, UPlayerSettings* /*PlayerSettings*/, bool /*bSuccess*/, FText /*Error*/);

/**
 * 
 */
UCLASS()
class NODECRAFTDISCOVERY_API UPlayerSettingsService : public UEngineSubsystem
{
	GENERATED_BODY()

public:
	static UPlayerSettingsService& Get() { return *GEngine->GetEngineSubsystem<UPlayerSettingsService>(); }

	bool UpdateCurrentPlayerRegion(const FString& RegionID, FPlayerSessionResponseDelegate& OnComplete);
	bool GetPlayerSettings(FGetPlayerSettingsDelegate& OnComplete);
	bool UpdateDefaultPlayerRegion(const FString& DefaultRegionID, FGetPlayerSettingsDelegate& OnComplete);
	bool UpdatePlayerAnalyticsOptOut(bool AnalyticsOptOut, FGetPlayerSettingsDelegate& OnComplete);
};
