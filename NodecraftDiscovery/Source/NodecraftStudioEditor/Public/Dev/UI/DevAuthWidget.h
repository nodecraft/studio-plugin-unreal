// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonTextBlock.h"
#include "EditorUtilityWidget.h"
#include "JsonObjectWrapper.h"
#include "Components/Button.h"
#include "Components/ComboBoxString.h"
#include "Components/EditableTextBox.h"
#include "DevAuthWidget.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class NODECRAFTSTUDIOEDITOR_API UDevAuthWidget : public UEditorUtilityWidget
{
	GENERATED_BODY()
	
	void HitEndpoint(const FString URI, const FString HttpMethod, const FString HeaderToken, const FJsonObjectWrapper& RequestBody);
	
	UFUNCTION()
	void SendRequest();

	UFUNCTION()
    void StartManualChallenge();
    
    UFUNCTION()
    void SendToken();

	void RefreshHeaderComboBox();
	// virtual void NativeOnInitialized() override;

	virtual void NativeConstruct() override;

protected:
	UPROPERTY(meta=(BindWidget), BlueprintReadOnly, Category = "Nodecraft UI|Dev")
	UEditableTextBox* RootTextBox;

	UPROPERTY(meta=(BindWidget), BlueprintReadOnly, Category = "Nodecraft UI|Dev")
	UButton* SendRequestButton;

	UPROPERTY(meta=(BindWidget), BlueprintReadOnly, Category = "Nodecraft UI|Dev")
	UEditableTextBox* EndpointTextBox;

	UPROPERTY(meta=(BindWidget), BlueprintReadOnly, Category = "Nodecraft UI|Dev")
	UComboBoxString* HttpMethodSelector;

	UPROPERTY(meta=(BindWidget), BlueprintReadOnly, Category = "Nodecraft UI|Dev")
	UComboBoxString* HeaderTokenSelector;
	
	UPROPERTY(meta=(BindWidget), BlueprintReadOnly, Category = "Nodecraft UI|Dev")
	UEditableTextBox* JsonBodyTextBox;

	// JSON Response readouts
	UPROPERTY(meta=(BindWidget), BlueprintReadOnly, Category = "Nodecraft UI|Dev")
	UCommonTextBlock* ResponseTextBlock;

	UPROPERTY(meta=(BindWidget), BlueprintReadOnly, Category = "Nodecraft UI|Dev")
	UCommonTextBlock* ResponseStatusTextBlock;

	// Auth / Ident
	UPROPERTY(meta=(BindWidget), BlueprintReadOnly, Category = "Nodecraft UI|Dev")
	UEditableTextBox* EmailAddressTextBox;

	UPROPERTY(meta=(BindWidget), BlueprintReadOnly, Category = "Nodecraft UI|Dev")
	UEditableTextBox* TokenTextBox;
	
	UPROPERTY(meta=(BindWidget), BlueprintReadOnly, Category = "Nodecraft UI|Dev")
	UButton* ManualChallengeButton;

	UPROPERTY(meta=(BindWidget), BlueprintReadOnly, Category = "Nodecraft UI|Dev")
	UButton* SendTokenButton;
};
