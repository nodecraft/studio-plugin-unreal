// Nodecraft, Inc. © 2012-2024, All Rights Reserved.


#include "UI/Auth/Auth_TermsOfServicePrompt.h"

#include "IdentityCommon.h"
#include "Services/ConsentsService.h"
#include "Services/IdentService.h"
#include "UI/Auth/AuthScreen.h"

void UAuth_TermsOfServicePrompt::NativeConstruct()
{
	Super::NativeConstruct();

	CancelButton->OnPressed().AddUObject(this, &UAuth_TermsOfServicePrompt::Cancel);
	BackButton->OnPressed().AddUObject(this, &UAuth_TermsOfServicePrompt::Cancel);
}

void UAuth_TermsOfServicePrompt::NativeOnActivated()
{
	Super::NativeOnActivated();
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
			DisplayError(Data.Error);
			break;
		default:
			unimplemented();
		}	
	});

	UIdentService::Get().ResendPreviousTokenWithConsents(OnComplete, Consents);
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
			Consents = InGameConsents;
			ConsentDetails->SetText(FText::FromString(InGameConsents.Content));
		}
		else
		{
			DisplayError(ErrorText.Get(FText()));
		}
	});
	LoadGuard->SetIsLoading(true);
	UConsentsService::Get().GetGameLegalConsents(IdentType, GetConsentsDelegate);
}
