// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/EngineSubsystem.h"
#include "Engine/Engine.h"
#include "GameService.generated.h"

enum class EIdentityType : uint8;
class UGameDataObject;
class UServerRegionDataObject;

DECLARE_DELEGATE_ThreeParams(FGameDetailsResponseDelegate, UGameDataObject* /*GameDataObject*/, bool /*bSuccess*/, TOptional<FText> /*Error*/);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerInGameStateChanged, bool /*bInGame*/);

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

	FString GetCachedGameLogoURL() const;

	bool IsGameDetailsCached() const;

	// TODO: This should be moved somewhere else
	FDelegateHandle AddPlayerInGameStateListener(const FOnPlayerInGameStateChanged::FDelegate& Delegate);

	// TODO: This should be moved somewhere else
	UFUNCTION(BlueprintCallable, Category = "Nodecraft Services")
	void SetIsPlayerInGame(bool bInGame);
	
protected:
	UPROPERTY()
	UGameDataObject* GameDataObject;

	// TODO: This should be moved somewhere else
	bool bIsPlayerInGame = false;

private:
	FOnPlayerInGameStateChanged OnPlayerInGameStateChanged;
};
