// Nodecraft, Inc. © 2012-2024, All Rights Reserved.


#include "UI/ServerDetails/TabbedSections/ServerDetailsRulesSection.h"

#include "CommonBorder.h"
#include "CommonInputSubsystem.h"
#include "CommonTextBlock.h"
#include "Input/CommonUIInputTypes.h"
#include "Services/ConsentsService.h"
#include "UI/Common/NodecraftLoadGuard.h"
#include "Utility/NodecraftMacros.h"

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
	RefreshSignConsentsAction();
	LoadGuard->SetIsLoading(false);
}

void UServerDetailsRulesSection::RefreshSignConsentsAction()
{
	RulesScrollBox->OnScrolledToBottom.RemoveAll(this);
	AcceptConsentsActionHandle.Unregister();

	// When we activate Rules Section, check to see if we need to sign consents.
	// If we do, register a delegate such that once we scroll down to the bottom of the rules (or if we can't scroll),
	// we'll show the consent button.
	// Otherwise, unregister the action if it's registesred.
	
	ECommonInputType CurrentInputType = ECommonInputType::MouseAndKeyboard;
	if (ULocalPlayer* LocalPlayer = GetOwningLocalPlayer())
	{
		CurrentInputType = UCommonInputSubsystem::Get(LocalPlayer)->GetCurrentInputType();
	}
	
	bool bNeedsToSignConsents = false;
	if (CurrentInputType == ECommonInputType::Gamepad)
	{
		if (ServerDataObject)
		{
			if (const auto Rules = ServerDataObject->GetRules())
			{
				if (Rules->GetConsentStatus() == EConsentStatus::Outdated || Rules->GetConsentStatus() == EConsentStatus::NotSigned)
				{
					bNeedsToSignConsents = true;

					auto SetupActionHandle = [this]()
					{
						AcceptConsentsActionHandle = RegisterUIActionBinding(
												FBindUIActionArgs(AcceptConsentsActionData, true,
													FSimpleDelegate::CreateWeakLambda(this, [this]
												{
													ConsentLabelButton->OnClicked().Broadcast();
												})));

						RulesScrollBox->OnScrolledToBottom.RemoveAll(this);
					};
					
					// If we can already see everything, we should just set up the action.
					// Otherwise, we'll wait until we scroll to the bottom.
					if (RulesScrollBox->GetScrollOffsetOfEnd() <= 0.0f)
					{
						SetupActionHandle();
					}
					else
					{
						RulesScrollBox->OnScrolledToBottom.AddWeakLambda(this, [this, SetupActionHandle]
						{
							SetupActionHandle();
						});
					}
				}
			}
		}
	}
	
	if (bNeedsToSignConsents == false || CurrentInputType != ECommonInputType::Gamepad)
	{
		AcceptConsentsActionHandle.Unregister();
	}
}

void UServerDetailsRulesSection::UpdateActionBindings(ECommonInputType CurrentInputType)
{
	RefreshSignConsentsAction();
}


void UServerDetailsRulesSection::NativeOnActivated()
{
	Super::NativeOnActivated();

	RulesScrollBox->SetListeningForInput(true);
	RefreshSignConsentsAction();

	ON_INPUT_METHOD_CHANGED(UpdateActionBindings);
}

void UServerDetailsRulesSection::NativeOnDeactivated()
{
	RulesScrollBox->SetListeningForInput(false);

	if (AcceptConsentsActionHandle.IsValid())
	{
		AcceptConsentsActionHandle.Unregister();
	}
	Super::NativeOnDeactivated();
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
			RefreshSignConsentsAction();
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
