// Nodecraft, Inc. © 2012-2024, All Rights Reserved.


#include "UI/Auth/Auth_TermsOfServicePrompt.h"

#include "CommonInputSubsystem.h"
#include "IdentityCommon.h"
#include "Components/ScrollBox.h"
#include "Input/CommonUIInputTypes.h"
#include "Services/ConsentsService.h"
#include "Services/IdentService.h"
#include "UI/Auth/AuthScreen.h"
#include "UI/Foundation/NodecraftButtonBase.h"
#include "Utility/NodecraftMacros.h"
#include "Utility/NodecraftUtility.h"
#include "TimerManager.h"

void UAuth_TermsOfServicePrompt::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	CancelButton->OnPressed().AddUObject(this, &UAuth_TermsOfServicePrompt::Cancel);
	BackButton->OnPressed().AddUObject(this, &UAuth_TermsOfServicePrompt::Cancel);
}

void UAuth_TermsOfServicePrompt::NativeOnActivated()
{
	Super::NativeOnActivated();

	ScrollBox->ScrollToStart();
	ScrollBox->SetListeningForInput(false, FText::FromString("Scroll Text"));

	// Disable the submit button until we've loaded the consents and the player has scrolled to the bottom (read it all).
	SubmitButton->SetIsEnabled(false);
	ON_INPUT_METHOD_CHANGED(RefreshActions)
}

void UAuth_TermsOfServicePrompt::NativeOnDeactivated()
{
	if (const ULocalPlayer* LocalPlayer = GetOwningLocalPlayer())
	{
		UCommonInputSubsystem::Get(LocalPlayer)->OnInputMethodChangedNative.RemoveAll(this);
	}
	Super::NativeOnDeactivated();
}

// Override standard back handle. We don't want to deactivate. We want to behave as if the player clicked the Back button.
bool UAuth_TermsOfServicePrompt::NativeOnHandleBackAction()
{
	Cancel();
	return true;
}

void UAuth_TermsOfServicePrompt::SubmitRequest()
{
	Super::SubmitRequest();
	
	FSendIdentTokenResponseDelegate OnComplete;
	OnComplete.BindWeakLambda(this, [this](const FSendIdentTokenResponseDelegateData Data)
	{
		OnCallEnded.ExecuteIfBound();
		
		switch (Data.Status)
		{
		case ESendIdentTokenResponseStatus::Success:
			OnLoggedInSuccess.ExecuteIfBound();
			break;
		case ESendIdentTokenResponseStatus::RequiresConsents:
			OnConsentsRequireSignature.ExecuteIfBound();
			break;
		case ESendIdentTokenResponseStatus::Error:
			RefreshActions(UNodecraftUtility::GetCurrentInputType(GetWorld()));
			DisplayError(Data.Error);
			break;
		default:
			unimplemented();
		}
	});

	// While loading is occurring, remove all actions
	ScrollBox->SetListeningForInput(false, FText::FromString("Scroll Text"));
	SubmitButtonActionHandle.Unregister();

	UIdentService::Get().ResendPreviousTokenWithConsents(OnComplete, Consents);
}

void UAuth_TermsOfServicePrompt::RefreshActions(ECommonInputType InputType)
{
	// NOTE: Call to Super is intentionally omitted here. We're handling all the logic necessary without calling Super.
	
	ScrollBox->OnScrolledToBottom.RemoveAll(this);
	SubmitButtonActionHandle.Unregister();

	// If we haven't yet loaded consents, we don't want to display any actions yet. 
	if (Consents.ConsentPages.Num() == 0)
	{
		return;
	}
	
	// When we activate Rules Section, check to see if we need to sign consents.
	// If we do, register a delegate such that once we scroll down to the bottom of the rules (or if we can't scroll),
	// we'll show the consent button.
	// Otherwise, unregister the action if it's registered.
	
	if (Consents.ConsentPages.Num() > 0) {
		auto SetupActionHandle = [this, InputType]()
		{
			if (InputType == ECommonInputType::Gamepad)
			{
				FBindUIActionArgs Args = FBindUIActionArgs(SubmitButtonActionData, bDisplayInActionBar,
					FSimpleDelegate::CreateWeakLambda(this, [this]
					{
						SubmitRequest();
					}));
				if (SubmitButton->GetInputActionNameOverride().IsEmptyOrWhitespace() == false)
				{
					Args.OverrideDisplayName = SubmitButton->GetInputActionNameOverride();
				}
				SubmitButtonActionHandle = RegisterUIActionBinding(Args);
			}
			else
			{
				SubmitButton->SetIsEnabled(true);
			}
			ScrollBox->OnScrolledToBottom.RemoveAll(this);
		};
		
		// If we can already see everything, we should just set up the action.
		// Otherwise, we'll wait until we scroll to the bottom.
		if (ScrollBox->GetScrollOffsetOfEnd() == 0.0f)
		{
			SetupActionHandle();
		}
		else
		{
			ScrollBox->OnScrolledToBottom.AddWeakLambda(this, [this, SetupActionHandle]
			{
				SetupActionHandle();
			});
		}
	}
	
	if (InputType != ECommonInputType::Gamepad && SubmitButtonActionHandle.IsValid())
	{
		SubmitButtonActionHandle.Unregister();
	}
}

void UAuth_TermsOfServicePrompt::Cancel()
{
	if (ParentScreen)
	{
		ParentScreen->Cancel();
	}
}

void UAuth_TermsOfServicePrompt::RetrieveConsentDetails(const FString& IdentType)
{
	FConsentsResponseDelegate GetConsentsDelegate;
	GetConsentsDelegate.BindWeakLambda(this, [this](FGameConsents InGameConsents, bool bSuccess, TOptional<FText> ErrorText)
	{
		LoadGuard->SetIsLoading(false);
		if (bSuccess)
		{
			ScrollBox->SetListeningForInput(true, FText::FromString("Scroll Text"));

			Consents = InGameConsents;
			ConsentDetails->SetText(FText::FromString(InGameConsents.Content));
			// wait one frame, then call Refresh Actions, otherwise we will think the scrollbox is empty when it's not
			FTimerManager& TimerManager = GetWorld()->GetTimerManager();
			TimerManager.SetTimerForNextTick(FTimerDelegate::CreateLambda([this]()
			{
				RefreshActions(UNodecraftUtility::GetCurrentInputType(GetWorld()));
			}));
		}
		else
		{
			DisplayError(ErrorText.Get(FText()));
		}
	});
	LoadGuard->SetIsLoading(true);
	UConsentsService::Get().GetGameLegalConsents(IdentType, GetConsentsDelegate);
}
