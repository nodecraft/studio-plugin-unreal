// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataTypes/IdentityType.h"
#include "UObject/Object.h"
#include "GameDataObject.generated.h"

class UServerRegionDataObject;
UCLASS()
class NODECRAFTDISCOVERY_API UGameDataObject : public UObject
{
	GENERATED_BODY()

public:
	static UGameDataObject* FromJson(const TSharedRef<FJsonObject>& Json);

	TArray<UServerRegionDataObject*> GetServerRegions() const;
	TArray<EIdentityType> GetSupportedIdentTypes() const;
	FString GetImageBackgroundURL() const;

protected:
	UPROPERTY()
	TArray<UServerRegionDataObject*> ServerRegions;

	UPROPERTY()
	TArray<EIdentityType> SupportedIdentTypes;

	FString ImageBackgroundURL;
};
