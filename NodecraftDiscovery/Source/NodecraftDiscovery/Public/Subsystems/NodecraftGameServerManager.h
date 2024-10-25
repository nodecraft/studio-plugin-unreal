// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Models/Server/HeartbeatDataObjects.h"
#include "Models/Server/PlayerJoinDataObjects.h"
#include "Subsystems/WorldSubsystem.h"
#include "Engine/Engine.h"
#include "NodecraftGameServerManager.generated.h"

USTRUCT()
struct NODECRAFTDISCOVERY_API FVerifiedPlayerData
{
	GENERATED_BODY()

	UPROPERTY()
	FString SessionId;

	UPROPERTY()
	FString AnalyticsSessionId;

	UPROPERTY()
	FString UUID;

	UPROPERTY()
	FString PlayerID;
};

/**
 * 
 */
UCLASS()
class NODECRAFTDISCOVERY_API UNodecraftGameServerManager : public UEngineSubsystem
{
	GENERATED_BODY()

public:
	static UNodecraftGameServerManager& Get() { return *GEngine->GetEngineSubsystem<UNodecraftGameServerManager>(); }

	void StartHeartbeats(UWorld* World);
	void SavePlayerVerificationData(UWorld* InWorld);
	// UWorldSubsystem interface
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	void StopHeartbeats();
	virtual void Deinitialize() override;
	// End of UWorldSubsystem interface

	void SetIsTransitioningToNewLevel(bool bIsTransitioning);

private:

	UPROPERTY()
	TArray<APlayerController*> UnverifiedPlayers = TArray<APlayerController*>();

	UPROPERTY()
	TArray<APlayerController*> VerifiedPlayers = TArray<APlayerController*>();

	// This is filled when server travel occurs, and cleared shortly after the new level is loaded.
	UPROPERTY()
	TMap<FString, FVerifiedPlayerData> VerifiedSessionsMap;

	// Begin gameplay event handles
	FDelegateHandle LogoutEventHandle;
	FDelegateHandle PostLoginEventHandle;
	FTimerHandle HeartbeatTimerHandle = FTimerHandle();
	FTimerHandle ClearVerifiedSessionsHandle = FTimerHandle();
	// End gameplay event handles

	TWeakObjectPtr<UWorld> WeakCachedWorld;

	FDateTime LastHeartbeatDate;

	// While this is true, any player leave events will be marked as "reconnecting", which will
	// prevent their session token from being automatically invalidated.
	bool bWorldIsTransitioningToNewLevel = false;

	// Called when a player joins the server.
	// We will check if the player is verified and if not, we will kick them.
	void OnPlayerJoinedServer(APlayerController* PlayerController);

	UFUNCTION()
	void OnHeartbeatResponseReceived(FHeartbeatResponseDataObject HeartbeatResponse, bool bSuccess, FText Error);
	void OnPlayerLeftServer(APlayerController* PlayerController);

	void KickPlayer(APlayerController* PlayerController, const FText& Reason);

protected:
	// The number of heartbeat calls that have failed in a row
	// You can use this to determine if the server is having issues staying in contact with Nodecraft
	// TODO: Dexter Add documentation for this
	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft Subsystems")
	int32 NumConsecutiveFailedHeartbeats = 0;

};
