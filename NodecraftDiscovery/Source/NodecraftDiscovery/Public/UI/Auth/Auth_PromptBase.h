// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AuthScreen.h"
#include "CommonActivatableWidget.h"
#include "CommonUserWidget.h"
#include "CommonButtonBase.h"
#include "Components/TextBlock.h"
#include "Interfaces/IHttpRequest.h"
#include "Auth_PromptBase.generated.h"

/**
 * 
 */
UCLASS(Abstract, Blueprintable)
class NODECRAFTDISCOVERY_API UAuth_PromptBase : public UCommonActivatableWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	void DisplayError(const FText& InErrorText);

	FSimpleDelegate OnCallEnded;
	FSimpleDelegate OnCallBegan;

protected:
	UPROPERTY(meta=(BindWidget))
	UCommonButtonBase* SubmitButton;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* ErrorTextBlock;

	UPROPERTY()
	UAuthScreen* ParentScreen;

public:
	void SetParentScreen(UAuthScreen* const InParentScreen);

protected:
	UFUNCTION()
	void SubmitButtonPressed();

	virtual void SubmitRequest();
};
