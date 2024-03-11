// Fill out your copyright notice in the Description page of Project Settings.


DEFINE_LOG_CATEGORY_STATIC(DiscoverySessionManager, Display, All)

#include "API/DiscoverySessionManagerSubsystem.h"

#include "API/DiscoverySaveGame.h"
#include "DeveloperSettings/DiscoveryAPISettings.h"
#include "Kismet/GameplayStatics.h"

void UDiscoverySessionManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	SetGameAccessToken(GetDefault<UDiscoveryAPISettings>()->GetPublicGameToken());
}

void UDiscoverySessionManager::Deinitialize()
{
	Super::Deinitialize();
}

#if WITH_EDITOR
void UDiscoverySessionManager::SetPlayerAccessToken(const FString& NewToken)
{
	PlayerSession.Token = NewToken;
	SaveToDisk();
}
#endif

void UDiscoverySessionManager::SetGameAccessToken(FString Token)
{
	GameAccessToken = Token;
}

void UDiscoverySessionManager::SetPlayerSession(const FPlayerSession& Session)
{
	PlayerSession = Session;
	SaveToDisk();
}

FString UDiscoverySessionManager::GetPlayerId()
{
	return PlayerSession.PlayerData ? PlayerSession.PlayerData->GetId() : "";;
}

FString UDiscoverySessionManager::GetGameAccessToken()
{
	return GameAccessToken;
}

FString UDiscoverySessionManager::GetPlayerAccessToken()
{
	return PlayerSession.Token;
}

const FPlayerSession& UDiscoverySessionManager::GetPlayerSession()
{
	return PlayerSession;
}

bool UDiscoverySessionManager::IsGameAccessTokenSet()
{
	return GameAccessToken != "";
}

bool UDiscoverySessionManager::IsPlayerSessionValid()
{
	return PlayerSession.IsValid();
}

bool UDiscoverySessionManager::IsPlayerSessionExpired()
{
	return PlayerSession.DateExpires < FDateTime::UtcNow();
}

bool UDiscoverySessionManager::SaveToDisk() const
{
	UDiscoverySaveGame* SaveGameInstance = Cast<UDiscoverySaveGame>(UGameplayStatics::CreateSaveGameObject(UDiscoverySaveGame::StaticClass()));
	SaveGameInstance->SetGameAccessToken(GameAccessToken);
	SaveGameInstance->SetPlayerSession(PlayerSession);
	return UGameplayStatics::SaveGameToSlot(SaveGameInstance, "DiscoverySaveGame", 0);
}

bool UDiscoverySessionManager::LoadFromDisk()
{
	if (const UDiscoverySaveGame* LoadedGameData = Cast<UDiscoverySaveGame>(UGameplayStatics::LoadGameFromSlot("DiscoverySaveGame", 0)))
	{
		GameAccessToken = LoadedGameData->GetGameAccessToken();
		PlayerSession = LoadedGameData->GetPlayerSession();
		return true;
	}
	return false;
}
