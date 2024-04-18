// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ServiceDelegates.h"
#include "Subsystems/EngineSubsystem.h"
#include "InvitesService.generated.h"


class UInviteDataObject;
class UServerDataObject;
DECLARE_DELEGATE_ThreeParams(FOnListInvitableServers, TArray<UServerDataObject*> /*Servers*/, bool /*bSuccess*/, TOptional<FText> /*Error*/);
DECLARE_DELEGATE_ThreeParams(FOnCreateInvite, const UInviteDataObject* /*Invite*/, bool /*bSuccess*/, TOptional<FText> /*Error*/);

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
	bool CreateInvite(const FString& ServerID, const FString& FriendID, FOnCreateInvite OnComplete);
};
