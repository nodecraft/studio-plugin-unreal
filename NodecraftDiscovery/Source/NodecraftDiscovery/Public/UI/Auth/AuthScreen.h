// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidgetSwitcher.h"
#include "CommonBorder.h"
#include "CommonLazyImage.h"
#include "CommonUserWidget.h"
#include "NodecraftAuthProvider.h"
#include "Input/CommonBoundActionBar.h"
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
	UFUNCTION()
	void OnAuthComplete_Internal();
	
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

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Auth", meta=(BindWidget))
	UWidget* ActionBarContainer;
	
	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Auth", meta=(BindWidget))
	UCommonBoundActionBar* ActionBar;

	UPROPERTY(meta=(BindWidget))
	UImage* LoadingSpinner;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAuthComplete);
	UPROPERTY(BlueprintAssignable, Category = "Nodecraft UI|Auth")
	FOnAuthComplete OnAuthComplete;

	FSimpleDelegate OnCallFailed;

private:
	void UpdateActionBindings(ECommonInputType CurrentInputType);

};
