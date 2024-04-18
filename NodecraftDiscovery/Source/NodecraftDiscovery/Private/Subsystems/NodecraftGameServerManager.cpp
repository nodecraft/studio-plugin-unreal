// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystems/NodecraftGameServerManager.h"

#include "NodecraftLogCategories.h"
#include "DeveloperSettings/NodecraftStudioApiSettings.h"
#include "GameFramework/GameSession.h"
#include "GameFramework/GameModeBase.h"
#include "Interfaces/NodecraftPlayer.h"
#include "Services/GameServerEventsService.h"

void UNodecraftGameServerManager::StartHeartbeats(UWorld* World)
{
	if (WeakCachedWorld.IsValid() && WeakCachedWorld.Get()->IsValidLowLevel())
	{
		StopHeartbeats();
		UE_LOG(LogNodecraftGameServerEvents, Warning, TEXT("Heartbeats are already running. Stopping them before starting again. This may be a bug."));
	}
	WeakCachedWorld = World;
	
	if (World->GetNetMode() == NM_DedicatedServer || World->GetNetMode() == NM_ListenServer)
	{
		// Get heartbeat rate from settings
		const UNodecraftStudioApiSettings* DiscoveryAPISettings = GetDefault<UNodecraftStudioApiSettings>();
		
		// Set up a timer to send heartbeats
		World->GetTimerManager().SetTimer(HeartbeatTimerHandle, [this]()
		{
			FHeartbeatRequestDataObject HeartbeatRequestData = FHeartbeatRequestDataObject();
			// TODO: Maybe we don't always set status to online?
			HeartbeatRequestData.Status = EServerStatus::Online;
			if (LastHeartbeatDate.GetTicks() > 0)
			{
				HeartbeatRequestData.Since = LastHeartbeatDate;
			}
			else
			{
				HeartbeatRequestData.Since = FDateTime(0);
			}

			HeartbeatRequestData.Sessions = TArray<FString>();
			for (APlayerController* Player : VerifiedPlayers)
			{
				HeartbeatRequestData.Sessions.Add(Cast<INodecraftPlayer>(Player)->GetSessionId());
			}
			
			UGameServerEventsService::Get().SendHeartbeat(HeartbeatRequestData);
		}, DiscoveryAPISettings->GetServerHeartbeatRate(), true);
	}
}

void UNodecraftGameServerManager::SavePlayerVerificationData(UWorld* InWorld)
{
	UWorld* CachedWorld = WeakCachedWorld.Get();

	if (InWorld == nullptr || CachedWorld == nullptr)
	{
		return;
	}

	if (CachedWorld->GetNetMode() == NM_DedicatedServer || CachedWorld->GetNetMode() ==
					NM_ListenServer)
	{
		// Stop any timer to clear the verified sessions map, in case we've switched maps and then switched again rapidly
		if (ClearVerifiedSessionsHandle.IsValid())
		{
			InWorld->GetTimerManager().ClearTimer(ClearVerifiedSessionsHandle);
		}
				
		if (InWorld == WeakCachedWorld.Get())
		{
			StopHeartbeats();
		}
		// Place all the verified players in the verified sessions map
		UE_LOG(LogNodecraftGameServerEvents, Verbose, TEXT("Placing verified players in verified sessions map."));
		if (VerifiedPlayers.Num() > 0)
		{
			for (APlayerController* Player : VerifiedPlayers)
			{
				FVerifiedPlayerData VerifiedPlayerData = FVerifiedPlayerData();
				VerifiedPlayerData.SessionId = Cast<INodecraftPlayer>(Player)->GetSessionId();
				VerifiedPlayerData.AnalyticsSessionId = Cast<INodecraftPlayer>(Player)->GetAnalyticsSessionId();
				VerifiedSessionsMap.Add(Player->GetNetConnection()->LowLevelGetRemoteAddress(), VerifiedPlayerData);
				UE_LOG(LogNodecraftGameServerEvents, Verbose, TEXT("Placed player in verified sessions map: %s"), *Player->GetNetConnection()->LowLevelGetRemoteAddress());
			}
		}
		else
		{
			UE_LOG(LogNodecraftGameServerEvents, Verbose, TEXT("No verified players to place in verified sessions map."));
		}
				
	}
}

void UNodecraftGameServerManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	if (IsRunningDedicatedServer())
	{
		// Subscribe to the game mode events
		PostLoginEventHandle = FGameModeEvents::OnGameModePostLoginEvent().AddWeakLambda(
			this, [this](AGameModeBase* GameMode, APlayerController* NewPlayer)
			{
				OnPlayerJoinedServer(NewPlayer);
			});
		
		LogoutEventHandle = FGameModeEvents::OnGameModeLogoutEvent().AddWeakLambda(
			this, [this](AGameModeBase* GameMode, AController* Controller)
			{
				if (Controller->IsA<APlayerController>())
				{
					OnPlayerLeftServer(Cast<APlayerController>(Controller));
				}
			});

		UGameServerEventsService::Get().GetHeartbeatDelegate().AddDynamic(this, &ThisClass::OnHeartbeatResponseReceived);

		FWorldDelegates::OnPostWorldInitialization.AddWeakLambda(
			this, [this](UWorld* World, const UWorld::InitializationValues IVS)
			{
				if (World->GetNetMode() == NM_DedicatedServer || World->GetNetMode() == NM_ListenServer)
				{
					SetIsTransitioningToNewLevel(false);
					StartHeartbeats(World);

					World->GetTimerManager().SetTimer(ClearVerifiedSessionsHandle, [this]()
					{
						UGameServerEventsService::Get().FinishedReconnectionPhase();
						UE_LOG(LogNodecraftGameServerEvents, Verbose, TEXT("Clearing verified sessions map."));
						VerifiedSessionsMap.Empty();
						ClearVerifiedSessionsHandle.Invalidate();
					}, 15.0f, false, 15.0f);
				}
			});
	}

}

void UNodecraftGameServerManager::StopHeartbeats()
{
	UWorld* CachedWorld = WeakCachedWorld.Get();
	if (CachedWorld)
	{
		if (CachedWorld->GetNetMode() == NM_DedicatedServer || CachedWorld->GetNetMode() == NM_ListenServer) {
			// Unsubscribe from the game mode events

			// Clear the heartbeat timer
			CachedWorld->GetTimerManager().ClearTimer(HeartbeatTimerHandle);
			HeartbeatTimerHandle.Invalidate();
			CachedWorld = nullptr;
		}
		else
		{
			UE_LOG(LogNodecraftGameServerEvents, Error, TEXT("CachedWorld is null. Cannot stop heartbeats."));
		}
	}
}

void UNodecraftGameServerManager::Deinitialize()
{
	Super::Deinitialize();
	if (IsRunningDedicatedServer())
	{
		FGameModeEvents::OnGameModePostLoginEvent().Remove(PostLoginEventHandle);
		FGameModeEvents::OnGameModeLogoutEvent().Remove(LogoutEventHandle);
	}
}

void UNodecraftGameServerManager::SetIsTransitioningToNewLevel(bool bIsTransitioning)
{
	bWorldIsTransitioningToNewLevel = bIsTransitioning;
}

void UNodecraftGameServerManager::OnPlayerJoinedServer(APlayerController* PlayerController)
{
	UE_LOG(LogNodecraftGameServerEvents, Log, TEXT("Player Joined Server. Verifying player..."));
	if (PlayerController->Implements<UNodecraftPlayer>() == false)
	{
		// Kick the player. They are unverifiable
		KickPlayer(PlayerController, FText::FromString("Player is unverifiable. They do not implement the INodecraftPlayer interface."));
		return;
	}

	if (VerifiedPlayers.Contains(PlayerController) || UnverifiedPlayers.Contains(PlayerController))
	{
		// Player is already verified or awaiting verification
		UE_LOG(LogNodecraftGameServerEvents, Log, TEXT("Player is already verified or awaiting verification. Doing no further work to verify."));
		return;
	}

	FPlayerJoinRequestData PlayerJoinRequestData = FPlayerJoinRequestData();

	// Try to get IP Address of the player
	bool bCanValidatePlayer = true;
	FText KickReason;

	// Get the player's IP Address, or kick them if we can't
	UNetConnection* Connection = PlayerController->GetNetConnection();

	// Check if the connection is valid
	if (Connection)
	{
		// Get the player's IP address
		FString IPAddress = Connection->LowLevelGetRemoteAddress();

		if (IPAddress.IsEmpty())
		{
			KickReason = FText::FromString("Player's IP Address is empty.");
			bCanValidatePlayer = false;
		}
		else
		{
			// Print or store the IP address
			UE_LOG(LogNodecraftGameServerEvents, Verbose, TEXT("Player's IP Address: %s"), *IPAddress);
			PlayerJoinRequestData.Ip = IPAddress;
		}
	}
	else
	{
		KickReason = FText::FromString("Could not get UNetConnection.");
		bCanValidatePlayer = false;
	}

	if (bCanValidatePlayer == false)
	{
		KickPlayer(PlayerController, KickReason);
		return;
	}

	// If the player's IP address is already in the verified sessions map, let them in
	if (VerifiedSessionsMap.Contains(PlayerJoinRequestData.Ip))
	{
		UE_LOG(LogNodecraftGameServerEvents, Log, TEXT("Player's IP Address is already in the verified sessions map. Letting them in."));
		// Player is verified
		FVerifiedPlayerData PlayerData = VerifiedSessionsMap[PlayerJoinRequestData.Ip];
		INodecraftPlayer* NodecraftPlayerController = Cast<INodecraftPlayer>(PlayerController);
		NodecraftPlayerController->SetSessionId(PlayerData.SessionId);
		NodecraftPlayerController->SetAnalyticsSessionId(PlayerData.AnalyticsSessionId);
		VerifiedPlayers.Add(PlayerController);
		UnverifiedPlayers.Remove(PlayerController);
		return;
	}
	else
	{
		UE_LOG(LogNodecraftGameServerEvents, Log, TEXT("Player's IP Address is not in the verified sessions map. Continuing to verify."));
	}

	// Try to get the player's session ID
	FString JoinToken = Cast<INodecraftPlayer>(PlayerController)->GetJoinToken();
	if (JoinToken.IsEmpty())
	{
		KickReason = FText::FromString("Player's Session ID is empty.");
		KickPlayer(PlayerController, KickReason);
		return;
	}
	else
	{
		PlayerJoinRequestData.Token = JoinToken;
	}

	// Try to get the player's analytics session ID
	PlayerJoinRequestData.AnalyticsToken = Cast<INodecraftPlayer>(PlayerController)->GetAnalyticsSessionId();

	// TODO: Get Agent
	// TODO: Get Agent Version
	
	UGameServerEventsService::Get().ValidatePlayerOnJoin(PlayerJoinRequestData,
		FPlayerJoinResponseDelegate::CreateWeakLambda(this, [this, PlayerController, PlayerJoinRequestData](FPlayerJoinResponseDataObject PlayerJoinResponse, bool bSuccess, TOptional<FText> Error)
		{
			if (bSuccess)
			{
				// Player is verified
				INodecraftPlayer* NodecraftPlayerController = Cast<INodecraftPlayer>(PlayerController);
				NodecraftPlayerController->SetPlayerID(PlayerJoinResponse.PlayerData->GetId());
				NodecraftPlayerController->SetSessionId(PlayerJoinResponse.SessionId);
				NodecraftPlayerController->SetAnalyticsSessionId(PlayerJoinResponse.AnalyticsSessionId.Get(""));
				VerifiedPlayers.Add(PlayerController);
				UnverifiedPlayers.Remove(PlayerController);
				UE_LOG(LogNodecraftGameServerEvents, Log, TEXT("Player verified successfully."));
			}
			else
			{
				UE_LOG(LogNodecraftGameServerEvents, Error, TEXT("Failed to verify player. They will be kicked. Error: '%s'"), *Error.GetValue().ToString());
				// Player is unverified, possibly due to error
				KickPlayer(PlayerController, Error.GetValue());
			}
		}));
}

void UNodecraftGameServerManager::OnHeartbeatResponseReceived(FHeartbeatResponseDataObject HeartbeatResponse, bool bSuccess, FText Error)
{
	if (bSuccess)
	{
		NumConsecutiveFailedHeartbeats = 0;
		if (HeartbeatResponse.Since.IsSet())
		{
			// Log the last heartbeat date
			UE_LOG(LogNodecraftGameServerEvents, Verbose, TEXT("Last heartbeat date: %s"), *HeartbeatResponse.Since.GetValue().ToString());
			LastHeartbeatDate = HeartbeatResponse.Since.GetValue();
		}
	
		// Every heartbeat, for each player...
		// If the player is in the kicks list, kick them.
		// If the player is in Stale Sessions, regardless of verification status, kick them.
		// If the player is not awaiting verification and they are not in the active sessions list, kick them
	
		// Combine VerifiedPlayers and UnverifiedPlayers into one list
		TArray<APlayerController*> AllPlayers = VerifiedPlayers;
		AllPlayers.Append(UnverifiedPlayers);

		for (APlayerController* Player : AllPlayers)
		{
			INodecraftPlayer* NodecraftPlayer = Cast<INodecraftPlayer>(Player);
			if (NodecraftPlayer == nullptr)
			{
				KickPlayer(Player, FText::FromString(
							   "Player is unverifiable. They do not implement the INodecraftPlayer interface."));
				continue;
			}

			FString PlayerSessionId = NodecraftPlayer->GetSessionId();

			// Check if player is in the kicks list
			for (FPlayerKickDataObject Kick : HeartbeatResponse.Kicks)
			{
				// log that we're comparing the player's session id to the kicked player's session id
				UE_LOG(LogNodecraftGameServerEvents, Log, TEXT("Comparing player's id: %s to kicked player's session id: %s"), *NodecraftPlayer->GetPlayerID(), *Kick.GetKickedPlayerId());
				if (Kick.GetKickedPlayerId() == NodecraftPlayer->GetPlayerID())
				{
					if (Kick.Player)
					{
						KickPlayer(Player, Kick.Reason);
						UE_LOG(LogNodecraftGameServerEvents, Log, TEXT("Kicked player %s with reason: %s\nGoing to track player kicked with session ID %s"), *NodecraftPlayer->GetPlayerID(), *Kick.Reason.ToString(), *PlayerSessionId);
						UGameServerEventsService::Get().TrackPlayerKicked(PlayerSessionId, Kick.Id,
							FSimpleServiceResponseDelegate::CreateWeakLambda(this,	[](bool bSuccess, TOptional<FText> Error)
						{
							if (bSuccess)
							{
								UE_LOG(LogNodecraftGameServerEvents, Log, TEXT("Successfully tracked player kicked"));
							}
							else
							{
								UE_LOG(LogNodecraftGameServerEvents, Error, TEXT("Failed to track player kicked: %s"), *Error.Get(FText::GetEmpty()).ToString());
							}
						}));
					}
					break;
				}
			}

			// Check if player's session is stale or not active
			if (HeartbeatResponse.SessionsStale.Contains(PlayerSessionId))
			{
				KickPlayer(Player, FText::FromString("Player's session is stale."));
			}
			else if (VerifiedPlayers.Contains(Player) && HeartbeatResponse.SessionsActive.Contains(PlayerSessionId) == false)
			{
				KickPlayer(Player, FText::FromString("Player is verified but their session is not active."));
			}
		}
	}
	else
	{
		++NumConsecutiveFailedHeartbeats;
	}
}

void UNodecraftGameServerManager::OnPlayerLeftServer(APlayerController* PlayerController)
{
	if (PlayerController == nullptr)
	{
		UE_LOG(LogNodecraftGameServerEvents, Error, TEXT("PlayerController is null. Cannot process player leaving server."));
		return;
	}

	if (PlayerController->Implements<UNodecraftPlayer>() == false)
	{
		UE_LOG(LogNodecraftGameServerEvents, Error, TEXT("Player is unverifiable. They do not implement the INodecraftPlayer interface."));
		return;
	}

	UE_LOG(LogNodecraftGameServerEvents, Warning, TEXT("Player left server. Removing them from verified and unverified players."));
	
	VerifiedPlayers.Remove(PlayerController);
	UnverifiedPlayers.Remove(PlayerController);
	
	FPlayerLeaveRequestData PlayerLeaveRequestData = FPlayerLeaveRequestData();
	PlayerLeaveRequestData.SessionId = Cast<INodecraftPlayer>(PlayerController)->GetSessionId();
	PlayerLeaveRequestData.AnalyticsSessionId = Cast<INodecraftPlayer>(PlayerController)->GetAnalyticsSessionId();
	PlayerLeaveRequestData.Ip = Cast<INodecraftPlayer>(PlayerController)->GetIpAddress();
	if (bWorldIsTransitioningToNewLevel)
	{
		UE_LOG(LogNodecraftGameServerEvents, Warning, TEXT("Player is awaiting reconnection. Marking as such."));
	}
	else
	{
		UE_LOG(LogNodecraftGameServerEvents, Warning, TEXT("Player is not awaiting reconnection. Not marking reconnection."));
	}
	PlayerLeaveRequestData.bIsAwaitingReconnection = bWorldIsTransitioningToNewLevel;
	
	UGameServerEventsService::Get().TrackPlayerLeft(PlayerLeaveRequestData,
		FSimpleServiceResponseDelegate::CreateWeakLambda(this, [this, PlayerController](bool bSuccess, TOptional<FText> Error)
	{
		if (bSuccess)
		{
			UE_LOG(LogNodecraftGameServerEvents, Warning, TEXT("Player left server successfully."));
		}
		else
		{
			UE_LOG(LogNodecraftGameServerEvents, Error, TEXT("Failed to track player leaving server: %s"), *Error.GetValue().ToString());
		}
	}));
}
void UNodecraftGameServerManager::KickPlayer(APlayerController* PlayerController, const FText& Reason)
{
	UE_LOG(LogNodecraftGameServerEvents, Log, TEXT("Kicking player with reason: %s"), *Reason.ToString());
	UWorld* CachedHeartbeatsWorld = WeakCachedWorld.Get();
	if (CachedHeartbeatsWorld == nullptr || CachedHeartbeatsWorld->IsValidLowLevel() == false)
	{
		UE_LOG(LogNodecraftGameServerEvents, Error, TEXT("CachedHeartbeatsWorld is null or invalid. Cannot kick player."));
		return;
	}
	AGameModeBase* GameMode = CachedHeartbeatsWorld->GetAuthGameMode();
	if (GameMode != nullptr)
	{
		AGameSession* GameSession = GameMode->GameSession;
		if (GameSession != nullptr)
		{
			GameSession->KickPlayer(PlayerController, Reason);
			
		}
		else
		{
			UE_LOG(LogNodecraftGameServerEvents, Error, TEXT("GameSession is null. Cannot kick player."));
		}
	}
	else
	{
		UE_LOG(LogNodecraftGameServerEvents, Error, TEXT("GameMode is null. Cannot kick player."));
	}
}
