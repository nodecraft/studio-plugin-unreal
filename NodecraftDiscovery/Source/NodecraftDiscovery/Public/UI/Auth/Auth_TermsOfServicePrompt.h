// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Auth_PromptBase.h"
#include "CommonRichTextBlock.h"
#include "Models/Consents.h"
#include "UI/Common/NodecraftLoadGuard.h"
#include "Auth_TermsOfServicePrompt.generated.h"

/**
 * 
 */
UCLASS()
class NODECRAFTDISCOVERY_API UAuth_TermsOfServicePrompt : public UAuth_PromptBase
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	virtual void NativeOnActivated() override;

	FSimpleDelegate OnConsentDetailsRetrieved;

	FSimpleDelegate OnLoggedInSuccess;

	FSimpleDelegate OnConsentsRequireSignature;

	void RetrieveConsentDetails(const FString& IdentType);

protected:
	UPROPERTY(meta=(BindWidget))
	UNodecraftLoadGuard* LoadGuard;
	
	UPROPERTY(meta=(BindWidget))
	UCommonRichTextBlock* ConsentDetails;

	UPROPERTY(meta=(BindWidget))
	UCommonButtonBase* CancelButton;

	UPROPERTY(meta=(BindWidgetOptional))
	UCommonButtonBase* BackButton;

	virtual void SubmitRequest() override;

	UFUNCTION()
	void Cancel();

private:

	FGameConsents Consents;

	FString PlayerEmail;
	FString PlayerAuthCode;
};
