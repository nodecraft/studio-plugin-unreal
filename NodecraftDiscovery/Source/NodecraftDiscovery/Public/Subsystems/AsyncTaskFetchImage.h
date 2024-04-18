// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Interfaces/IHttpRequest.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "UObject/Object.h"
#include "AsyncTaskFetchImage.generated.h"

class UTexture2DDynamic;

DECLARE_DELEGATE_TwoParams(FFetchImageDelegate, UTexture2D* /*Texture*/, FString /*URL*/);

/**
 * 
 */
UCLASS()
class NODECRAFTDISCOVERY_API UAsyncTaskFetchImage : public UBlueprintAsyncActionBase
{
	GENERATED_UCLASS_BODY()

public:
	UFUNCTION(BlueprintCallable, meta=( BlueprintInternalUseOnly="true" ))
	static UAsyncTaskFetchImage* DownloadImage(FString URL);

public:

	FFetchImageDelegate OnSuccess;

	FFetchImageDelegate OnFail;

public:

	void Start(FString URL);

private:

	/** Handles image requests coming from the web */
	void HandleImageRequest(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded);
};
