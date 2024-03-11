// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Models/Consents.h"
#include "UObject/Object.h"
#include "RulesDataObject.generated.h"

UENUM()
enum class EConsentStatus : uint8
{
	Unknown,
	NotSigned,
	Signed,
	Outdated
};	

/**
 * 
 */
UCLASS()
class NODECRAFTDISCOVERY_API URulesDataObject : public UObject
{
	GENERATED_BODY()

public:
	FText GetContent() const;
	EConsentStatus GetConsentStatus() const;
	FDateTime GetDateSigned() const;
	void SetDateSigned(FDateTime InSigned);
	FGameConsents GetConsents() const;
	void Update(const URulesDataObject* InRules);

	static URulesDataObject* FromJson(const TSharedRef<FJsonObject> Json);
	
private:
	FString Content;
	FString StatusRaw;
	FString DateAllSigned;
	FGameConsents Consents;
};
