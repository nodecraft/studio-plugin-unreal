// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "UI/Common/NodecraftScrollBox.h"
#include "TermsOfServiceModal.generated.h"

struct FPlayerConsents;
class URulesDataObject;
class UCommonTextBlock;
class UNodecraftButtonBase;
/**
 * 
 */
UCLASS()
class NODECRAFTDISCOVERY_API UTermsOfServiceModal : public UCommonActivatableWidget
{
	GENERATED_BODY()

public:
	void Configure(const FPlayerConsents& Consents, FSimpleDelegate ClosePopupDelegate);

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Terms of Service", meta = (BindWidget))
	UNodecraftButtonBase* CloseButton;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Terms of Service", meta = (BindWidget))
	UCommonTextBlock* AgreedToTermsDateText;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Terms of Service", meta = (BindWidget))
	UCommonTextBlock* SystemLegalText;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Terms of Service", meta = (BindWidget))
	UCommonTextBlock* GameLegalText;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Terms of Service", meta = (BindWidget))
	UCommonTextBlock* EmailAgreementText;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Terms of Service", meta = (BindWidget))
	UNodecraftButtonBase* CancelButton;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Terms of Service", meta = (BindWidget))
	UNodecraftScrollBox* ScrollBox;
};
