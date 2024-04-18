// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AuthScreen.h"
#include "CommonActivatableWidget.h"
#include "CommonButtonBase.h"
#include "Auth_PromptBase.generated.h"

class UAlertMessage;
/**
 * 
 */
UCLASS(Abstract, Blueprintable)
class NODECRAFTDISCOVERY_API UAuth_PromptBase : public UCommonActivatableWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	void DisplayError(const FText& InErrorText);

	FSimpleDelegate OnCallEnded;
	FSimpleDelegate OnCallBegan;

protected:
	UPROPERTY(meta=(BindWidget))
	UCommonButtonBase* SubmitButton;

	UPROPERTY(meta=(BindWidget))
	UAlertMessage* Alert;

	UPROPERTY()
	UAuthScreen* ParentScreen;

public:
	void SetParentScreen(UAuthScreen* const InParentScreen);

protected:
	UFUNCTION()
	void SubmitButtonPressed();

	virtual void SubmitRequest();
};
