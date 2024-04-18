// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
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

public:
	void GoToMainPage() const;

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UNodecraftButtonBase* BackButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UCommonActivatableWidgetSwitcher* WidgetSwitcher;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UUserSettingsMainPage* MainPage;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UUserSettingsServerRegion* ServerRegionPage;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UUserSettingsAccountReputation* AccountReputationPage;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UUserSettingsDataPrivacy* DataPrivacyPage;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UUserSettingsLegal* LegalPage;
};
