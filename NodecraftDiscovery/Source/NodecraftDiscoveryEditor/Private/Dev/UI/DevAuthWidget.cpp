// Fill out your copyright notice in the Description page of Project Settings.


#include "Dev/UI/DevAuthWidget.h"

#include "API/DiscoveryAPI.h"
#include "JsonObjectWrapper.h"
#include "API/DiscoverySessionManagerSubsystem.h"
#include "Interfaces/IHttpResponse.h"

#define CALLBACK_DEF 	   [&](\
	   FHttpRequestPtr Req,\
	   FHttpResponsePtr Res,\
	   bool bConnectedSuccessfully) mutable\

#define CALLBACK_BODY_BASE if(bConnectedSuccessfully) \
		   { \
		   	ResponseTextBlock->SetText(FText::FromString(Res.Get()->GetContentAsString())); \
		   	if (EHttpResponseCodes::IsOk(Res.Get()->GetResponseCode()))\
		   	{\
		   		ResponseStatusTextBlock->SetText(FText::FromString("Success"));\
		   		ResponseStatusTextBlock->SetColorAndOpacity(FColor::Green);\
		   	}\
		   	else\
		   	{\
		   		ResponseStatusTextBlock->SetText(FText::FromString("Failure"));\
		   		ResponseStatusTextBlock->SetColorAndOpacity(FColor::Red);\
		   	}\
		   }\
			else\
			{\
				ResponseTextBlock->SetText(FText());\
				ResponseStatusTextBlock->SetText(FText::FromString("Failed to connect to server."));\
				ResponseStatusTextBlock->SetColorAndOpacity(FColor::Red);\
			}\

void UDevAuthWidget::HitEndpoint(const FString URI, const FString HttpMethod, const FString HeaderToken, const FJsonObjectWrapper& RequestBody)
{
	UDiscoveryAPI::MakeRequest(this, URI, HttpMethod, HeaderToken, RequestBody,
		CALLBACK_DEF
	   {
	   		CALLBACK_BODY_BASE
	   })->ProcessRequest();
}

void UDevAuthWidget::SendRequest()
{
	FJsonObjectWrapper JsonBody;
	FString JsonText = JsonBodyTextBox->GetText().ToString();
	JsonBody.JsonObjectFromString(JsonText);
	
	FString HeaderToken;
	if (HeaderTokenSelector->GetSelectedOption().Contains(
		StaticEnum<EAccessTokenType>()->GetNameStringByIndex(static_cast<uint8>(EAccessTokenType::GameAccessToken_Public))))
	{
		HeaderToken = UDiscoverySessionManager::Get().GetGameAccessToken();
	}
	else if (HeaderTokenSelector->GetSelectedOption().Contains(
		StaticEnum<EAccessTokenType>()->GetNameStringByIndex(static_cast<uint8>(EAccessTokenType::PlayerAccessToken))))
	{
		HeaderToken = UDiscoverySessionManager::Get().GetPlayerAccessToken();
	}

	HitEndpoint(RootTextBox->GetText().ToString() + EndpointTextBox->GetText().ToString(),
	            HttpMethodSelector->GetSelectedOption(),
	            HeaderToken,
	            JsonBody);
}

void UDevAuthWidget::StartManualChallenge()
{
	UDiscoveryAPI::Ident_ManualChallenge(this, EmailAddressTextBox->GetText().ToString(),
	CALLBACK_DEF
	   {
		  CALLBACK_BODY_BASE
	   })->ProcessRequest();
}

void UDevAuthWidget::SendToken()
{
	TOptional<FGameConsents> EmptyConsents = TOptional<FGameConsents>();
	// Convert this to use service instead
	UDiscoveryAPI::Ident_Token(this, EmailAddressTextBox->GetText().ToString(), TokenTextBox->GetText().ToString(),
	EmptyConsents,
	CALLBACK_DEF
	   {
		  CALLBACK_BODY_BASE
	   })->ProcessRequest();
}

void UDevAuthWidget::RefreshHeaderComboBox()
{
	HeaderTokenSelector->ClearOptions();
	
	UDiscoverySessionManager& SessionManager = UDiscoverySessionManager::Get();

	for (EAccessTokenType TokenType : TEnumRange<EAccessTokenType>())
	{
		FString Base = StaticEnum<EAccessTokenType>()->GetNameStringByIndex(static_cast<uint8>(TokenType));

		bool bIsTokenSet = false;
		switch (TokenType)
		{
		case EAccessTokenType::GameAccessToken_Public:
			bIsTokenSet = SessionManager.IsGameAccessTokenSet();
			break;
		case EAccessTokenType::PlayerAccessToken:
			bIsTokenSet = SessionManager.IsPlayerSessionValid();
			break;
		default:
			ensureMsgf(false, TEXT("You have created a new access token but haven't told DevAuthWidget how to handle it"));
		}
		HeaderTokenSelector->AddOption(bIsTokenSet ? Base : Base + " ( NOT SET)");
	}
}

void UDevAuthWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SendRequestButton->OnClicked.AddUniqueDynamic(this, &UDevAuthWidget::SendRequest);
	ManualChallengeButton->OnClicked.AddUniqueDynamic(this, &UDevAuthWidget::StartManualChallenge);
	SendTokenButton->OnClicked.AddUniqueDynamic(this, &UDevAuthWidget::SendToken);

	// Populate method selector
	for (EHttpMethod Method : TEnumRange<EHttpMethod>())
	{
		HttpMethodSelector->AddOption(StaticEnum<EHttpMethod>()->GetNameStringByIndex(static_cast<uint8>(Method)));
	}
	
	// Populate header selector
	RefreshHeaderComboBox();

	UDiscoverySessionManager& SessionManager = UDiscoverySessionManager::Get();
	SessionManager.OnPlayerAccessTokenUpdated.AddWeakLambda(this, [this](FString NewToken)
	{
		RefreshHeaderComboBox();
	});
}

#undef CALLBACK_BODY_BASE
#undef CALLBACK_DEF