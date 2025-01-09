// Nodecraft, Inc. © 2012-2024, All Rights Reserved.


#include "UI/Auth/Auth_PromptBase.h"

#include "CommonButtonBase.h"
#include "CommonInputSubsystem.h"
#include "API/NodecraftMessageCodes.h"
#include "Input/CommonUIInputTypes.h"
#include "Subsystems/MessageRouterSubsystem.h"
#include "UI/Alerts/AlertMessage.h"
#include "UI/Foundation/NodecraftButtonBase.h"

void UAuth_PromptBase::DisplayError(const FText& InErrorText)
{
	if (InErrorText.IsEmpty())
	{
		Alert->Hide();
	}
	else
	{
		Alert->Show(InErrorText, EAlertType::Error);
	}
}

void UAuth_PromptBase::NativeConstruct()
{
	Super::NativeConstruct();
	
	Alert->Hide();
	SubmitButton->OnPressed().AddUObject(this, &UAuth_PromptBase::SubmitButtonPressed);
	
	const TArray<FString> MessageCodes = {
		SERVER_VALIDATION_FAILURE,
		IDENT_MISSING_TOKEN_FAILURE,
		IDENT_INVALID_CODE_FAILURE,
		IDENT_INVALID_TYPE_FAILURE,
		IDENT_INCOMPATIBLE_TYPE_FAILURE,
		IDENT_MISSING_IP_FAILURE,
		IDENT_PLATFORM_FAILURE,
		IDENT_PLATFORM_CONFIG_FAILURE,
		IDENT_MISSING_LEGAL_CONSENT_FAILURE,
		IDENT_MISSING_RULES_CONSENT_FAILURE,
		IDENT_CHALLENGE_REQUESTED_SUCCESS,
		IDENT_SUCCESS,
		IDENT_SESSION_SUCCESS,
		IDENT_SESSION_UPDATE_SUCCESS,
		IDENT_GAME_IDENTS_SUCCESS
	};
	
	FOnRoutedMessageReceived OnMessageReceived;
	OnMessageReceived.BindWeakLambda(this, [this](const FString& MessageCode, const FText& Message, EAlertType AlertType)
	{
		if (AlertType == EAlertType::Error)
		{
			Alert->Show(Message, AlertType);
		}
		else if (AlertType == EAlertType::Success)
		{
			Alert->Hide();
		}
	});
	UMessageRouterSubsystem::Get().AddMessageReceiver("AuthPrompt", MessageCodes, OnMessageReceived);
}

void UAuth_PromptBase::NativeDestruct()
{
	Super::NativeDestruct();
	UMessageRouterSubsystem::Get().RemoveMessageReceiver("AuthPrompt");
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

void UAuth_PromptBase::RefreshActions(ECommonInputType InputType)
{
	if (InputType == ECommonInputType::Gamepad && SubmitButtonActionData.IsNull() == false && SubmitButtonActionHandle.IsValid() == false)
	{
		FBindUIActionArgs Args = FBindUIActionArgs(SubmitButtonActionData, bDisplayInActionBar,
					FSimpleDelegate::CreateWeakLambda(this, [this]
					{
						SubmitRequest();
					}));
		if (SubmitButton->GetInputActionNameOverride().IsEmptyOrWhitespace() == false)
		{
			Args.OverrideDisplayName = SubmitButton->GetInputActionNameOverride();
		}
		SubmitButtonActionHandle = RegisterUIActionBinding(Args);
	}
	else if (SubmitButtonActionHandle.IsValid())
	{
		SubmitButtonActionHandle.Unregister();
	}
}
