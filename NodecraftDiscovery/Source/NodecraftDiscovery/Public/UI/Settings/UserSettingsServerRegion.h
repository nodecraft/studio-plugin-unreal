// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "UserSettingsServerRegion.generated.h"

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
	virtual void NativeConstruct() override;

	// Create row widgets. Mark current region as selected. Mark default region.
	void CreateServerRegionRows(const TArray<UServerRegionDataObject*>& Regions);
	
	// Control the state of Change Region Button with the state of the checkbox.
	UFUNCTION()
	void OnDefaultRegionCheckboxStateChanged(bool bIsChecked);

	void UpdateChangeRegionButtonEnabledState();

	UPROPERTY(EditDefaultsOnly)
	TSoftClassPtr<UServerRegionRow> RegionRowWidget;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UNodecraftLoadGuard* LoadGuard;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UAlertMessage* AlertMessage;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UVerticalBox* RegionsVerticalBox;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UCheckBox* SetAsDefaultCheckBox;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UNodecraftButtonBase* ChangeRegionButton;

	FString CurrentRegion;
	FString SelectedRegion;
	FString DefaultRegion;
};
