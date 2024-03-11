// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "SteamAuthDeveloperSettings.generated.h"

UCLASS(config=Nodecraft, DefaultConfig, DisplayName="Nodecraft Auth - Steam")
class STEAMAUTH_API USteamAuthDeveloperSettings : public UDeveloperSettings
{
	GENERATED_BODY()

private:
	UPROPERTY(Config, EditAnywhere)
	FString AppId;

public:
	FString GetAppId() const;

	virtual FName GetCategoryName() const override;
};
