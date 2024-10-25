// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "CommonInputTypeEnum.h"
#include "UserSettingsDataPrivacy.generated.h"

class UNodecraftRadioButtonGroup;
class UCommonTextStyle;
class UNodecraftButtonBase;
class UNodecraftLoadGuard;
class UAlertMessage;
/**
 * 
 */
UCLASS()
class NODECRAFTDISCOVERY_API UUserSettingsDataPrivacy : public UCommonActivatableWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeOnActivated() override;
	virtual UWidget* NativeGetDesiredFocusTarget() const override;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Data Privacy", meta = (BindWidget))
	UNodecraftLoadGuard* LoadGuard;
	
	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Data Privacy", meta = (BindWidget))
	UAlertMessage* AlertMessage;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Data Privacy", meta = (BindWidget))
	UNodecraftRadioButtonGroup* RadioButtonGroup;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Data Privacy", meta = (BindWidget))
	UNodecraftButtonBase* SaveSettingsButton;

	UPROPERTY(EditDefaultsOnly, Category = "Nodecraft UI|Input", meta=(RowType="/Script/CommonUI.CommonInputActionDataBase"))
	FDataTableRowHandle SaveInputActionData;

private:
	void UpdateActionBindings(ECommonInputType CurrentInputType);
	
	FUIActionBindingHandle SaveUIActionHandle;
};
