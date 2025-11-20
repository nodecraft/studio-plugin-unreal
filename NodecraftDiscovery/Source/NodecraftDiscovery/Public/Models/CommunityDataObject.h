// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Dom/JsonObject.h"
#include "CommunityDataObject.generated.h"

/**
 * 
 */
UCLASS()
class NODECRAFTDISCOVERY_API UCommunityDataObject : public UObject
{
	GENERATED_BODY()

private:
	FString Id;
	FText Title;
	FText Summary;
	FString ImageUrl;

public:
	FString GetId() const;
	FText GetTitle() const;
	FText GetSummary() const;
	FString GetImageUrl() const;

	static UCommunityDataObject* FromJson(const TSharedRef<FJsonObject>& Json);
};
