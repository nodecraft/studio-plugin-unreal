// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "CreateServerModal.generated.h"

class UNodecraftScrollBox;
class UImage;
class UNodecraftRadioButton;
class UNodecraftButtonBase;
/**
 * 
 */
UCLASS()
class NODECRAFTDISCOVERY_API UCreateServerModal : public UCommonActivatableWidget
{
	GENERATED_BODY()

public:
	void Configure(FSimpleDelegate OnModalClosed);

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeOnActivated() override;
	virtual void NativeOnDeactivated() override;
	virtual UWidget* NativeGetDesiredFocusTarget() const override;
	virtual bool NativeOnHandleBackAction() override;
	
	UPROPERTY(BlueprintReadOnly, Category="Nodecraft UI|Create Server Modal", meta=(BindWidget))
	UNodecraftButtonBase* PersonalServerButton;

	UPROPERTY(meta=(BindWidget))
	UNodecraftRadioButton* PersonalServerRadioButton;

	UPROPERTY(meta=(BindWidget))
	UImage* PersonalServerImage;

	UPROPERTY(meta=(BindWidget))
	UNodecraftButtonBase* GetPersonalServerButton;

	UPROPERTY(BlueprintReadOnly, Category="Nodecraft UI|Create Server Modal", meta=(BindWidget))
	UNodecraftButtonBase* NodecraftProServerButton;

	UPROPERTY(meta=(BindWidget))
	UNodecraftRadioButton* NodecraftProRadioButton;

	UPROPERTY(meta=(BindWidget))
	UImage* NodecraftProImage;

	UPROPERTY(meta=(BindWidget))
	UNodecraftButtonBase* GetNodecraftProButton;

	UPROPERTY(meta=(BindWidget))
	UNodecraftScrollBox* ScrollBox;
	
	UPROPERTY(meta=(BindWidget))
	UNodecraftButtonBase* CloseButton;

	UPROPERTY(meta=(BindWidget))
	UNodecraftButtonBase* CancelButton;

	UPROPERTY(meta=(BindWidget))
	UNodecraftButtonBase* NextButton;

	UPROPERTY(EditDefaultsOnly, Category="Nodecraft UI|Input", meta=(RowType="/Script/CommonUI.CommonInputActionDataBase"))
	FDataTableRowHandle NextActionData;

	UPROPERTY(EditDefaultsOnly, Category="Nodecraft UI|Style")
	FLinearColor SelectedImageTint;

	UPROPERTY(EditDefaultsOnly, Category="Nodecraft UI|Style")
	FLinearColor UnselectedImageTint;

private:
	void SelectPersonalServer();
	void SelectNodecraftProServer();
	void UpdateActionBindings(ECommonInputType CurrentInputType);

	FUIActionBindingHandle NextActionHandle;
};
