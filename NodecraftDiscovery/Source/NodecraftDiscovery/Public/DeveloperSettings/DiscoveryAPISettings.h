// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DiscoveryAPISettings.generated.h"

/**
 * 
 */
UCLASS(Config=Nodecraft, DefaultConfig)
class NODECRAFTDISCOVERY_API UDiscoveryAPISettings : public UDeveloperSettings
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Config)
	FString ApiRoot;

	UPROPERTY(EditAnywhere, Config)
	FString PublicGameToken;

	UPROPERTY(EditAnywhere, Config)
	float NotificationPollingInterval = 15.0f;

	UPROPERTY(EditAnywhere, Config)
	float ServerJoinPollingInterval = 15.0f;

public:
	FString GetApiRoot() const;
	FString GetPublicGameToken() const;
	float GetNotificationPollingInterval() const;
	float GetServerJoinPollingInterval() const;

	virtual FName GetCategoryName() const override;

	static const UDiscoveryAPISettings& Get() {
		return *GetDefault<UDiscoveryAPISettings>();
	}

};
