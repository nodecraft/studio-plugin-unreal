// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AuthScreen.h"
#include "CommonActivatableWidget.h"
#include "Auth_PromptBase.generated.h"

class UNodecraftButtonBase;
class UAlertMessage;
/**
 * 
 */
UCLASS(Abstract, Blueprintable)
class NODECRAFTDISCOVERY_API UAuth_PromptBase : public UCommonActivatableWidget
{
	GENERATED_BODY()

public:
	void SetParentScreen(UAuthScreen* const InParentScreen);
	void DisplayError(const FText& InErrorText);

	FSimpleDelegate OnCallEnded;
	FSimpleDelegate OnCallBegan;

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	
	virtual void SubmitRequest();
	virtual void RefreshActions(ECommonInputType InputType);
	
	UFUNCTION()
	void SubmitButtonPressed();

	UPROPERTY(EditDefaultsOnly, Category = "Nodecraft UI|Input", meta=(RowType="/Script/CommonUI.CommonInputActionDataBase"))
	FDataTableRowHandle SubmitButtonActionData;
	
	UPROPERTY(meta=(BindWidget))
	UNodecraftButtonBase* SubmitButton;

	UPROPERTY(meta=(BindWidget))
	UAlertMessage* Alert;

	UPROPERTY()
	UAuthScreen* ParentScreen;

	FUIActionBindingHandle SubmitButtonActionHandle;
};
