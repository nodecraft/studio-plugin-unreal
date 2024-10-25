// Nodecraft, Inc. © 2012-2024, All Rights Reserved.


#include "UI/Auth/Auth_TokenPrompt.h"

#include "CommonInputSubsystem.h"
#include "Components/TextBlock.h"
#include "IdentityCommon.h"
#include "Services/IdentService.h"
#include "Utility/NodecraftMacros.h"

void UAuth_TokenPrompt::SetEmail(const FString InEmail)
{
	EmailTextBlock->SetText(FText::FromString(InEmail));
}

void UAuth_TokenPrompt::NativeOnActivated()
{
	Super::NativeOnActivated();
	NativeGetDesiredFocusTarget()->SetFocus();
	ON_INPUT_METHOD_CHANGED(RefreshActions);
}

void UAuth_TokenPrompt::NativeOnDeactivated()
{
	if (const ULocalPlayer* LocalPlayer = GetOwningLocalPlayer())
	{
		UCommonInputSubsystem::Get(LocalPlayer)->OnInputMethodChangedNative.RemoveAll(this);
	}
	Super::NativeOnDeactivated();
}

void UAuth_TokenPrompt::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	TokenEditText->OnTextCommitted.AddUniqueDynamic(this, &UAuth_TokenPrompt::OnTextCommitted);
}

UWidget* UAuth_TokenPrompt::NativeGetDesiredFocusTarget() const
{
	return TokenEditText ? TokenEditText : Super::NativeGetDesiredFocusTarget();
}

void UAuth_TokenPrompt::SubmitRequest()
{
	Super::SubmitRequest();
	SendToken();
}

void UAuth_TokenPrompt::OnTextCommitted(const FText& InText, ETextCommit::Type CommitMethod)
{
	if (CommitMethod == ETextCommit::OnEnter)
	{
		SubmitRequest();
	}
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
