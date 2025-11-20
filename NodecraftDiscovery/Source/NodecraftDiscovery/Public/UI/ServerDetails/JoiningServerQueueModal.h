// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "JoiningServerQueueModal.generated.h"

class UServerQueueIdleCheckModal;
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
	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Details", meta = (BindWidget))
	UCommonTextBlock* QueuePositionText;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Details", meta = (BindWidget))
	UProgressBar* ProgressBar;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Details", meta = (BindWidget))
	UCommonTextBlock* EstimatedWaitTime;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Details", meta = (BindWidget))
	UNodecraftButtonBase* CancelButton;

	UPROPERTY(EditDefaultsOnly, Category = "Nodecraft UI|Server Details")
	TSoftClassPtr<UServerQueueIdleCheckModal> ServerQueueIdleCheckModalClass;

private:
	int32 WaitTimeInSeconds;
	FTimerHandle CountdownTimerHandle;
	FSimpleDelegate ClosePopupDelegate;
	
	void CountdownTimerTick();
	
	UFUNCTION()
	void OnGetServerSession(const UServerSessionDataObject* Session);

	void UpdateWaitTimeUI(float TimeLeftSeconds);
	void ResetModal();
};
