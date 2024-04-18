// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#include "NodecraftGameModeBase.h"

#include "API/NodecraftStudioSessionManagerSubsystem.h"
#include "Interfaces/NodecraftPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "Subsystems/NodecraftGameServerManager.h"

DEFINE_LOG_CATEGORY_STATIC(LogNodecraftGame, Log, All);

#if NC_USE_NODECRAFT_ON_SERVER

void ANodecraftGameModeBase::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);
	if (NewPlayer->Implements<UNodecraftPlayer>())
	{
		if (const auto Connection = NewPlayer->GetNetConnection())
		{
			Cast<INodecraftPlayer>(NewPlayer)->SetIpAddress(Connection->LowLevelGetRemoteAddress());
			UE_LOG(LogNodecraftGame, Verbose, TEXT("Just set IP Address of PostLogin Player Controller to: %s"), *Cast<INodecraftPlayer>(NewPlayer)->GetIpAddress());
		}
	}
}

APlayerController* ANodecraftGameModeBase::Login(UPlayer* NewPlayer, ENetRole InRemoteRole, const FString& Portal,
	const FString& Options, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	APlayerController* NewPlayerController =  Super::Login(NewPlayer, InRemoteRole, Portal, Options, UniqueId, ErrorMessage);

	FString JoinToken;
	
	if (UGameplayStatics::HasOption(Options, TEXT("join_token")))
	{
		JoinToken = UGameplayStatics::ParseOption(Options, TEXT("join_token"));
		if (NewPlayerController->Implements<UNodecraftPlayer>())
		{
			Cast<INodecraftPlayer>(NewPlayerController)->SetJoinToken(JoinToken);
			
			UE_LOG(LogNodecraftGame, Verbose, TEXT("Created player controller with join token: %s"), *JoinToken);
		}
		else
		{
			UE_LOG(LogNodecraftGame, Verbose, TEXT("Failed to cast player controller to INodecraftPlayer when setting join token."
											"They will be kicked by the Game Mode during validation."));
		}
	}
	return NewPlayerController;
}

void ANodecraftGameModeBase::PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId,
	FString& ErrorMessage)
{
	Super::PreLogin(Options, Address, UniqueId, ErrorMessage);
	// If we already have an error, don't bother checking for a join token
	if (ErrorMessage.Len() == 0)
	{
		// If the player doesn't have a join token, they can't join
		if (!UGameplayStatics::HasOption(Options, TEXT("join_token")))
		{
			ErrorMessage = TEXT("Player missing join token.");
		}
	}
}

void ANodecraftGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	if (GetNetMode() == NM_DedicatedServer || GetNetMode() == NM_ListenServer)
	{
		if (UNodecraftStudioSessionManager::Get().IsGameServerAccessTokenSet())
		{
			UE_LOG(LogNodecraftGame, Log, TEXT("GameServerAccessToken is set."));
		}
		else
		{
			UE_LOG(LogNodecraftGame, Error, TEXT("GameServerAccessToken not found in command line arguments or environment variables. Refer to our documentation at https://dash.readme.com/project/nodecraft-studio/v1.0/docs/dedicated-server for information on how to set up your server."));
			FPlatformMisc::RequestExit(false);
		}
	}
}

void ANodecraftGameModeBase::ProcessServerTravel(const FString& URL, bool bAbsolute)
{
	UNodecraftGameServerManager::Get().SavePlayerVerificationData(GetWorld());
	Super::ProcessServerTravel(URL, bAbsolute);
}

void ANodecraftGameModeBase::StartToLeaveMap()
{
	Super::StartToLeaveMap();
	// Copied from AGameModeBase::ProcessServerTravel
	if (const bool bSeamless = (bUseSeamlessTravel && GetWorld()->TimeSeconds < 172800.0f); bSeamless == false)
	{
		// If we aren't doing seamless travel i.e. are actually leaving the map
		// we need to set a flag so that our player leave events mark players as awaiting reconnection
		UNodecraftGameServerManager::Get().SetIsTransitioningToNewLevel(true);
	}
}

#endif // NC_USE_NODECRAFT_ON_SERVER
