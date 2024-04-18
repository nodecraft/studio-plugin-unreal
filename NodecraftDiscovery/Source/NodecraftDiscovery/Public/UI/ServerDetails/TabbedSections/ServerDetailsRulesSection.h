// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "CommonTextBlock.h"
#include "ServerDetailsSection.h"
#include "Models/ServerDataObject.h"
#include "UI/Common/ConsentLabelButton.h"
#include "UObject/Object.h"
#include "ServerDetailsRulesSection.generated.h"

class UNodecraftLoadGuard;
class UCommonBorder;
/**
 * 
 */
UCLASS()
class NODECRAFTDISCOVERY_API UServerDetailsRulesSection : public UCommonActivatableWidget, public IServerDetailsSection
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UNodecraftLoadGuard* LoadGuard;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UCommonTextBlock* RulesText;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UCommonBorder* ConsentExplanation;

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

	// Sets server data for this section. Expects to have a URulesDataObject.
	// Will log an error if the server data object or rules is null.
	virtual void SetServerData(UServerDataObject* InServerDataObject) override;
	
	void SetLoading(bool bLoading);

	UFUNCTION()
	void OnConsentButtonClicked();
	
	virtual void NativeConstruct() override;
};
