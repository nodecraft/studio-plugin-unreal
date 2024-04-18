// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "ServerRegionDataObject.generated.h"

/**
 * 
 */
UCLASS()
class NODECRAFTDISCOVERY_API UServerRegionDataObject : public UObject
{
	GENERATED_BODY()

public:
	static UServerRegionDataObject* FromJson(const TSharedRef<FJsonObject>& Json);

	FText GetTitle() const;
	FString GetID() const;
	
protected:
	FString Title;
	FString ID;
};
