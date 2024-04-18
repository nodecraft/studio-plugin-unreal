// Nodecraft, Inc. © 2012-2024, All Rights Reserved.


#include "UI/Auth/Auth_TokenPrompt.h"

#include "API/NodecraftStudioApi.h"
#include "Components/TextBlock.h"
#include "IdentityCommon.h"
#include "DeveloperSettings/NodecraftStudioApiSettings.h"
#include "Services/IdentService.h"

void UAuth_TokenPrompt::SetEmail(const FString InEmail)
{
	EmailTextBlock->SetText(FText::FromString(InEmail));
}

void UAuth_TokenPrompt::SubmitRequest()
{
	Super::SubmitRequest();
	SendToken();
}

void UAuth_TokenPrompt::SendToken()
{
	const FString Email = EmailTextBlock->GetText().ToString();
	const FString Token = TokenEditText->GetText().ToString();

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
	
	UIdentService::Get().SendEmailToken(Email, Token, OnComplete);
}
