// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "ServerQueueIdleCheckModal.generated.h"

class UNodecraftButtonBase;
/**
 * 
 */
UCLASS()
class NODECRAFTDISCOVERY_API UServerQueueIdleCheckModal : public UCommonActivatableWidget
{
	GENERATED_BODY()

public:
	void Configure(FSimpleDelegate& OnClose);

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Queue", meta=(BindWidget))
	UNodecraftButtonBase* BackButton;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Queue", meta=(BindWidget))
	UNodecraftButtonBase* CloseButton;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Queue", meta=(BindWidget))
	UNodecraftButtonBase* ContinueButton;

private:
	void CancelQueueAndCloseModal(const FSimpleDelegate& OnClose);
};
