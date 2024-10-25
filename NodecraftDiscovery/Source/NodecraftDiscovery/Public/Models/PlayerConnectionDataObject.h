// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Dom/JsonObject.h"
#include "PlayerConnectionDataObject.generated.h"

enum class EPlayerConnectionStatus : uint8;
/**
 * 
 */
UCLASS()
class NODECRAFTDISCOVERY_API UPlayerConnectionDataObject : public UObject
{
	GENERATED_BODY()

public:
	static UPlayerConnectionDataObject* FromJson(const TSharedRef<FJsonObject>& Json);

	FString GetId() const;
	FString GetUrl() const;
	FString GetConnectionStatusRaw() const;
	EPlayerConnectionStatus GetConnectionStatus() const;
	FDateTime GetDateExpires() const;

private:
	FString Id;
	FString Url;
	EPlayerConnectionStatus Status;
	FDateTime DateExpires;
	FString StatusRaw;
};
