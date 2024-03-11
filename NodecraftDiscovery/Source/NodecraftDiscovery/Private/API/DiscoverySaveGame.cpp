// Fill out your copyright notice in the Description page of Project Settings.


#include "API/DiscoverySaveGame.h"

FString UDiscoverySaveGame::GetGameAccessToken() const
{
	return GameAccessToken;
}

void UDiscoverySaveGame::SetGameAccessToken(const FString& InGameAccessToken)
{
	this->GameAccessToken = InGameAccessToken;
}

FString UDiscoverySaveGame::GetPlayerAccessToken() const
{
	return PlayerAccessToken;
}

void UDiscoverySaveGame::SetPlayerAccessToken(const FString& InPlayerAccessToken)
{
	this->PlayerAccessToken = InPlayerAccessToken;
}

FPlayerSession UDiscoverySaveGame::GetPlayerSession() const
{
	return PlayerSession;
}

void UDiscoverySaveGame::SetPlayerSession(const FPlayerSession& InPlayerSession)
{
	this->PlayerSession = InPlayerSession;
}
