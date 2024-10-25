// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "NodecraftAuthProvider.h"
#include "Subsystems/EngineSubsystem.h"
#include "ThirdParty/SteamAuthLibrary/sdk/public/steam/isteamuser.h"
#include "ThirdParty/SteamAuthLibrary/sdk/public/steam/steam_api_common.h"
#include "Engine/Engine.h"
#include "SteamAuthSubsystem.generated.h"


UCLASS()
class NODECRAFTSTEAMAUTH_API USteamAuthSubsystem : public UEngineSubsystem, public FTickableGameObject, public INodecraftAuthProvider
{
	GENERATED_BODY()

	// Begin Steam Callbacks
private:
	STEAM_CALLBACK(USteamAuthSubsystem, OnReceivedAuthTicket, GetTicketForWebApiResponse_t);
	// End Steam Callbacks

	bool bSteamInitialized = false;

	FOnIdentTokenReceived TokenReceivedCallback;

public:

	// Begin INodecraftAuthProvider
	virtual void GetIdentTokenAsync(FOnIdentTokenReceived OnTokenReceived) override;
	// End INodecraftAuthProvider

	// Begin UEngineSubsystem
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	// End UEngineSubsystem

	// Begin FTickableGameObject
	virtual void Tick(float DeltaTime) override;
	virtual ETickableTickType GetTickableTickType() const override { return ETickableTickType::Conditional; }
	virtual bool IsTickable() const override { return bSteamInitialized; }
	virtual TStatId GetStatId() const override { RETURN_QUICK_DECLARE_CYCLE_STAT(UIdentService, STATGROUP_Tickables); }
	// End FTickableGameObject
	
	static USteamAuthSubsystem& Get() { return *GEngine->GetEngineSubsystem<USteamAuthSubsystem>(); }

};
