// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/EditableTextBox.h"
#include "Services/IdentService.h"
#include "UObject/Object.h"
#include "NodecraftStudioSessionManagerSubsystem.generated.h"

/**
 * 
 */
UCLASS(Config=Nodecraft, DefaultConfig)
class NODECRAFTDISCOVERY_API UNodecraftStudioSessionManager : public UEngineSubsystem
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
	
	static UNodecraftStudioSessionManager& Get() { return *GEngine->GetEngineSubsystem<UNodecraftStudioSessionManager>(); }

private:
	FString GameAccessToken;
	FString GameServerAccessToken;

	UPROPERTY()
	FPlayerSession PlayerSession;

	bool SaveToDisk() const;

public:
	void SetGameAccessToken(FString Token);
	void SetGameServerAccessToken(FString Token);
	void SetPlayerSession(const FPlayerSession& Session);
	FString GetPlayerId();

	// TODO: This should be automated
	bool LoadFromDisk();

	FString GetGameAccessToken();
	FString GetPlayerAccessToken();
	FString GetGameServerAccessToken();
	const FPlayerSession& GetPlayerSession();

	bool IsGameServerAccessTokenSet();
	bool IsGameAccessTokenSet();
	bool IsPlayerSessionValid();
	bool IsPlayerSessionExpired();

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerAccessTokenUpdated, FString);
	FOnPlayerAccessTokenUpdated OnPlayerAccessTokenUpdated;
};
