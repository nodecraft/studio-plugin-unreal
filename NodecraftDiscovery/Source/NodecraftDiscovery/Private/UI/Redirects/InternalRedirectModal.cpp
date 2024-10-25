// Nodecraft, Inc. © 2012-2024, All Rights Reserved.


#include "UI/Redirects/InternalRedirectModal.h"

#include "CommonInputSubsystem.h"
#include "NodecraftLogCategories.h"
#include "API/NodecraftMessageCodes.h"
#include "DataTypes/ImageBackgroundTypes.h"
#include "DataTypes/LinkTypes.h"
#include "DataTypes/PlayerConnectionStatus.h"
#include "DataTypes/PlayerConnectionSubjects.h"
#include "Input/CommonUIInputTypes.h"
#include "Services/LinksService.h"
#include "Services/ServiceDelegates.h"
#include "Subsystems/MessageRouterSubsystem.h"
#include "UI/Common/AsyncImage.h"
#include "UI/Foundation/NodecraftButtonBase.h"
#include "Utility/NodecraftMacros.h"
#include "Utility/NodecraftUtility.h"

#define LOCTEXT_NAMESPACE "NodecraftDiscovery"
#define ROUTE_ID "InternalRedirectModal"

void UInternalRedirectModal::Configure(FSimpleDelegate OnClosed, EPlayerConnectionSubject Subject,
                                       TOptional<FString> ServerId)
{
	bHasLoadedQrCode = false;
	LoadGuard->SetIsLoading(true);

	FSimpleServiceResponseDelegate OnQrCodeLoaded;
	OnQrCodeLoaded.BindWeakLambda(this, [this](bool bSuccess, TOptional<FText> Error)
	{
		if (bSuccess)
		{
			bHasLoadedQrCode = true;
		}
		else
		{
			ShowErrorUI(Error.GetValue());
		}
	});
	
	FOnPlayerConnection OnPlayerConnectionCallComplete;
	OnPlayerConnectionCallComplete.BindWeakLambda(this, [this, OnQrCodeLoaded](UPlayerConnectionDataObject* PlayerConnectionDataObject, bool bSuccess, TOptional<FText> Error)
	{
		if (bSuccess)
		{
			RefreshUI(PlayerConnectionDataObject->GetConnectionStatus());
			QRCode->LoadImageAsyncSkipCache(
				UNodecraftUtility::GetQrCodeUrl(PlayerConnectionDataObject->GetUrl(), ELinkType::Internal),
				OnQrCodeLoaded, ETransparentPixelOverrides::FillWhite);

			ULinksService::Get().StartPollingForPlayerConnectionStatus(GetWorld());
	
			OpenInBrowserButton->OnClicked().Clear();
			OpenInBrowserButton->OnClicked().AddWeakLambda(this, [this, PlayerConnectionDataObject]
			{
				if (PlayerConnectionDataObject)
				{
					if (PlayerConnectionDataObject->GetUrl().IsEmpty() == false)
					{
						UE_LOG(LogTemp, Log, TEXT("Trying to open URI in browser. Supposedly it's not empty! It is : %s"), *PlayerConnectionDataObject->GetUrl());
						ULinksService::Get().OpenURLInBrowser(ELinkType::Internal, PlayerConnectionDataObject->GetUrl());
					}
					else
					{
						UE_LOG(LogNodecraft_QrCodeLinks, Error, TEXT("Cannot open URI in browser. URL is empty!"));
					}
				}
				else
				{
					UE_LOG(LogNodecraft_QrCodeLinks, Error, TEXT("Cannot open URI in browser. PlayerConnectionDataObject is null!"));
				}
			});

			SubmitButton->OnClicked().Clear();
			SubmitButton->OnClicked().AddWeakLambda(this, [this]
			{
				ULinksService::Get().ConsumeChallengeResponse(CodeChallengeInput->GetText().ToString());
			});
		}
		else
		{
			ShowErrorUI(Error.GetValue());
		}
		LoadGuard->SetIsLoading(false);
	});

	if (Subject == EPlayerConnectionSubject::ServerSettings)
	{
		if (ServerId.IsSet())
		{
			ULinksService::Get().CreatePlayerConnectionForServer(Subject, ServerId.GetValue(), OnPlayerConnectionCallComplete);
		}
		else
		{
			// Hacky way to show custom error
			ShowErrorUI(LOCTEXT("ServerIdNotSet", "Tried to open server settings with an empty server ID. Please try again."));
		}
	}
	else
	{
		ULinksService::Get().CreatePlayerConnection(Subject, OnPlayerConnectionCallComplete);
	}
	
	CloseButtonBottom->OnClicked().Clear();
	CloseButtonBottom->OnClicked().AddWeakLambda(this, [OnClosed]
	{
		OnClosed.ExecuteIfBound();
	});
}

void UInternalRedirectModal::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	ON_INPUT_METHOD_CHANGED(UpdateActionBindings)
}

void UInternalRedirectModal::NativeOnActivated()
{
	Super::NativeOnActivated();
	// Listen for messages from router
	UMessageRouterSubsystem::Get().AddMessageReceiver(ROUTE_ID,
		{PLAYER_CONNECTION_GET_SUCCESS, PLAYER_CONNECTION_GET_NOT_FOUND_FAILURE, PLAYER_CONNECTION_CHALLENGE_CONSUME_FAILURE, PLAYER_CONNECTION_CHALLENGE_CONSUME_SUCCESS},
		FOnRoutedMessageReceived::CreateWeakLambda(this, [this](const FString& MessageIdentifer, const FText& MessageBody, EAlertType AlertType)
	{
		if (MessageIdentifer == PLAYER_CONNECTION_GET_SUCCESS)
		{
			EPlayerConnectionStatus Status = EPlayerConnectionStatusHelpers::FromString(MessageBody.ToString());
			switch (Status)
			{
			case EPlayerConnectionStatus::Undefined:
				ShowErrorUI(LOCTEXT("PlayerConnectionStatusUndefined", "The connection status was unknown or undefined. Please try again."));
				break;
			default:
				RefreshUI(Status);
				break;	
			}
		}
		else if (MessageIdentifer == PLAYER_CONNECTION_GET_NOT_FOUND_FAILURE)
		{
			AlertMessage->Show(MessageBody, EAlertType::Error);
		}
		else if (MessageIdentifer == PLAYER_CONNECTION_CHALLENGE_CONSUME_FAILURE)
		{
			// Show error message saying that the code was invalid
			AlertMessage->Show(
				LOCTEXT("ChallengeCodeInvalid", "The code you entered was invalid. Please try again."),
				EAlertType::Error);
		}
	}));
}

void UInternalRedirectModal::NativeOnDeactivated()
{
	UMessageRouterSubsystem::Get().RemoveMessageReceiver(ROUTE_ID);
	ULinksService::Get().StopPollingForPlayerConnectionStatus(GetWorld());
	Super::NativeOnDeactivated();
}

bool UInternalRedirectModal::NativeOnHandleBackAction()
{
	CloseButtonBottom->OnClicked().Broadcast();
	return Super::NativeOnHandleBackAction();
}

void UInternalRedirectModal::RefreshUI(EPlayerConnectionStatus Status)
{
	PendingContainer->SetVisibility(ESlateVisibility::Collapsed);
	CodeChallengeContainer->SetVisibility(ESlateVisibility::Collapsed);
	AlertMessage->Hide();

	bool bShowOpenInBrowserButton = false;
	
	switch (Status)
	{
	case EPlayerConnectionStatus::Undefined:
		break;
	case EPlayerConnectionStatus::Pending:
		PendingContainer->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		bShowOpenInBrowserButton = true;
		break;
	case EPlayerConnectionStatus::CodeChallenge:
		CodeChallengeContainer->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		bShowOpenInBrowserButton = true;
		break;
	case EPlayerConnectionStatus::Completed:
		AlertMessage->Show(LOCTEXT("PlayerConnectionSuccess", "You've completed the browser connection. You can close this window when you're done."), EAlertType::Success);
		break;
	case EPlayerConnectionStatus::Expired:
		AlertMessage->Show(LOCTEXT("PlayerConnectionExpired", "This link has expired to protect your account security. Close this window and try again."), EAlertType::Error);
		break;
	}
	
	OpenInBrowserButton->SetVisibility(bShowOpenInBrowserButton ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
}

void UInternalRedirectModal::ShowErrorUI(const FText& ErrorMsg)
{
	PendingContainer->SetVisibility(ESlateVisibility::Collapsed);
	CodeChallengeContainer->SetVisibility(ESlateVisibility::Collapsed);
	OpenInBrowserButton->SetVisibility(ESlateVisibility::Collapsed);
	AlertMessage->Show(ErrorMsg, EAlertType::Error);
}

void UInternalRedirectModal::UpdateActionBindings(ECommonInputType CurrentInputType)
{
	if (CurrentInputType == ECommonInputType::Gamepad)
	{
		if (OpenInBrowserActionData.IsNull() == false)
		{
			FBindUIActionArgs Args = FBindUIActionArgs(OpenInBrowserActionData, bDisplayInActionBar,
				FSimpleDelegate::CreateWeakLambda(this, [this]
				{
					OpenInBrowserButton->OnClicked().Broadcast();
				}));
			if (OpenInBrowserButton->GetInputActionNameOverride().IsEmptyOrWhitespace() == false)
			{
				Args.OverrideDisplayName = OpenInBrowserButton->GetInputActionNameOverride();
			}
			OpenInBrowserActionHandle = RegisterUIActionBinding(Args);
		}

		if (CodeChallengeContainer->IsVisible() && SubmitActionData.IsNull() == false)
		{
			FBindUIActionArgs Args = FBindUIActionArgs(SubmitActionData, bDisplayInActionBar,
				FSimpleDelegate::CreateWeakLambda(this, [this]
				{
					SubmitButton->OnClicked().Broadcast();
				}));
			if (SubmitButton->GetInputActionNameOverride().IsEmptyOrWhitespace() == false)
			{
				Args.OverrideDisplayName = SubmitButton->GetInputActionNameOverride();
			}
			SubmitActionHandle = RegisterUIActionBinding(Args);
		}
	}
	else
	{
		if (OpenInBrowserActionHandle.IsValid())
		{
			OpenInBrowserActionHandle.Unregister();
		}
		if (SubmitActionHandle.IsValid())
		{
			SubmitActionHandle.Unregister();
		}
	}
}

#undef LOCTEXT_NAMESPACE
