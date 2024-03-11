// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "DiscoveryWidgetSettings.generated.h"

class UCommonActivatableWidget;
/**
 * 
 */
UCLASS(Config=Nodecraft, DefaultConfig)
class NODECRAFTDISCOVERY_API UDiscoveryWidgetSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	static const UDiscoveryWidgetSettings& Get() {
		return *GetDefault<UDiscoveryWidgetSettings>();
	}

	virtual FName GetCategoryName() const override;

	TSoftClassPtr<UCommonActivatableWidget> GetJoiningServerQueueModal() const;

	TSoftClassPtr<UCommonActivatableWidget> GetServerPasswordModal() const;

protected:
	// TODO: move queue and password modals to the main menu
	UPROPERTY(EditAnywhere, Config)
	TSoftClassPtr<UCommonActivatableWidget> JoiningServerQueueModal;

	UPROPERTY(EditAnywhere, Config)
	TSoftClassPtr<UCommonActivatableWidget> ServerPasswordModal;
};
