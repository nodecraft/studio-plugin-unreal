// Nodecraft, Inc. © 2012-2024, All Rights Reserved.


#include "UI/Auth/Auth_EmailPrompt.h"

#include "API/NodecraftStudioApi.h"
#include "Subsystems/MessageRouterSubsystem.h"

FString UAuth_EmailPrompt::GetEmail() const
{
	return EmailEditText->GetText().ToString();
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
