// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "NodecraftStudioApiSettings.generated.h"

/**
 * 
 */
UCLASS(Config=Nodecraft, DefaultConfig)
class NODECRAFTDISCOVERY_API UNodecraftStudioApiSettings : public UDeveloperSettings
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Config)
	FString ApiRoot = "https://api.nodecraft.studio/v1/";

	UPROPERTY(EditAnywhere, Config)
	FString PublicGameToken;

	UPROPERTY(EditAnywhere, Config)
	float NotificationPollingInterval = 15.0f;

	UPROPERTY(EditAnywhere, Config)
	float ServerJoinPollingInterval = 15.0f;

	UPROPERTY(EditAnywhere, Config)
	float FriendsPollingIntervalInGame = 180.0f;

	UPROPERTY(EditAnywhere, Config)
	float FriendsPollingIntervalOutOfGame = 30.0f;

	// In Seconds
	UPROPERTY(EditAnywhere, Config)
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
