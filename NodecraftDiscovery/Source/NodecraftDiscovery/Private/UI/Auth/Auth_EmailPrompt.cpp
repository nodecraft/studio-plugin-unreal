// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Auth/Auth_EmailPrompt.h"

#include "API/DiscoveryAPI.h"
#include "DeveloperSettings/DiscoveryAPISettings.h"

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
	UDiscoveryAPI::Ident_ManualChallenge(this, EmailEditText->GetText().ToString(),
	 [this](FHttpRequestPtr Req, FHttpResponsePtr Res,
		 bool bConnectedSuccessfully) mutable
	 {
		 if (bConnectedSuccessfully)
		 {
			 if (EHttpResponseCodes::IsOk(Res.Get()->GetResponseCode()))
			 {
			 	OnManualChallengeCreated.ExecuteIfBound();
			 }
			 else
			 {
			 	DisplayError(FText::FromString("TODO: Parse error from JSON"));
			 }
		 }
		 else
		 {
			DisplayError(FText::FromString("Failed to connect to server. Please try again."));
		 }
	 	OnCallEnded.ExecuteIfBound();
	 })->ProcessRequest();
}
