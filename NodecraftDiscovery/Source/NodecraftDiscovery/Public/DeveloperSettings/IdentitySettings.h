// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataTypes/IdentityType.h"
#include "UObject/Object.h"
#include "IdentitySettings.generated.h"

USTRUCT()
struct FIconMapWrapper
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TMap<EIdentityType, UTexture2D*> IconMap;

};
/**
 * 
 */
UCLASS(Config=Nodecraft, DefaultConfig)
class NODECRAFTDISCOVERY_API UIdentitySettings : public UDeveloperSettings
{
	GENERATED_BODY()
public:
	static const UIdentitySettings& Get() {
		return *GetDefault<UIdentitySettings>();
	}
	
	UPROPERTY(EditAnywhere, Config)
	FIconMapWrapper IconMapWrapper;

	UTexture2D* GetIconForType(EIdentityType Type) const;

	virtual FName GetCategoryName() const override;
};
