// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "UserSettingsLegal.generated.h"

class UTermsOfServiceModal;
class UNodecraftButtonBase;
/**
 * 
 */
UCLASS()
class NODECRAFTDISCOVERY_API UUserSettingsLegal : public UCommonActivatableWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UNodecraftButtonBase* TermsOfService;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UNodecraftButtonBase* PrivacyPolicy;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UNodecraftButtonBase* SoftwareLicenses;

	UPROPERTY(EditDefaultsOnly)
	TSoftClassPtr<UTermsOfServiceModal> TermsOfServiceModal;
};
