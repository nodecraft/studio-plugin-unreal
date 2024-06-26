﻿// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CommonButtonBase.h"
#include "CommonInputBaseTypes.h"
#include "HAL/Platform.h"
#include "Internationalization/Text.h"
#include "UObject/UObjectGlobals.h"

#include "NodecraftButtonBase.generated.h"

class UObject;
struct FFrame;

UCLASS(Abstract, BlueprintType, Blueprintable)
class NODECRAFTDISCOVERY_API UNodecraftButtonBase : public UCommonButtonBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetButtonText(const FText& InText);

	FText GetButtonText() const;
	
	virtual void SetIsEnabled(bool bInIsEnabled) override;
	virtual void NativeOnSelected(bool bBroadcast) override;
	virtual void NativeOnDeselected(bool bBroadcast) override;
	
protected:
	// UUserWidget interface
	virtual void NativePreConstruct() override;
	// End of UUserWidget interface

	// UCommonButtonBase interface
	virtual void UpdateInputActionWidget() override;
	virtual void OnInputMethodChanged(ECommonInputType CurrentInputType) override;
	// End of UCommonButtonBase interface

	void RefreshButtonText();
	
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateButtonText(const FText& InText);

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateButtonStyle();
	
private:
	UPROPERTY(EditAnywhere, Category="Button", meta=(InlineEditConditionToggle))
	uint8 bOverride_ButtonText : 1;
	
	UPROPERTY(EditAnywhere, Category="Button", meta=( editcondition="bOverride_ButtonText" ))
	FText ButtonText;
};