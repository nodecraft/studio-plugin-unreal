// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/Public/Delegates/Delegate.h"
// #include "Auth_EmailPrompt.h"
// #include "Auth_TermsOfServicePrompt.h"
// #include "Auth_TokenPrompt.h"
#include "CommonActivatableWidgetSwitcher.h"
#include "CommonLazyImage.h"
#include "CommonUserWidget.h"
#include "NodecraftAuthProvider.h"
#include "UI/Common/AsyncImage.h"
#include "AuthScreen.generated.h"

class UAuth_EmailPrompt;
class UAuth_TokenPrompt;
class UAuth_TermsOfServicePrompt;
/**
 * 
 */
UCLASS(Blueprintable)
class NODECRAFTDISCOVERY_API UAuthScreen : public UCommonUserWidget
{
	GENERATED_BODY()

	void SetIsLoading(bool bIsLoading);
	virtual void NativeConstruct() override;
	
	void AttemptAutoAuth();

	UFUNCTION()
	void OnPromptCallFinished();

	UFUNCTION()
	void OnPromptCallBegan();

public:
    void Cancel();

protected:
	UPROPERTY(meta=(BindWidget))
	UCommonActivatableWidgetSwitcher* WidgetSwitcher;

	UPROPERTY(meta=(BindWidget))
	UAsyncImage* LazyGameBackgroundImage;

	UPROPERTY(meta=(BindWidget))
	UAuth_EmailPrompt* EmailPrompt;

	UPROPERTY(meta=(BindWidget))
	UAuth_TokenPrompt* TokenPrompt;

	UPROPERTY(meta=(BindWidget))
	UAuth_TermsOfServicePrompt* ConsentsPrompt;

	UPROPERTY(meta=(BindWidget))
	UImage* LoadingSpinner;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAuthComplete);
	UPROPERTY(BlueprintAssignable)
	FOnAuthComplete OnAuthComplete;

	FSimpleDelegate OnCallFailed;
};
