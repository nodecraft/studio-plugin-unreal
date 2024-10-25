// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "UserSettingsLegal.generated.h"

class UTermsOfServiceModal;
class UNodecraftButtonBase;
/**
 * 
 */
UCLASS()
class NODECRAFTDISCOVERY_API UUserSettingsLegal : public UCommonActivatableWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	virtual UWidget* NativeGetDesiredFocusTarget() const override;
	
	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Legal Settings", meta = (BindWidget))
	UNodecraftButtonBase* TermsOfService;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Legal Settings", meta = (BindWidget))
	UNodecraftButtonBase* PrivacyPolicy;

	UPROPERTY(EditDefaultsOnly, Category = "Nodecraft UI|Legal Settings")
	TSoftClassPtr<UTermsOfServiceModal> TermsOfServiceModal;
};
