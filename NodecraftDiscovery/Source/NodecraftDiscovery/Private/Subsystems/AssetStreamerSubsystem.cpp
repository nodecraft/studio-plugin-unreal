// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystems/AssetStreamerSubsystem.h"

void UAssetStreamerSubsystem::LoadAssetAsync(const FSoftObjectPath& AssetPath, const FStreamableDelegate& OnLoaded)
{
	StreamableManager.RequestAsyncLoad(AssetPath, OnLoaded);
}

void UAssetStreamerSubsystem::LoadAssetAsync(const TSoftObjectPtr<> AssetPath, const FStreamableDelegate& OnLoaded)
{
	StreamableManager.RequestAsyncLoad(AssetPath.ToSoftObjectPath(), OnLoaded);
}

void UAssetStreamerSubsystem::LoadAssetSync(const FSoftObjectPath& AssetPath)
{
	StreamableManager.RequestSyncLoad(AssetPath);
}

void UAssetStreamerSubsystem::LoadAssetSync(const TSoftObjectPtr<> AssetPath)
{
	StreamableManager.RequestAsyncLoad(AssetPath.ToSoftObjectPath());
}

void UAssetStreamerSubsystem::LoadAssetsAsync(const TArray<FSoftObjectPath>& AssetPaths,
	const FStreamableDelegate& OnLoaded)
{
	StreamableManager.RequestAsyncLoad(AssetPaths, OnLoaded);
}

void UAssetStreamerSubsystem::LoadAssetsSync(const TArray<FSoftObjectPath>& AssetPaths)
{
	StreamableManager.RequestSyncLoad(AssetPaths);
}
