// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#include "API/NodecraftStudioSessionManagerSubsystem.h"

#include "NodecraftLogCategories.h"
#include "API/NodecraftStudioSaveGame.h"
#include "DeveloperSettings/NodecraftStudioApiSettings.h"
#include "Kismet/GameplayStatics.h"

#define SAVE_GAME_FILE_NAME "NodecraftStudioSaveGame"

void UNodecraftStudioSessionManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	SetGameAccessToken(GetDefault<UNodecraftStudioApiSettings>()->GetPublicGameToken());

	// First try to read from the command line
	FString ParsedGameServerAccessToken;
	if (FParse::Value(FCommandLine::Get(), TEXT("GameServerAccessToken="), ParsedGameServerAccessToken))
	{
		SetGameServerAccessToken(ParsedGameServerAccessToken);
	}
	// If that doesn't work, try to read from the environment variable
	else if (ParsedGameServerAccessToken = FPlatformMisc::GetEnvironmentVariable(TEXT("GameServerAccessToken")); ParsedGameServerAccessToken.Len() > 0)
	{
		SetGameServerAccessToken(ParsedGameServerAccessToken);
	}
	else
	{
#if UE_SERVER
		UE_LOG(LogNodecraftGameServerEvents, Error, TEXT("GameServerAccessToken not found in command line arguments or environment variables. Refer to our documentation at https://dash.readme.com/project/nodecraft-studio/v1.0/docs/dedicated-server for information on how to set up your server."));
		FPlatformMisc::RequestExit(false);
#endif
	}
}

void UNodecraftStudioSessionManager::Deinitialize()	
{
	Super::Deinitialize();
}

#if WITH_EDITOR
void UNodecraftStudioSessionManager::SetPlayerAccessToken(const FString& NewToken)
{
	PlayerSession.Token = NewToken;
	SaveToDisk();
}
#endif

void UNodecraftStudioSessionManager::SetGameAccessToken(FString Token)
{
	GameAccessToken = Token;
}

void UNodecraftStudioSessionManager::SetGameServerAccessToken(FString Token)
{
	UE_LOG(LogNodecraftGameServerEvents, Log, TEXT("Successfully set GameServerAccessToken. Use Verbose logging to print token."));
	UE_LOG(LogNodecraftGameServerEvents, Verbose, TEXT("Successfully set GameServerAccessToken to %s"), *Token);
	GameServerAccessToken = Token;
}

void UNodecraftStudioSessionManager::SetPlayerSession(const FPlayerSession& Session)
{
	PlayerSession = Session;
	SaveToDisk();
}

FString UNodecraftStudioSessionManager::GetPlayerId()
{
	return PlayerSession.PlayerData ? PlayerSession.PlayerData->GetId() : "";
}

FString UNodecraftStudioSessionManager::GetGameAccessToken()
{
	return GameAccessToken;
}

FString UNodecraftStudioSessionManager::GetPlayerAccessToken()
{
	return PlayerSession.Token;
}

FString UNodecraftStudioSessionManager::GetGameServerAccessToken()
{
	return GameServerAccessToken;
}

const FPlayerSession& UNodecraftStudioSessionManager::GetPlayerSession()
{
	return PlayerSession;
}

bool UNodecraftStudioSessionManager::IsGameServerAccessTokenSet()
{
	return GameServerAccessToken != "";
}

bool UNodecraftStudioSessionManager::IsGameAccessTokenSet()
{
	return GameAccessToken != "";
}

bool UNodecraftStudioSessionManager::IsPlayerSessionValid()
{
	return PlayerSession.IsValid();
}

bool UNodecraftStudioSessionManager::IsPlayerSessionExpired()
{
	return PlayerSession.DateExpires < FDateTime::UtcNow();
}

bool UNodecraftStudioSessionManager::SaveToDisk() const
{
	UNodecraftStudioSaveGame* SaveGameInstance = Cast<UNodecraftStudioSaveGame>(UGameplayStatics::CreateSaveGameObject(UNodecraftStudioSaveGame::StaticClass()));
	SaveGameInstance->SetGameAccessToken(GameAccessToken);
	SaveGameInstance->SetPlayerSession(PlayerSession);
	return UGameplayStatics::SaveGameToSlot(SaveGameInstance, SAVE_GAME_FILE_NAME, 0);
}

bool UNodecraftStudioSessionManager::LoadFromDisk()
{
	if (const UNodecraftStudioSaveGame* LoadedGameData = Cast<UNodecraftStudioSaveGame>(UGameplayStatics::LoadGameFromSlot(SAVE_GAME_FILE_NAME, 0)))
	{
		GameAccessToken = LoadedGameData->GetGameAccessToken();
		PlayerSession = LoadedGameData->GetPlayerSession();
		return true;
	}
	return false;
}
