// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Json/Public/Dom/JsonObject.h"
#include "Core/Public/Templates/SharedPointer.h"
#include "Consents.generated.h"

UENUM()
enum class EConsentsStatus
{
	Missing,
	Outdated,
	Signed
};

USTRUCT()
struct NODECRAFTCOMMON_API FConsentPageDetails
{
	GENERATED_BODY()

	FString PageId;
	FString PageContentId;
	FString Type;
	FString Entity;
	FString EntityId;
	
	static FConsentPageDetails FromJson(const TSharedPtr<FJsonObject, ESPMode::ThreadSafe>& Json);
	TSharedRef<FJsonObject> ToJson() const;
	bool operator==(const FConsentPageDetails& Element) const;
};


USTRUCT()
struct NODECRAFTCOMMON_API FGameConsents
{
	GENERATED_BODY()

	TArray<FConsentPageDetails> ConsentPages;
	FString Content;

	
	TArray<TSharedPtr<FJsonValue>> ToJsonArray() const;

	FString GetConsentType() const;
	bool operator==(const FGameConsents& GameConsents) const;
	bool operator!=(const FGameConsents& GameConsents) const;

	static FGameConsents FromDataJson(const TSharedPtr<FJsonObject>& Json);

	static FGameConsents FromDataJsonArray(const TArray<TSharedPtr<FJsonValue>>& Json);
};

USTRUCT()
struct NODECRAFTCOMMON_API FPlayerConsents
{
	GENERATED_BODY()
	
	FPlayerConsents()
	{
		Content = "";
		ConsentsStatusAll = EConsentsStatus::Missing;
		DateAllSigned = FDateTime();
	}

	static FPlayerConsents FromJson(const TSharedPtr<FJsonObject>& Json);
	
	FString Content;
	EConsentsStatus ConsentsStatusAll;

	// valid only if consents status is signed
	FDateTime DateAllSigned;
};
