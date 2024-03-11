// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "NodecraftRadioButton.generated.h"

class UNodecraftButtonBase;
class UNodecraftRadioButton;
class UImage;

DECLARE_DELEGATE(FOnNodecraftRadioButtonClicked);
/**
 * 
 */
UCLASS()
class NODECRAFTDISCOVERY_API UNodecraftRadioButton : public UCommonActivatableWidget
{
	GENERATED_BODY()

public:
	FOnNodecraftRadioButtonClicked OnNodecraftRadioButtonClicked;

	void SetIsChecked(bool bIsChecked);
	bool GetIsChecked() const;

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(EditDefaultsOnly)
	FColor SelectedTint;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UNodecraftButtonBase* Button;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UNamedSlot* ContentSlot;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UImage* ButtonStateImage;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UImage* ButtonStateBackground;
};
