// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Auth_PromptBase.h"
#include "Components/EditableText.h"
#include "Auth_TokenPrompt.generated.h"

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
