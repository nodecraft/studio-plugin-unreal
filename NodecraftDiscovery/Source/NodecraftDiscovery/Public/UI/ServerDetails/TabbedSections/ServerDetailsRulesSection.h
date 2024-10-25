// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "CommonTextBlock.h"
#include "ServerDetailsSection.h"
#include "Models/ServerDataObject.h"
#include "UI/Common/ConsentLabelButton.h"
#include "UI/Common/NodecraftScrollBox.h"
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
	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Details", meta = (BindWidget))
	UNodecraftLoadGuard* LoadGuard;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Details", meta = (BindWidget))
	UCommonTextBlock* RulesText;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Details", meta = (BindWidget))
	UCommonBorder* ConsentExplanation;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Details", meta = (BindWidget))
	UConsentLabelButton* ConsentLabelButton;
	
	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Details", meta = (BindWidget))
	UNodecraftScrollBox* RulesScrollBox;
	
	// Consent related styles
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Nodecraft UI|Styles")
	TSoftObjectPtr<UMaterialInterface> ConsentGrantedBackgroundMaterial;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Nodecraft UI|Styles")
	TSoftObjectPtr<UMaterialInterface> ConsentNotGrantedBackgroundMaterial;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Nodecraft UI|Styles")
	TSoftClassPtr<UCommonTextStyle> ConsentGrantedTextStyle;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Nodecraft UI|Styles")
	TSoftClassPtr<UCommonTextStyle> ConsentNotGrantedTextStyle;

	UPROPERTY()
	UServerDataObject* ServerDataObject;
	
public:
	void RefreshConsentLabelButton(const URulesDataObject* Rules);

	// Sets server data for this section. Expects to have a URulesDataObject.
	// Will log an error if the server data object or rules is null.
	virtual void SetServerData(UServerDataObject* InServerDataObject) override;

	virtual void NativeOnActivated() override;

	virtual void NativeOnDeactivated() override;
	
	void SetLoading(bool bLoading);

	UFUNCTION()
	void OnConsentButtonClicked();
	
	virtual void NativeConstruct() override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Nodecraft UI|Input", meta=(RowType="/Script/CommonUI.CommonInputActionDataBase"))
	FDataTableRowHandle AcceptConsentsActionData;

private:
	void RefreshSignConsentsAction();
	void UpdateActionBindings(ECommonInputType CurrentInputType);

	FUIActionBindingHandle ScrollRulesActionHandle;

	FUIActionBindingHandle AcceptConsentsActionHandle;
};
