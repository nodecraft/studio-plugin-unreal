// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Auth_PromptBase.h"
#include "Components/EditableText.h"
#include "Auth_EmailPrompt.generated.h"

/**
 * 
 */
UCLASS()
class NODECRAFTDISCOVERY_API UAuth_EmailPrompt : public UAuth_PromptBase
{
	GENERATED_BODY()

public:
	FSimpleDelegate OnManualChallengeCreated;

	FString GetEmail() const;

protected:
	virtual UWidget* NativeGetDesiredFocusTarget() const override;
	virtual void NativeOnInitialized() override;
	virtual void NativeOnActivated() override;
	virtual void NativeOnDeactivated() override;
	
	UPROPERTY(meta=(BindWidget))
	UEditableText* EmailEditText;

	virtual void SubmitRequest() override;

private:
	UFUNCTION()
	void OnTextCommitted(const FText& InText, ETextCommit::Type CommitMethod);
	void StartManualChallenge();
};
