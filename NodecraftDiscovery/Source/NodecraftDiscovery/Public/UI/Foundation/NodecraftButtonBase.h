// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CommonButtonBase.h"
#include "CommonInputBaseTypes.h"
#include "Internationalization/Text.h"
#include "UObject/UObjectGlobals.h"

#include "NodecraftButtonBase.generated.h"

class UObject;

UCLASS(Abstract, BlueprintType, Blueprintable)
class NODECRAFTDISCOVERY_API UNodecraftButtonBase : public UCommonButtonBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Nodecraft UI|Button")
	void SetButtonText(const FText& InText);

	FText GetButtonText() const;
	
	virtual void SetIsEnabled(bool bInIsEnabled) override;
	virtual void SetVisibility(ESlateVisibility InVisibility) override;

	FText GetInputActionNameOverride() const;
	
protected:
	virtual FReply NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent) override;
	virtual void NativeOnSelected(bool bBroadcast) override;
	virtual void NativeOnDeselected(bool bBroadcast) override;
	
	// UUserWidget interface
	virtual void NativePreConstruct() override;
	// End of UUserWidget interface

	// UCommonButtonBase interface
	virtual void BindTriggeringInputActionToClick() override;
	virtual void UpdateInputActionWidget() override;
	virtual void OnInputMethodChanged(ECommonInputType CurrentInputType) override;
	// End of UCommonButtonBase interface

	void RefreshButtonText();
	
	UFUNCTION(BlueprintImplementableEvent, Category = "Nodecraft UI|Button")
	void UpdateButtonText(const FText& InText);

	UFUNCTION(BlueprintImplementableEvent, Category = "Nodecraft UI|Button")
	void UpdateButtonStyle();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Nodecraft UI|Input", meta = (AllowPrivateAccess = true))
	FText InputActionNameOverride;

	// Assumes that this button should be visible when using mouse and keyboard.
	// If that's not the case, please implement your own custom logic.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Nodecraft UI|Input", meta = (AllowPrivateAccess = true))
	bool bHideWhenUsingGamepad = false;
	
private:
	UPROPERTY(EditAnywhere, Category="Nodecraft UI|Button", meta=(InlineEditConditionToggle))
	uint8 bOverride_ButtonText : 1;

	UPROPERTY(EditAnywhere, Category="Nodecraft UI|Button", meta=( EditCondition="bOverride_ButtonText", EditConditionHides ))
	bool bOverrideButtonTextWithInputActionDisplayName = true;
	
	UPROPERTY(EditAnywhere, Category="Nodecraft UI|Button", meta=( editcondition="bOverride_ButtonText" ))
	FText ButtonText;
};