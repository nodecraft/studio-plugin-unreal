// Nodecraft, Inc. © 2012-2024, All Rights Reserved.


#include "UI/Auth/Auth_EmailPrompt.h"

#include "CommonInputSubsystem.h"
#include "API/NodecraftStudioApi.h"
#include "Subsystems/MessageRouterSubsystem.h"
#include "Utility/NodecraftMacros.h"

FString UAuth_EmailPrompt::GetEmail() const
{
	return EmailEditText->GetText().ToString();
}

UWidget* UAuth_EmailPrompt::NativeGetDesiredFocusTarget() const
{
	return EmailEditText ? EmailEditText : Super::NativeGetDesiredFocusTarget();
}

void UAuth_EmailPrompt::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	EmailEditText->OnTextCommitted.AddUniqueDynamic(this, &UAuth_EmailPrompt::OnTextCommitted);
}

void UAuth_EmailPrompt::NativeOnActivated()
{
	Super::NativeOnActivated();
	NativeGetDesiredFocusTarget()->SetFocus();
	ON_INPUT_METHOD_CHANGED(RefreshActions);
}

void UAuth_EmailPrompt::NativeOnDeactivated()
{
	if (const ULocalPlayer* LocalPlayer = GetOwningLocalPlayer())
	{
		UCommonInputSubsystem::Get(LocalPlayer)->OnInputMethodChangedNative.RemoveAll(this);
	}
	Super::NativeOnDeactivated();
}

void UAuth_EmailPrompt::OnTextCommitted(const FText& InText, ETextCommit::Type CommitMethod)
{
	if (CommitMethod == ETextCommit::OnEnter)
	{
		SubmitRequest();
	}
}

void UAuth_EmailPrompt::SubmitRequest()
{
	Super::SubmitRequest();
	StartManualChallenge();
}

void UAuth_EmailPrompt::StartManualChallenge()
{
	UNodecraftStudioApi::Ident_ManualChallenge(this, EmailEditText->GetText().ToString(),
	 [this](FHttpRequestPtr Req, FHttpResponsePtr Res, bool bConnectedSuccessfully) mutable
	 {
		 if (bConnectedSuccessfully)
		 {
			 if (EHttpResponseCodes::IsOk(Res.Get()->GetResponseCode()))
			 {
			 	OnManualChallengeCreated.ExecuteIfBound();
			 }
		 	UMessageRouterSubsystem::Get().RouteHTTPResult(Res, __FUNCTION__);
		 }
		 else
		 {
		 	UMessageRouterSubsystem::Get().RouteFailureToConnect(__FUNCTION__);
		 }
	 	OnCallEnded.ExecuteIfBound();
	 })->ProcessRequest();
}
