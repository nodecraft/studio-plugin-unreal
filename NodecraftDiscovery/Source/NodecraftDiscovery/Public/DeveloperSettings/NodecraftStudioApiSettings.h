// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "NodecraftStudioApiSettings.generated.h"

/**
 * 
 */
UCLASS(Config=Nodecraft, DefaultConfig)
class NODECRAFTDISCOVERY_API UNodecraftStudioApiSettings : public UDeveloperSettings
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Nodecraft Settings", Config)
	FString ApiRoot = "https://api.nodecraft.studio/v1/";

	UPROPERTY(EditAnywhere, Category = "Nodecraft Settings", Config)
	FString PublicGameToken;

	UPROPERTY(EditAnywhere, Category = "Nodecraft Settings", Config)
	float NotificationPollingInterval = 15.0f;

	UPROPERTY(EditAnywhere, Category = "Nodecraft Settings", Config)
	float ServerJoinPollingInterval = 15.0f;

	UPROPERTY(EditAnywhere, Category = "Nodecraft Settings", Config)
	float FriendsPollingIntervalInGame = 180.0f;

	UPROPERTY(EditAnywhere, Category = "Nodecraft Settings", Config)
	float FriendsPollingIntervalOutOfGame = 30.0f;

	// In Seconds
	UPROPERTY(EditAnywhere, Category = "Nodecraft Settings", Config)
	float ServerHeartbeatRate = 8.0f;

public:
	FString GetApiRoot() const;
	FString GetPublicGameToken() const;
	float GetNotificationPollingInterval() const;
	float GetServerJoinPollingInterval() const;
	float GetFriendsPollingIntervalInGame() const;
	float GetFriendsPollingIntervalOutOfGame() const;
	float GetServerHeartbeatRate() const;

	virtual FName GetCategoryName() const override;

	static const UNodecraftStudioApiSettings& Get() {
		return *GetDefault<UNodecraftStudioApiSettings>();
	}

};
