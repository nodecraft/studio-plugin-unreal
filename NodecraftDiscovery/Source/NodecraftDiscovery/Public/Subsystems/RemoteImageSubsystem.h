// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RemoteImageSubsystem.generated.h"

#define NC_CACHED_GAME_BACKGROUND_FILENAME "game_background"

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
	void FetchImage(FString CachedFileName, FString RemoteURL, FOnImageDownloadedDelegate& Complete);
	void SaveTextureToDisk(UTexture2D* Texture, const FString& FilePath);
	UTexture2D* LoadTextureFromDisk(const FString& Filename);
	UTexture2D* GetCachedGameBackground();
	void CacheGameBackgroundImage(bool bForce);
};
