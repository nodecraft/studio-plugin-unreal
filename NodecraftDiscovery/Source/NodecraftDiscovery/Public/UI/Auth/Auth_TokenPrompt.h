// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Auth_PromptBase.h"
#include "Components/EditableText.h"
#include "Auth_TokenPrompt.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class NODECRAFTDISCOVERY_API UAuth_TokenPrompt : public UAuth_PromptBase
{
	GENERATED_BODY()

public:
	FSimpleDelegate OnLoggedInSuccess;
	FSimpleDelegate OnConsentsRequireSignature;
	
	void SetEmail(FString InEmail);

protected:
	UPROPERTY(meta=(BindWidget))
	UEditableText* TokenEditText;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* EmailTextBlock;

protected:
	virtual void SubmitRequest() override;

private:
	void SendToken();

};
