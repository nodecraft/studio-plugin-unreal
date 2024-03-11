// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "UserSettingsDataPrivacy.generated.h"

class UCommonTextBlock;
class UCommonTextStyle;
class UNodecraftButtonBase;
class UNodecraftLoadGuard;
class UNodecraftRadioButton;
class UAlertMessage;
/**
 * 
 */
UCLASS()
class NODECRAFTDISCOVERY_API UUserSettingsDataPrivacy : public UCommonActivatableWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	virtual void NativeOnActivated() override;

	void UpdateStyles();

	UPROPERTY(EditDefaultsOnly)
	TSoftClassPtr<UCommonTextStyle> SelectedTextStyle;

	UPROPERTY(EditDefaultsOnly)
	TSoftClassPtr<UCommonTextStyle> NormalTextStyle;

	UPROPERTY(EditDefaultsOnly)
	FColor ButtonStateImageSelectedColor;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UNodecraftLoadGuard* LoadGuard;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UAlertMessage* AlertMessage;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UNodecraftRadioButton* AnalyticsOptIn;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UNodecraftRadioButton* AnalyticsOptOut;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UNodecraftButtonBase* SaveSettingsButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UCommonTextBlock* OptInText;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UCommonTextBlock* OptOutText;
};
