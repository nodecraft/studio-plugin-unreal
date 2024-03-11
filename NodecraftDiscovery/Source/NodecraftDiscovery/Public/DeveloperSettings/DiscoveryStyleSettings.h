// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "DiscoveryStyleSettings.generated.h"


UENUM()
enum class EDateFormat
{
	DayMonthYear		UMETA(DisplayName = "DD/MM/YYYY"),
	MonthDayYear		UMETA(DisplayName = "MM/DD/YYYY")
};


/**
 * 
 */
UCLASS(Config=Nodecraft, DefaultConfig)
class NODECRAFTDISCOVERY_API UDiscoveryStyleSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	static const UDiscoveryStyleSettings& Get() {
		return *GetDefault<UDiscoveryStyleSettings>();
	}
	virtual FName GetCategoryName() const override;

	TSoftObjectPtr<UTexture2D> GetDefaultAvatarImage() const;
	
	static FString GetDateFormat();

protected:
	UPROPERTY(EditAnywhere, Config)
	TSoftObjectPtr<UTexture2D> DefaultAvatarImage;

	UPROPERTY(EditAnywhere, Config)
	EDateFormat DateFormat;
};
