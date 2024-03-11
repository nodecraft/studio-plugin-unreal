// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ServerDetails/TabbedSections/ServerDetailsRulesSection.h"

#include "CommonLoadGuard.h"
#include "CommonTextBlock.h"
#include "Services/ConsentsService.h"

#define LOCTEXT_NAMESPACE "ServerDetailsRulesSection"

#define LOCTEXT_YOU_AGREED_TO_THESE_RULES LOCTEXT("ConsentGrantedOnDate", "You agreed to these rules on: {0}")
#define SIGNED_DATE_FORMATTED_STRING TEXT("%B %d, %Y")

DEFINE_LOG_CATEGORY_STATIC(LogServerDetailsRulesSection, All, All);

void UServerDetailsRulesSection::RefreshConsentLabelButton(const URulesDataObject* Rules)
{
	if (Rules)
	{
		RulesText->SetText(Rules->GetContent());
	
		if (Rules->GetConsentStatus() == EConsentStatus::Signed)
		{
			ConsentLabelButton->SetButtonText(FText::Format(LOCTEXT_YOU_AGREED_TO_THESE_RULES, FText::FromString(Rules->GetDateSigned().ToFormattedString(SIGNED_DATE_FORMATTED_STRING))));
			ConsentLabelButton->SetIsEnabled(false);
			ConsentExplanation->SetVisibility(ESlateVisibility::Collapsed);
		}
		else
		{
			ConsentLabelButton->SetButtonText(LOCTEXT("IAgreeToTheseRules", "I Agree to these Rules"));
			ConsentLabelButton->SetIsEnabled(true);
			ConsentExplanation->SetVisibility(ESlateVisibility::Visible);
		}
	}
	else
	{
		UE_LOG(LogServerDetailsRulesSection, Error, TEXT("UServerDetailsRulesSection::SetServerData(): ServerDataObject->GetRules() is null!"));
	}
}

void UServerDetailsRulesSection::SetServerData(UServerDataObject* InServerDataObject)
{
	ServerDataObject = InServerDataObject;
	if (InServerDataObject == nullptr)
	{
		LoadGuard->SetIsLoading(false);
		UE_LOG(LogServerDetailsRulesSection, Error, TEXT("UServerDetailsRulesSection::SetServerData(): ServerDataObject is null !"));
		return;
	}
	
	RefreshConsentLabelButton(InServerDataObject->GetRules());

	LoadGuard->SetIsLoading(false);

}

void UServerDetailsRulesSection::SetLoading(const bool bLoading)
{
	LoadGuard->SetIsLoading(bLoading);
}

void UServerDetailsRulesSection::OnConsentButtonClicked()
{
	ConsentLabelButton->SetIsLoading(true);
	FSignConsentsResponseDelegate OnComplete = FSignConsentsResponseDelegate::CreateWeakLambda(this, [this](URulesDataObject* Rules, const bool bSuccess, const TOptional<FText>& Error)
	{
		ConsentLabelButton->SetIsLoading(false);
		if (bSuccess)
		{
			ServerDataObject->UpdateRules(Rules);
			RefreshConsentLabelButton(ServerDataObject->GetRules());
		}
		else
		{
			UE_LOG(LogServerDetailsRulesSection, Error, TEXT("UServerDetailsRulesSection::OnConsentButtonClicked(): Failed to sign consents!"));
		}
	});
	
	UConsentsService::Get().SignGameRulesConsents(ServerDataObject->GetRules()->GetConsents(), OnComplete);
}

void UServerDetailsRulesSection::NativeConstruct()
{
	Super::NativeConstruct();
	ConsentLabelButton->OnClicked().AddUObject(this, &UServerDetailsRulesSection::OnConsentButtonClicked);
}

#undef LOCTEXT_NAMESPACE
