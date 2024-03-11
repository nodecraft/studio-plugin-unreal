// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "TermsOfServiceModal.generated.h"

struct FPlayerConsents;
class URulesDataObject;
class UCommonTextBlock;
class UNodecraftButtonBase;
/**
 * 
 */
UCLASS()
class NODECRAFTDISCOVERY_API UTermsOfServiceModal : public UCommonActivatableWidget
{
	GENERATED_BODY()

public:
	void Configure(const FPlayerConsents& Consents, FSimpleDelegate ClosePopupDelegate);

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UNodecraftButtonBase* CloseButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UCommonTextBlock* AgreedToTermsDateText;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UCommonTextBlock* SystemLegalText;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UCommonTextBlock* GameLegalText;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UCommonTextBlock* EmailAgreementText;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UNodecraftButtonBase* CancelButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UNodecraftButtonBase* SubmitButton;
};
