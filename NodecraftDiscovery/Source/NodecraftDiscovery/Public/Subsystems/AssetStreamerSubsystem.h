// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StreamableManager.h"
#include "Subsystems/EngineSubsystem.h"
#include "AssetStreamerSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class NODECRAFTDISCOVERY_API UAssetStreamerSubsystem : public UEngineSubsystem
{
	GENERATED_BODY()

private:
	FStreamableManager StreamableManager;

public:
	static UAssetStreamerSubsystem& Get() { return *GEngine->GetEngineSubsystem<UAssetStreamerSubsystem>(); }

	void LoadAssetAsync(const FSoftObjectPath& AssetPath, const FStreamableDelegate& OnLoaded);
	void LoadAssetAsync(TSoftObjectPtr<> AssetPath, const FStreamableDelegate& OnLoaded);
	void LoadAssetSync(const FSoftObjectPath& AssetPath);
	void LoadAssetSync(TSoftObjectPtr<> AssetPath);

	void LoadAssetsAsync(const TArray<FSoftObjectPath>& AssetPaths, const FStreamableDelegate& OnLoaded);
	void LoadAssetsSync(const TArray<FSoftObjectPath>& AssetPaths);
};
