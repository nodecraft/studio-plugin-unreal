// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonLazyImage.h"
#include "Models/PlayerDataObject.h"
#include "AsyncImage.generated.h"

/**
 * 
 */
UCLASS()
class NODECRAFTDISCOVERY_API UAsyncImage : public UCommonLazyImage
{
	GENERATED_BODY()

public:
	// Will load the image from the cache if it exists, otherwise it will download it from the URI and cache it.
	void LoadImageAsync(const FString& URI, const FString& FilenameForCache);

	void LoadPlayerAvatarAsync(const UPlayerDataObject* PlayerDataObject);

	void SetLoading(bool bIsLoading);
};
