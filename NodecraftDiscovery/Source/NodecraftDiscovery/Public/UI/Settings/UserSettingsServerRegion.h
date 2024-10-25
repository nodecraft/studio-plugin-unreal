// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "CommonInputTypeEnum.h"
#include "UserSettingsServerRegion.generated.h"

class UNodecraftRadioButtonGroup;
class UAlertMessage;class UNodecraftLoadGuard;
class UServerRegionDataObject;
class UCheckBox;
class UNodecraftButtonBase;
class UVerticalBox;
class UServerRegionRow;


/**
 * 
 */
UCLASS()
class NODECRAFTDISCOVERY_API UUserSettingsServerRegion : public UCommonActivatableWidget
{
	GENERATED_BODY()

public:
	UAlertMessage* GetAlertMessage();

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual UWidget* NativeGetDesiredFocusTarget() const override;

	// Create row widgets. Mark current region as selected. Mark default region.
	void CreateServerRegionRows(const TArray<UServerRegionDataObject*>& Regions);
	
	// Control the state of Change Region Button with the state of the checkbox.
	UFUNCTION()
	void OnDefaultRegionCheckboxStateChanged(bool bIsChecked);

	void UpdateChangeRegionButtonEnabledState();

	// Custom navigation function used to navigate UP to the last server region in the vertical box from the Set As Default checkbox.
	// This is used to set up navigation rules in the widget blueprint.
	UFUNCTION(BlueprintCallable, Category = "Nodecraft UI|User Settings")
	UWidget* NavRule_NavigateToLastServerRegion(EUINavigation InNavigation);

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|User Settings", meta = (BindWidget))
	UNodecraftLoadGuard* LoadGuard;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|User Settings", meta = (BindWidget))
	UAlertMessage* AlertMessage;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|User Settings", meta = (BindWidget))
	UNodecraftRadioButtonGroup* RadioButtonGroup;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|User Settings", meta = (BindWidget))
	UCheckBox* SetAsDefaultCheckBox;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|User Settings", meta = (BindWidget))
	UNodecraftButtonBase* ChangeRegionButton;

	UPROPERTY(EditDefaultsOnly, Category = "Nodecraft UI|Input", meta=(RowType="/Script/CommonUI.CommonInputActionDataBase"))
	FDataTableRowHandle ConfirmInputActionData;
	
private:
	void UpdateActionBindings(ECommonInputType CurrentInputType);
	
	UFUNCTION()
	void UpdateSelectedRegion(int32 Index);
	
	FString CurrentRegion;
	FString SelectedRegion;
	FString DefaultRegion;

	FUIActionBindingHandle ConfirmUIActionHandle;
};
