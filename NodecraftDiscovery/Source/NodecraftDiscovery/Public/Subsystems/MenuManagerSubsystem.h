// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DataTypes/PlayerConnectionSubjects.h"
#include "Subsystems/EngineSubsystem.h"
#include "Engine/Engine.h"
#include "MenuManagerSubsystem.generated.h"


enum class ELinkType : uint8;
class UServerQueueTokenDataObject;
class UServerQueueDataObject;
class UFriendDataObject;
class UCommonActivatableWidget;
struct FPlayerConsents;
class UNodecraftStudioMainMenu;
class UServerDataObject;

typedef TMap<FString, FText> FServerDetailsArgument;

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnRequestShowServerDetailsModal, UServerDataObject* /* Server*/, TOptional<FServerDetailsArgument> /* Argument */);
DECLARE_DELEGATE_TwoParams(FOnPushConsentsPopupDelegate, TSubclassOf<UCommonActivatableWidget> /*ModalClass*/, FPlayerConsents /*Consents*/);
DECLARE_DELEGATE_ThreeParams(FOnPushServerInvitesPopupDelegate, TSubclassOf<UCommonActivatableWidget> /*ModalClass*/, TArray<UServerDataObject*> /*Servers*/, const UFriendDataObject* /*Invitee*/);
DECLARE_MULTICAST_DELEGATE(FOnPushJoiningServerQueuePopupDelegate);
DECLARE_DELEGATE_OneParam(FOnPushServerPasswordPopupDelegate, UServerDataObject* /*Server*/);
DECLARE_DELEGATE_OneParam(FServerQueueErrorDelegate, const FText& /*ErrorText*/);
DECLARE_DELEGATE_OneParam(FOnPushSimpleModalDelegate, TSubclassOf<UCommonActivatableWidget> /*Modal*/);
DECLARE_DELEGATE_ThreeParams(FOnPushExternalRedirectModalDelegate, TSubclassOf<UCommonActivatableWidget> /*Modal*/, FString /*URL*/, ELinkType /*LinkType*/);
DECLARE_DELEGATE_ThreeParams(FOnPushInternalRedirectModalDelegate, TSubclassOf<UCommonActivatableWidget> /*Modal*/, EPlayerConnectionSubject /*Subject*/, TOptional<FString> /*ServerId*/);


UCLASS()
class NODECRAFTDISCOVERY_API UMenuManagerSubsystem : public UEngineSubsystem
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	UNodecraftStudioMainMenu* MainMenu;

public:
	static UMenuManagerSubsystem& Get() { return *GEngine->GetEngineSubsystem<UMenuManagerSubsystem>(); }
	
	void ShowServerDetails(UServerDataObject* ServerDataObject, TOptional<FServerDetailsArgument> Argument = TOptional<FServerDetailsArgument>());
	void ShowPlayerConsents(TSubclassOf<UCommonActivatableWidget> Modal, const FPlayerConsents& Consents);
	void ShowServerInvites(TSubclassOf<UCommonActivatableWidget> Modal, const TArray<UServerDataObject*>& Servers, const UFriendDataObject* Invitee);
	void ShowJoiningServerQueue();
	void ShowServerPasswordModal(UServerDataObject* ServerDataObject);
	void ShowCreateServerModal(TSoftClassPtr<UCommonActivatableWidget> ModalClass);
	void ShowRedirectModal(FString QrCodeUrl, ELinkType LinkType);
	void ShowInternalRedirectModal(EPlayerConnectionSubject Subject);
	void ShowServerSettingsRedirectModal(const FString& ServerId);
	void ShowIdleCheckModal(TSoftClassPtr<UCommonActivatableWidget> ModalClass);

	void ClearPopupStack();

	FOnRequestShowServerDetailsModal OnRequestShowServerDetailsModal;
	FOnPushConsentsPopupDelegate OnPushConsentsPopupDelegate;
	FOnPushServerInvitesPopupDelegate OnPushServerInvitesPopupDelegate;
	FOnPushJoiningServerQueuePopupDelegate OnPushJoiningServerQueuePopupDelegate;
	FOnPushServerPasswordPopupDelegate OnPushServerPasswordPopupDelegate;
	FServerQueueErrorDelegate OnServerQueueError;
	FOnPushSimpleModalDelegate OnPushCreateServerPopupDelegate;
	FOnPushSimpleModalDelegate OnPushIdleCheckPopupDelegate;
	FOnPushInternalRedirectModalDelegate OnPushInternalRedirectPopupDelegate;
	FOnPushExternalRedirectModalDelegate OnPushExternalRedirectModalDelegate;

	FSimpleDelegate OnClearPopupStack;
};
