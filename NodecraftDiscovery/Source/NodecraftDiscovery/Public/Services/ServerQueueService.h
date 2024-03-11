// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ServiceDelegates.h"
#include "Interfaces/IHttpRequest.h"
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
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGetServerSessionDelegate, const UServerSessionDataObject*, ServerSession);

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
	
	void CreateJoinServerDelegate(const FJoinServerDelegate& OnComplete, FHttpRequestCompleteDelegate& ReqCallbackOut);
	void CreateGetServerQueueDelegate(const FGetServerQueueDelegate& OnComplete, FHttpRequestCompleteDelegate& ReqCallbackOut);
	void CreateRenewServerQueueDelegate(const FJoinServerDelegate& OnComplete, FHttpRequestCompleteDelegate& ReqCallbackOut);
	void CreateCancelServerQueueDelegate(const FSimpleServiceResponseDelegate& OnComplete, FHttpRequestCompleteDelegate& ReqCallbackOut);

	bool JoinServer(const FString& ServerId, const TOptional<FString>& ServerPassword, FJoinServerDelegate& OnComplete);
	bool GetServerQueue(const FString& Token, FGetServerQueueDelegate& OnComplete);
	bool RenewServerQueue(const FString& Token, FJoinServerDelegate& OnComplete);
	bool CancelServerQueue(const FString& Token, FSimpleServiceResponseDelegate& OnComplete);

	UFUNCTION()
	void PollServerQueue(const UServerQueueTokenDataObject* ServerQueueTokenDataObject);

	void StartPollingServerQueue(const UServerQueueTokenDataObject* TokenDataObject);
	void StopPollingServerQueue();
	
	FRenewServerQueueDelegate OnRenewServerQueue;
	FGetServerQueueDelegate OnGetServerQueue;
	FServerQueueErrorDelegate OnServerQueueError;

	UPROPERTY(BlueprintAssignable)
	FGetServerSessionDelegate OnGetServerSession;

protected:
	FTimerDelegate ServerQueuePollTimerDelegate;
	FTimerHandle ServerQueuePollTimerHandle;
};
