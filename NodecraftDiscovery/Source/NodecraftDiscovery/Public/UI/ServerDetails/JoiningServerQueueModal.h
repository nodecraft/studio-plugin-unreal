// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "JoiningServerQueueModal.generated.h"

class UServerSessionDataObject;
class UServerQueueTokenDataObject;
class UServerQueueDataObject;
class UNodecraftButtonBase;
class UProgressBar;
class UCommonTextBlock;
/**
 * 
 */
UCLASS()
class NODECRAFTDISCOVERY_API UJoiningServerQueueModal : public UCommonActivatableWidget
{
	GENERATED_BODY()

public:
	void Configure(const UServerQueueDataObject* ServerQueueDataObject, const UServerQueueTokenDataObject* InServerQueueTokenDataObject, FSimpleDelegate& OnPopupClosed);

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UCommonTextBlock* QueuePositionText;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UProgressBar* ProgressBar;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UCommonTextBlock* EstimatedWaitTime;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UNodecraftButtonBase* CancelButton;

	UPROPERTY()
	const UServerQueueTokenDataObject* ServerQueueTokenDataObject;

private:
	int32 WaitTimeInSeconds;
	FTimerHandle CountdownTimerHandle;
	FSimpleDelegate OnClosed;
	
	void CountdownTimerTick();
	
	UFUNCTION()
	void OnGetServerSession(const UServerSessionDataObject* Session);

	void UpdateWaitTimeUI(float TimeLeftSeconds);
};
