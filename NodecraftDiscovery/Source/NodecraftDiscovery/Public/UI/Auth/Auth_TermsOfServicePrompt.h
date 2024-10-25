// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Auth_PromptBase.h"
#include "CommonRichTextBlock.h"
#include "Models/Consents.h"
#include "UI/Common/NodecraftLoadGuard.h"
#include "UI/Common/NodecraftScrollBox.h"
#include "Auth_TermsOfServicePrompt.generated.h"

/**
 * 
 */
UCLASS()
class NODECRAFTDISCOVERY_API UAuth_TermsOfServicePrompt : public UAuth_PromptBase
{
	GENERATED_BODY()

public:
	FSimpleDelegate OnConsentDetailsRetrieved;
	FSimpleDelegate OnLoggedInSuccess;
	FSimpleDelegate OnConsentsRequireSignature;

	void RetrieveConsentDetails(const FString& IdentType);

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeOnActivated() override;
	virtual void NativeOnDeactivated() override;
	virtual bool NativeOnHandleBackAction() override;
	
	virtual void RefreshActions(ECommonInputType InputType) override;
	virtual void SubmitRequest() override;

	UFUNCTION()
	void Cancel();
	
	UPROPERTY(meta=(BindWidget))
	UNodecraftLoadGuard* LoadGuard;
	
	UPROPERTY(meta=(BindWidget))
	UCommonRichTextBlock* ConsentDetails;

	UPROPERTY(meta=(BindWidget))
	UCommonButtonBase* CancelButton;

	UPROPERTY(meta=(BindWidgetOptional))
	UCommonButtonBase* BackButton;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI", meta = (BindWidget))
	UNodecraftScrollBox* ScrollBox;
	
private:
	FGameConsents Consents;

	FString PlayerEmail;
	FString PlayerAuthCode;

	FUIActionBindingHandle AcceptConsentsActionHandle;
};
