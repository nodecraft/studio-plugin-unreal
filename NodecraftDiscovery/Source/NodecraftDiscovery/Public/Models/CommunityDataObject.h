// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
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
