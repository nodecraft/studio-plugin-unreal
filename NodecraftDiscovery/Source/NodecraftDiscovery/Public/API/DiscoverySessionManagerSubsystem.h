// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/EditableTextBox.h"
#include "Services/IdentService.h"
#include "UObject/Object.h"
#include "DiscoverySessionManagerSubsystem.generated.h"

/**
 * 
 */
UCLASS(Config=Nodecraft, DefaultConfig)
class NODECRAFTDISCOVERY_API UDiscoverySessionManager : public UEngineSubsystem
{
	GENERATED_BODY()

public:
	//~UEngineSubsystem interface
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	//~End of UEngineSubsystem interface

#if WITH_EDITOR
	void SetPlayerAccessToken(const FString& NewToken);
#endif
	
	static UDiscoverySessionManager& Get() { return *GEngine->GetEngineSubsystem<UDiscoverySessionManager>(); }

private:
	FString GameAccessToken;

	UPROPERTY()
	FPlayerSession PlayerSession;

	bool SaveToDisk() const;

public:
	void SetGameAccessToken(FString Token);
	void SetPlayerSession(const FPlayerSession& Session);
	FString GetPlayerId();

	// TODO: This should be automated
	bool LoadFromDisk();

	FString GetGameAccessToken();
	FString GetPlayerAccessToken();
	const FPlayerSession& GetPlayerSession();

	bool IsGameAccessTokenSet();
	bool IsPlayerSessionValid();
	bool IsPlayerSessionExpired();

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerAccessTokenUpdated, FString);
	FOnPlayerAccessTokenUpdated OnPlayerAccessTokenUpdated;
};
