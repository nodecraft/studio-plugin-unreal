// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ServiceDelegates.h"
#include "Subsystems/EngineSubsystem.h"
#include "InvitesService.generated.h"


class UServerDataObject;
DECLARE_DELEGATE_ThreeParams(FOnListInvitableServers, TArray<UServerDataObject*> /*Servers*/, bool /*bSuccess*/, TOptional<FText> /*Error*/);

/**
 * 
 */
UCLASS()
class NODECRAFTDISCOVERY_API UInvitesService : public UEngineSubsystem
{
	GENERATED_BODY()

public:
	static UInvitesService& Get() { return *GEngine->GetEngineSubsystem<UInvitesService>(); }

	bool AcceptInvite(const FString& InviteId, FSimpleServiceResponseDelegate OnComplete);
	bool DeclineInvite(const FString& InviteId, FSimpleServiceResponseDelegate OnComplete);
	bool ListInvitableServers(const FString& FriendID, FOnListInvitableServers OnComplete);
};
