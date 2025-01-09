// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonLazyImage.h"
#include "DataTypes/ImageBackgroundTypes.h"
#include "Models/PlayerDataObject.h"
#include "Services/ServiceDelegates.h"
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
	void LoadImageAsync(const FString& URI, const FString& FilenameForCache, const ETransparentPixelOverrides TransparentPixelOverride = ETransparentPixelOverrides::None);
	// Will download the image from the URI and not cache it, nor will it look for it in the cache.
	void LoadImageAsyncSkipCache(const FString& URI, const ETransparentPixelOverrides TransparentPixelOverride = ETransparentPixelOverrides::None);
	void LoadImageAsyncSkipCache(const FString& URI, FSimpleServiceResponseDelegate OnComplete, const ETransparentPixelOverrides TransparentPixelOverride = ETransparentPixelOverrides::None);

	void LoadPlayerAvatarAsync(const UPlayerDataObject* PlayerDataObject);

	void SetLoading(bool bIsLoading);

private:
	// We need to keep track of the loading state here because UCommonLazyImage::IsLoading() is not reliable for our purposes
	bool bIsCurrentlyLoading = false;
};
