// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Auth/Auth_PromptBase.h"

#include "CommonButtonBase.h"
#include "Components/TextBlock.h"

void UAuth_PromptBase::DisplayError(const FText& InErrorText)
{
	ErrorTextBlock->SetText(InErrorText);
	ErrorTextBlock->SetVisibility(InErrorText.IsEmpty() ? ESlateVisibility::Collapsed : ESlateVisibility::Visible);
}

void UAuth_PromptBase::NativeConstruct()
{
	Super::NativeConstruct();

	SubmitButton->OnPressed().AddUObject(this, &UAuth_PromptBase::SubmitButtonPressed);
}

void UAuth_PromptBase::SetParentScreen(UAuthScreen* const InParentScreen)
{
	ParentScreen = InParentScreen;
}

void UAuth_PromptBase::SubmitButtonPressed()
{
	SubmitRequest();
}

void UAuth_PromptBase::SubmitRequest()
{
	OnCallBegan.ExecuteIfBound();
}
