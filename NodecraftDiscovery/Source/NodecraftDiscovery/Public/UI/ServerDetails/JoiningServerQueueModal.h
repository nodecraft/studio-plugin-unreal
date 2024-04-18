// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

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

UCLASS()
class NODECRAFTDISCOVERY_API UJoiningServerQueueModal : public UCommonActivatableWidget
{
	GENERATED_BODY()

public:
	void Configure(FSimpleDelegate& OnPopupClosed);

	virtual void NativeOnInitialized() override;
protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UCommonTextBlock* QueuePositionText;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UProgressBar* ProgressBar;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UCommonTextBlock* EstimatedWaitTime;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UNodecraftButtonBase* CancelButton;

private:
	int32 WaitTimeInSeconds;
	FTimerHandle CountdownTimerHandle;
	FSimpleDelegate ClosePopupDelegate;
	
	void CountdownTimerTick();
	
	UFUNCTION()
	void OnGetServerSession(const UServerSessionDataObject* Session);

	void UpdateWaitTimeUI(float TimeLeftSeconds);
};
