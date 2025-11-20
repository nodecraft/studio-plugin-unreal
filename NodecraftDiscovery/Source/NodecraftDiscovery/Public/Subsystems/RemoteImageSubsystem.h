// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DataTypes/ImageBackgroundTypes.h"
#include "Engine/Engine.h"
#include "RemoteImageSubsystem.generated.h"

#define NC_CACHED_GAME_BACKGROUND_FILENAME "game_background"
#define NC_CACHED_GAME_LOGO_FILENAME "game_logo"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnImageDownloadedDelegate, UTexture*);

/**
 * 
 */
UCLASS()
class NODECRAFTDISCOVERY_API URemoteImageSubsystem : public UEngineSubsystem
{
	GENERATED_BODY()

public:
	//~UEngineSubsystem interface
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	//~End of UEngineSubsystem interface

	static URemoteImageSubsystem& Get() { return *GEngine->GetEngineSubsystem<URemoteImageSubsystem>(); }

public:
	
	// Will immediately return image from cache if available, otherwise will fetch it from remote
	void FetchImage(FString CachedFileName, FString RemoteURL, FOnImageDownloadedDelegate& Complete, const ETransparentPixelOverrides TransparentPixelOverride = ETransparentPixelOverrides::None);
	void SaveTextureToDisk(UTexture2D* Texture, const FString& FilePath, const ETransparentPixelOverrides TransparentPixelOverride = ETransparentPixelOverrides::None);
	UTexture2D* LoadTextureFromDisk(const FString& Filename);
	UTexture2D* GetCachedGameBackground();
	void CacheGameBackgroundImage(bool bForce);
};
