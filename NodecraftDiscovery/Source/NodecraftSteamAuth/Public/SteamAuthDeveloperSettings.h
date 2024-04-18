// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "SteamAuthDeveloperSettings.generated.h"

UCLASS(config=Nodecraft, DefaultConfig, DisplayName="Nodecraft Auth - Steam")
class NODECRAFTSTEAMAUTH_API USteamAuthDeveloperSettings : public UDeveloperSettings
{
	GENERATED_BODY()

private:
	UPROPERTY(Config, EditAnywhere)
	FString AppId;

public:
	FString GetAppId() const;

	virtual FName GetCategoryName() const override;
};
