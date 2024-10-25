// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ServiceDelegates.h"
#include "Interfaces/IHttpRequest.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "ServerQueueService.generated.h"

class UCommonActivatableWidget;
class UServerDataObject;
class UServerSessionDataObject;
class UServerQueueDataObject;
class UServerQueueTokenDataObject;

DECLARE_DELEGATE_ThreeParams(FJoinServerDelegate, UServerQueueTokenDataObject* /*QueueTokenObject*/, bool /*bSuccess*/, TOptional<FText> /*Error*/);
DECLARE_DELEGATE_ThreeParams(FGetServerQueueDelegate, UServerQueueDataObject* /*QueueObject*/, bool /*bSuccess*/, TOptional<FText> /*Error*/);
DECLARE_DELEGATE_OneParam(FRenewServerQueueDelegate, UServerQueueTokenDataObject* /*QueueTokenObject*/);
DECLARE_DELEGATE_OneParam(FServerQueueErrorDelegate, const FText& /*ErrorText*/);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGetServerConnectionString, const FString&, ConnectionString);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGetServerSessionDelegate, const UServerSessionDataObject*, ServerSession);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnUserMustAcceptServerConsents, UServerDataObject* /*Server*/, FText /*Error*/);

/**
 * 
 */
UCLASS()
class NODECRAFTDISCOVERY_API UServerQueueService : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	static UServerQueueService* Get(const UWorld* World)
	{
		if (World)
		{
			return World->GetSubsystem<UServerQueueService>();
		}
		return nullptr;
	}
	
	void CreateJoinServerDelegate(FHttpRequestCompleteDelegate& ReqCallbackOut, UServerDataObject* Server);
	void CreateGetServerQueueDelegate(const FGetServerQueueDelegate& OnComplete, FHttpRequestCompleteDelegate& ReqCallbackOut);
	void CreateRenewServerQueueDelegate(FHttpRequestCompleteDelegate& ReqCallbackOut);
	void CreateCancelServerQueueDelegate(const FSimpleServiceResponseDelegate& OnComplete, FHttpRequestCompleteDelegate& ReqCallbackOut);

	bool JoinServer(UServerDataObject* Server, const TOptional<FString>& ServerPassword = TOptional<FString>());
	bool GetServerQueue(FGetServerQueueDelegate& OnComplete);
	bool RenewServerQueue();
	bool CancelServerQueue();
	bool CancelServerQueue(FSimpleServiceResponseDelegate& OnComplete);

	UFUNCTION()
	void PollServerQueue();

	void StartPollingServerQueue();
	void StopPollingServerQueue();
	void AddGetServerQueueDelegate(FGetServerQueueDelegate Delegate);
	void RemoveGetServerQueueDelegate();

	FRenewServerQueueDelegate OnRenewServerQueue;
	
	FServerQueueErrorDelegate OnServerQueueError;

	FSimpleMulticastDelegate OnStartedPollingServerQueue;

	FSimpleMulticastDelegate OnServerQueueCancelled;

	FSimpleMulticastDelegate OnPasswordIncorrect;

	FSimpleMulticastDelegate OnIdleCheckinRequired;

	// You only need to use either OnGetServerSession or OnGetServerConnectionString.
	// OnGetServerConnectionString is more convenient for most use cases.
	UPROPERTY(BlueprintAssignable, Category = "Nodecraft Services")
	FGetServerSessionDelegate OnGetServerSession;
	
	UPROPERTY(BlueprintAssignable, Category = "Nodecraft Services")
	FGetServerConnectionString OnGetServerConnectionString;

	FOnUserMustAcceptServerConsents OnUserMustAcceptServerConsents;

protected:
	FTimerDelegate ServerQueuePollTimerDelegate;
	FTimerHandle ServerQueuePollTimerHandle;
	FTimerDelegate CancelQueueTimerDelegate;
	FTimerHandle CancelQueueTimerHandle;

private:
	// The current server queue token we are using to poll the server queue
	UPROPERTY()
	const UServerQueueTokenDataObject* ServerQueueTokenDataObject;

	// The latest server queue data we've received
	// Containts info on the queue position, estimated wait time, and server session data, which if present means we are ready to join the server
	UPROPERTY()
	UServerQueueDataObject* ServerQueueDataObject;

	// We need to keep this private and use an accessor to make sure that if, by some slim chance,
	// our UI hangs while we are opening the server queue modal long enough for the server queue to be retrieved,
	// we want to make sure that as soon as the UI loads it can get whatever the latest data is.
	FGetServerQueueDelegate OnGetServerQueue;

};
