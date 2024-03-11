// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/EngineSubsystem.h"
#include "MenuManagerSubsystem.generated.h"


class UServerQueueTokenDataObject;
class UServerQueueDataObject;
class UFriendDataObject;
class UCommonActivatableWidget;
struct FPlayerConsents;
class UDiscoveryMainMenu;
class UServerDataObject;

DECLARE_DELEGATE_OneParam(FOnRequestShowServerDetailsModal, UServerDataObject*);
DECLARE_DELEGATE_TwoParams(FOnPushConsentsPopupDelegate, TSubclassOf<UCommonActivatableWidget> /*ModalClass*/, FPlayerConsents /*Consents*/);
DECLARE_DELEGATE_ThreeParams(FOnPushServerInvitesPopupDelegate, TSubclassOf<UCommonActivatableWidget> /*ModalClass*/, TArray<UServerDataObject*> /*Servers*/, const UFriendDataObject* /*Invitee*/);
DECLARE_DELEGATE_TwoParams(FOnPushJoiningServerQueuePopupDelegate, const UServerQueueDataObject* /*ServerQueue*/, const UServerQueueTokenDataObject* /*ServerQueueTokenDataObject*/);
DECLARE_DELEGATE_OneParam(FOnPushServerPasswordPopupDelegate, const UServerDataObject* /*Server*/);
DECLARE_DELEGATE_OneParam(FServerQueueErrorDelegate, const FText& /*ErrorText*/);

UCLASS()
class NODECRAFTDISCOVERY_API UMenuManagerSubsystem : public UEngineSubsystem
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	UDiscoveryMainMenu* MainMenu;

public:
	static UMenuManagerSubsystem& Get() { return *GEngine->GetEngineSubsystem<UMenuManagerSubsystem>(); }
	void ShowServerDetails(UServerDataObject* ServerDataObject);
	void ShowPlayerConsents(TSubclassOf<UCommonActivatableWidget> Modal, const FPlayerConsents& Consents);
	void ShowServerInvites(TSubclassOf<UCommonActivatableWidget> Modal, const TArray<UServerDataObject*>& Servers, const UFriendDataObject* Invitee);
	void ShowJoiningServerQueue(const UWorld* World, const UServerDataObject* ServerDataObject, FString Password="");
	void ShowServerPasswordModal(const UServerDataObject* ServerDataObject);
	
	FOnRequestShowServerDetailsModal OnRequestShowServerDetailsModal;
	FOnPushConsentsPopupDelegate OnPushConsentsPopupDelegate;
	FOnPushServerInvitesPopupDelegate OnPushServerInvitesPopupDelegate;
	FOnPushJoiningServerQueuePopupDelegate OnPushJoiningServerQueuePopupDelegate;
	FOnPushServerPasswordPopupDelegate OnPushServerPasswordPopupDelegate;
	FServerQueueErrorDelegate OnServerQueueError;
};
