// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ServiceDelegates.h"
#include "Models/Server/HeartbeatDataObjects.h"
#include "Models/Server/PlayerJoinDataObjects.h"
#include "Models/Server/PlayerLeaveDataObjects.h"
#include "Subsystems/EngineSubsystem.h"
#include "Engine/Engine.h"
#include "GameServerEventsService.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FHeartbeatDelegate, FHeartbeatResponseDataObject, HeartbeatResponse, bool, bSuccess, FText, Error);
DECLARE_DELEGATE_ThreeParams(FPlayerJoinResponseDelegate, FPlayerJoinResponseDataObject /*HeartbeatResponse*/, bool /*bSuccess*/, TOptional<FText> /*Error*/);

UCLASS()
class NODECRAFTDISCOVERY_API UGameServerEventsService : public UEngineSubsystem
{
	GENERATED_BODY()

public:
	static UGameServerEventsService& Get() { return *GEngine->GetEngineSubsystem<UGameServerEventsService>(); }

	/*
	 * Heartbeat is used to keep the server status up to date and to ensure the server is still active and online.
	 * This endpoint will also return any moderation updates for players that need to be kicked or disconnected.
	 */
	bool SendHeartbeat(const FHeartbeatRequestDataObject&);

	/* Marks the server as online and available for Players to join,
	 * or marks the server as offline and unavailable for Players to join. */
	bool SetServerStatus(bool bIsOnline, FSimpleServiceResponseDelegate OnComplete);

	/*
	 Consumes a Player Join Token to validate the Player platform identity,
	 access to this server, and creates a session for this Player.
	 The Session ID and Analytics ID are used to track other events from the Game Server.
	 */
	bool ValidatePlayerOnJoin(const FPlayerJoinRequestData& PlayerJoinReqData, FPlayerJoinResponseDelegate OnComplete);

	// Tracks a Player leaving the server. This is used to track the Player's session time and other analytics.
	bool TrackPlayerLeft(const FPlayerLeaveRequestData& PlayerLeaveReqData, FSimpleServiceResponseDelegate OnComplete);

	// Marks a kick as having being executed on the server, after they have been disconnected. Doubles as a "leave" event report.
	bool TrackPlayerKicked(const FString& SessionId, const FString& KickId, FSimpleServiceResponseDelegate OnComplete);

	// Marks the end of the reconnection phase for Players. This is used to track the Player's session time and other analytics.
	bool FinishedReconnectionPhase();
	
	FHeartbeatDelegate& GetHeartbeatDelegate() { return OnHeartbeatResponse; }

protected:
	// Use this to listen for heartbeat responses
	UPROPERTY(BlueprintAssignable, Category = "Nodecraft Services")
	FHeartbeatDelegate OnHeartbeatResponse;
};
