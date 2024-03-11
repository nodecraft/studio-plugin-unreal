// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonTextBlock.h"
#include "EditorUtilityWidget.h"
#include "JsonObjectWrapper.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"

#include "SessionManagerUtilityWidget.generated.h"



/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class NODECRAFTDISCOVERYEDITOR_API USessionManagerUtilityWidget : public UEditorUtilityWidget
{
	GENERATED_BODY()
	
	UFUNCTION()
	void SetPlayerToken();
	
	virtual void NativeConstruct() override;

protected:
	// Player Token
	UPROPERTY(meta=(BindWidget), BlueprintReadOnly)
	UCommonTextBlock* CurrentPlayerTokenTextBlock;
	
	UPROPERTY(meta=(BindWidget), BlueprintReadOnly)
	UEditableTextBox* NewPlayerTokenTextBox;

	UPROPERTY(meta=(BindWidget), BlueprintReadOnly)
	UButton* SetNewPlayerTokenButton;
	
	// Auth / Ident
	UPROPERTY(meta=(BindWidget), BlueprintReadOnly)
	UEditableTextBox* EmailAddressTextBox;

	UPROPERTY(meta=(BindWidget), BlueprintReadOnly)
	UEditableTextBox* TokenTextBox;
	
	UPROPERTY(meta=(BindWidget), BlueprintReadOnly)
	UButton* ManualChallengeButton;

	UPROPERTY(meta=(BindWidget), BlueprintReadOnly)
	UButton* SendTokenButton;

	// JSON Response readouts
	UPROPERTY(meta=(BindWidget), BlueprintReadOnly)
	UCommonTextBlock* ResponseTextBlock;

	UPROPERTY(meta=(BindWidget), BlueprintReadOnly)
	UCommonTextBlock* ResponseStatusTextBlock;
};
