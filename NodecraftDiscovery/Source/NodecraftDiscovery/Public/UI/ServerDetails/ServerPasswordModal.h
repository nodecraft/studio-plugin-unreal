// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "CommonInputTypeEnum.h"
#include "Services/ServerQueueService.h"
#include "ServerPasswordModal.generated.h"

class UServerSessionDataObject;
class UAlertMessage;
class UServerDataObject;
class UNodecraftButtonBase;
class UEditableTextBox;
/**
 * 
 */
UCLASS()
class NODECRAFTDISCOVERY_API UServerPasswordModal : public UCommonActivatableWidget
{
	GENERATED_BODY()

public:
	void Configure(UServerDataObject* InServerDataObject, FSimpleDelegate InOnClosed);

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeOnActivated() override;
	virtual void NativeOnDeactivated() override;
	virtual UWidget* NativeGetDesiredFocusTarget() const override;
	virtual bool NativeOnHandleBackAction() override;
	void UnregisterActionBindings();

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Details", meta = (BindWidget))
	UEditableTextBox* InputBox;
	
	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Details", meta = (BindWidget))
	UNodecraftButtonBase* ShowHidePasswordButton;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Details", meta = (BindWidget))
	UNodecraftButtonBase* CancelButton;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Details", meta = (BindWidget))
	UNodecraftButtonBase* JoinButton;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Details", meta = (BindWidget))
	UAlertMessage* AlertMessage;

	UPROPERTY(EditDefaultsOnly, Category="Nodecraft UI|Input", meta=(RowType="/Script/CommonUI.CommonInputActionDataBase"))
	FDataTableRowHandle JoinButtonActionData;

	UPROPERTY(EditDefaultsOnly, Category="Nodecraft UI|Input", meta=(RowType="/Script/CommonUI.CommonInputActionDataBase"))
	FDataTableRowHandle PasswordVisibilityToggleButtonActionData;
	
	FDelegateHandle TransitionToJoiningServerQueueHandle;

	UPROPERTY()
	UServerDataObject* ServerDataObject;
	
private:
	void RefreshActions(ECommonInputType InputType);
	void AttemptToJoinServer();
	void ToggleShowPassword();
	
	UFUNCTION()
	void OnGetServerSession(const UServerSessionDataObject* Session);

	UFUNCTION()
	void OnTextChanged(const FText& InText);
	
	UFUNCTION()
    void OnTextCommitted(const FText& InText, ETextCommit::Type CommitMethod);

	FSimpleDelegate OnClosed;
	FUIActionBindingHandle JoinButtonActionHandle;
	FUIActionBindingHandle PasswordVisibilityToggleButtonActionHandle;
};
