// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Settings/TermsOfServiceModal.h"

#include "CommonTextBlock.h"
#include "Models/Consents.h"
#include "UI/Foundation/NodecraftButtonBase.h"

#define LOCTEXT_NAMESPACE "TermsOfServiceModal"

void UTermsOfServiceModal::Configure(const FPlayerConsents& Consents, FSimpleDelegate ClosePopupDelegate)
{
	TArray<FString> OutStrings;
	Consents.Content.ParseIntoArray(OutStrings, TEXT("\n\n"));
	
	SystemLegalText->SetText(FText::FromString(OutStrings.IsValidIndex(0) ? OutStrings[0] : ""));
	GameLegalText->SetText(FText::FromString(OutStrings.IsValidIndex(1) ? OutStrings[1] : ""));
	EmailAgreementText->SetText(FText::FromString(OutStrings.IsValidIndex(2) ? OutStrings[2] : ""));
	
	AgreedToTermsDateText->SetText(Consents.DateAllSigned.GetTicks() == 0 ?
		LOCTEXT("ConsentsNotSigned", "You have not signed consents.") :
		FText::Format(LOCTEXT("AgreedToConsentsOnDate", "You last agreed to this document on: {0}"), FText::AsDate(Consents.DateAllSigned)));

	CloseButton->OnClicked().AddWeakLambda(this, [this, ClosePopupDelegate]
	{
		ClosePopupDelegate.ExecuteIfBound();
	});
	CancelButton->OnClicked().AddWeakLambda(this, [this, ClosePopupDelegate]
	{
		ClosePopupDelegate.ExecuteIfBound();
	});
	SubmitButton->OnClicked().AddWeakLambda(this, [this, ClosePopupDelegate]
	{
		// TODO: implement submitting consents once consents refactor is done
	});
}

#undef LOCTEXT_NAMESPACE