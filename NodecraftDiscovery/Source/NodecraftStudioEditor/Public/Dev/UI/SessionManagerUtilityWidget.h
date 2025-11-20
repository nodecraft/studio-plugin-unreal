// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

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
class NODECRAFTSTUDIOEDITOR_API USessionManagerUtilityWidget : public UEditorUtilityWidget
{
	GENERATED_BODY()
	
	UFUNCTION()
	void SetPlayerToken();
	
	virtual void NativeConstruct() override;

protected:
	// Player Token
	UPROPERTY(meta=(BindWidget), BlueprintReadOnly, Category = "Nodecraft UI|Dev")
	UCommonTextBlock* CurrentPlayerTokenTextBlock;
	
	UPROPERTY(meta=(BindWidget), BlueprintReadOnly, Category = "Nodecraft UI|Dev")
	UEditableTextBox* NewPlayerTokenTextBox;

	UPROPERTY(meta=(BindWidget), BlueprintReadOnly, Category = "Nodecraft UI|Dev")
	UButton* SetNewPlayerTokenButton;
	
	// Auth / Ident
	UPROPERTY(meta=(BindWidget), BlueprintReadOnly, Category = "Nodecraft UI|Dev")
	UEditableTextBox* EmailAddressTextBox;

	UPROPERTY(meta=(BindWidget), BlueprintReadOnly, Category = "Nodecraft UI|Dev")
	UEditableTextBox* TokenTextBox;
	
	UPROPERTY(meta=(BindWidget), BlueprintReadOnly, Category = "Nodecraft UI|Dev")
	UButton* ManualChallengeButton;

	UPROPERTY(meta=(BindWidget), BlueprintReadOnly, Category = "Nodecraft UI|Dev")
	UButton* SendTokenButton;

	// JSON Response readouts
	UPROPERTY(meta=(BindWidget), BlueprintReadOnly, Category = "Nodecraft UI|Dev")
	UCommonTextBlock* ResponseTextBlock;

	UPROPERTY(meta=(BindWidget), BlueprintReadOnly, Category = "Nodecraft UI|Dev")
	UCommonTextBlock* ResponseStatusTextBlock;
};
