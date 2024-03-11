// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Services/IdentService.h"
#include "DiscoverySaveGame.generated.h"

/**
 * 
 */
UCLASS()
class NODECRAFTDISCOVERY_API UDiscoverySaveGame : public USaveGame
{
	GENERATED_BODY()

	UPROPERTY()
	FString GameAccessToken;

public:
	FString GetGameAccessToken() const;
	void SetGameAccessToken(const FString& InGameAccessToken);
	FString GetPlayerAccessToken() const;
	void SetPlayerAccessToken(const FString& InPlayerAccessToken);
	FPlayerSession GetPlayerSession() const;
	void SetPlayerSession(const FPlayerSession& InPlayerSession);

private:
	UPROPERTY()
	FString PlayerAccessToken;

	UPROPERTY()
	FPlayerSession PlayerSession = FPlayerSession();
};
