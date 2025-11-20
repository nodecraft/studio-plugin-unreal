// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "UserSettingsMainPage.generated.h"

class UIconTextLoadingButton;
class UCommonTextBlock;
class UImage;
class UAsyncImage;

DECLARE_DELEGATE_OneParam(FOnButtonClicked, int32 /* TabIndex */);

/**
 * 
 */
UCLASS()
class NODECRAFTDISCOVERY_API UUserSettingsMainPage : public UCommonActivatableWidget
{
	GENERATED_BODY()

public:
	FOnButtonClicked OnButtonClicked;
	
protected:
	virtual void NativeConstruct() override;
	virtual UWidget* NativeGetDesiredFocusTarget() const override;
	
	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|User Settings", meta = (BindWidget))
	UAsyncImage* PlayerImage;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|User Settings", meta = (BindWidget))
	UImage* IdentTypeImage;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|User Settings", meta = (BindWidget))
	UCommonTextBlock* Username;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|User Settings", meta = (BindWidget))
	UIconTextLoadingButton* ChangeServerRegionButton;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|User Settings", meta = (BindWidget))
	UIconTextLoadingButton* PlayerReputationButton;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|User Settings", meta = (BindWidget))
	UIconTextLoadingButton* CustomerSupportButton;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|User Settings", meta = (BindWidget))
	UIconTextLoadingButton* BillingButton;
	
	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|User Settings", meta = (BindWidget))
	UIconTextLoadingButton* DataPrivacyButton;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|User Settings", meta = (BindWidget))
	UIconTextLoadingButton* LegalStuffButton;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|User Settings", meta = (BindWidget))
	UIconTextLoadingButton* ExitMultiplayerButton;
};
