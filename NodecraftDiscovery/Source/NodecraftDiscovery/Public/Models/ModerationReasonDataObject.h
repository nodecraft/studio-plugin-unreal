// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Dom/JsonObject.h"
#include "ModerationReasonDataObject.generated.h"

UCLASS()
class NODECRAFTDISCOVERY_API UModerationReasonDataObject : public UObject
{
	GENERATED_BODY()

public:
	FString GetId() const;
	FText GetTitle() const;
	FText GetDescription() const;

	static UModerationReasonDataObject* FromJson(const TSharedRef<FJsonObject> Json);
	
private:
	FString Id;
	FText Title;
	FText Description;
};
