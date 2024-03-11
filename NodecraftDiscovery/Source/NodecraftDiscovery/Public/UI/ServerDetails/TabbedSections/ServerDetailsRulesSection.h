// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "CommonLoadGuard.h"
#include "CommonTextBlock.h"
#include "ServerDetailsSection.h"
#include "Components/Border.h"
#include "Models/ServerDataObject.h"
#include "UI/Common/ConsentLabelButton.h"
#include "UObject/Object.h"
#include "ServerDetailsRulesSection.generated.h"

/**
 * 
 */
UCLASS()
class NODECRAFTDISCOVERY_API UServerDetailsRulesSection : public UCommonActivatableWidget, public IServerDetailsSection
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UCommonLoadGuard* LoadGuard;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UCommonTextBlock* RulesText;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UConsentLabelButton* ConsentExplanation;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UConsentLabelButton* ConsentLabelButton;

	// Consent related styles
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TSoftObjectPtr<UMaterialInterface> ConsentGrantedBackgroundMaterial;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TSoftObjectPtr<UMaterialInterface> ConsentNotGrantedBackgroundMaterial;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TSoftClassPtr<UCommonTextStyle> ConsentGrantedTextStyle;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TSoftClassPtr<UCommonTextStyle> ConsentNotGrantedTextStyle;

	UPROPERTY()
	UServerDataObject* ServerDataObject;
	
public:
	void RefreshConsentLabelButton(const URulesDataObject* Rules);
	virtual void SetServerData(UServerDataObject* InServerDataObject) override;
	
	void SetLoading(bool bLoading);

	UFUNCTION()
	void OnConsentButtonClicked();
	
	virtual void NativeConstruct() override;
};
