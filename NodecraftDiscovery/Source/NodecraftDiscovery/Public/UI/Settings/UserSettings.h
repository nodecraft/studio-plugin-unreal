// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "CommonInputTypeEnum.h"
#include "CommonActivatableWidget.h"
#include "UserSettings.generated.h"

class UUserSettingsServerRegion;
class UUserSettingsLegal;
class UUserSettingsDataPrivacy;
class UUserSettingsAccountReputation;
class UUserSettingsMainPage;
class UCommonActivatableWidgetSwitcher;
class UNodecraftButtonBase;
class UNodecraftLoadGuard;
/**
 * 
 */
UCLASS()
class NODECRAFTDISCOVERY_API UUserSettings : public UCommonActivatableWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual UWidget* NativeGetDesiredFocusTarget() const override;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|User Settings", meta = (BindWidget))
	UNodecraftButtonBase* BackButton;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|User Settings", meta = (BindWidget))
	UCommonActivatableWidgetSwitcher* WidgetSwitcher;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|User Settings", meta = (BindWidget))
	UUserSettingsMainPage* MainPage;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|User Settings", meta = (BindWidget))
	UUserSettingsServerRegion* ServerRegionPage;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|User Settings", meta = (BindWidget))
	UUserSettingsAccountReputation* AccountReputationPage;
	
	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|User Settings", meta = (BindWidget))
	UUserSettingsDataPrivacy* DataPrivacyPage;
	
	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|User Settings", meta = (BindWidget))
	UUserSettingsLegal* LegalPage;

	UPROPERTY(EditDefaultsOnly, Category = "Nodecraft UI|Input", meta=(RowType="/Script/CommonUI.CommonInputActionDataBase"))
	FDataTableRowHandle BackButtonInputActionData;

private:
	void GoToMainPage();
	void UpdateActionBindings(ECommonInputType CurrentInputType);
	
	FUIActionBindingHandle BackButtonInputActionHandle;
	int32 CurrentPageIndex = 0;
};
