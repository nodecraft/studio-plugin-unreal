// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/EngineSubsystem.h"
#include "GameService.generated.h"

enum class EIdentityType : uint8;
class UGameDataObject;
class UServerRegionDataObject;

DECLARE_DELEGATE_ThreeParams(FGameDetailsResponseDelegate, UGameDataObject* /*GameDataObject*/, bool /*bSuccess*/, TOptional<FText> /*Error*/);

/**
 * 
 */
UCLASS()
class NODECRAFTDISCOVERY_API UGameService : public UEngineSubsystem
{
	GENERATED_BODY()

public:
	static UGameService& Get() { return *GEngine->GetEngineSubsystem<UGameService>(); }

	bool GetGameDetails(FGameDetailsResponseDelegate OnComplete);

	TArray<UServerRegionDataObject*> GetCachedServerRegions() const;

	TArray<EIdentityType> GetCachedIdentTypes() const;

	FString GetCachedGameBackgroundURL() const;

	bool IsGameDetailsCached() const;

protected:
	UPROPERTY()
	UGameDataObject* GameDataObject;
};
