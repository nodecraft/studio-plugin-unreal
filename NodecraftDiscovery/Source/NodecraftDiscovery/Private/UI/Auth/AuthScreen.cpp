// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Auth/AuthScreen.h"

#include "API/DiscoveryAPI.h"
#include "Models/GameDataObject.h"
#include "Services/GameService.h"
#include "Subsystems/RemoteImageSubsystem.h"
#include "UI/Auth/Auth_EmailPrompt.h"
#include "UI/Auth/Auth_TermsOfServicePrompt.h"
#include "UI/Auth/Auth_TokenPrompt.h"

DEFINE_LOG_CATEGORY_STATIC(AuthScreen, Display, All)

void UAuthScreen::NativeConstruct()
{
	Super::NativeConstruct();

	SetIsLoading(false);

	// TODO: This ideally would exist in some kind of World Subsystem or GameInstance. Anywhere where it can be initialized once and then used throughout the game.
	UDiscoverySessionManager::Get().LoadFromDisk();

	EmailPrompt->OnCallEnded.BindUObject(this, &UAuthScreen::OnPromptCallFinished);
	TokenPrompt->OnCallEnded.BindUObject(this, &UAuthScreen::OnPromptCallFinished);
	ConsentsPrompt->OnCallEnded.BindUObject(this, &UAuthScreen::OnPromptCallFinished);

	EmailPrompt->OnCallBegan.BindUObject(this, &UAuthScreen::OnPromptCallBegan);
	TokenPrompt->OnCallBegan.BindUObject(this, &UAuthScreen::OnPromptCallBegan);
	ConsentsPrompt->OnCallBegan.BindUObject(this, &UAuthScreen::OnPromptCallBegan);

	EmailPrompt->OnManualChallengeCreated.BindWeakLambda(this, [this]()
	{
		TokenPrompt->SetEmail(EmailPrompt->GetEmail());
		WidgetSwitcher->SetActiveWidget(TokenPrompt);
	});

	FPlayerSessionResponseDelegate PlayerSessionCallback = FPlayerSessionResponseDelegate::CreateLambda(
		[this](FPlayerSession Session, bool bSuccess, FText Error)
		{
			if (bSuccess && Session.IsValid())
			{
				UE_LOG(AuthScreen, Log, TEXT("Player session is valid"));
				if (UGameService::Get().IsGameDetailsCached())
				{
					OnAuthComplete.Broadcast();
				}
				else
				{
					// Get game details then call OnAuthComplete
					FGameDetailsResponseDelegate OnGetGameDetails;
					OnGetGameDetails.BindWeakLambda(
						this, [this](UGameDataObject* GameDataObject, bool bSuccess, TOptional<FText> Error)
						{
							if (bSuccess && GameDataObject)
							{
								OnAuthComplete.Broadcast();
							}
							else
							{
								SetIsLoading(false);
								UE_LOG(AuthScreen, Error, TEXT("Verified session but failed to get game details: %s"),
								       *Error.GetValue().ToString());
							}
						});
					UGameService::Get().GetGameDetails(OnGetGameDetails);
				}
			}
			else
			{
				AttemptAutoAuth();
			}
		});

	FSimpleDelegate OnLoggedInSuccess = FSimpleDelegate::CreateLambda([this, PlayerSessionCallback]()
	{
		UE_LOG(AuthScreen, Verbose, TEXT("Logged in successfully. Verifying player session."));
		SetIsLoading(true);
		UIdentService::Get().VerifyPlayerSession(PlayerSessionCallback);
	});

	UIdentService::Get().OnAutoAuthSuccess = OnLoggedInSuccess;
	UIdentService::Get().OnAutoAuthRequiresConsent.BindWeakLambda(
		this, [this](FAcceptConsentsDelegate AcceptConsentsDelegate, const FString& IdentType)
		{
			WidgetSwitcher->SetActiveWidget(ConsentsPrompt);
			ConsentsPrompt->RetrieveConsentDetails(IdentType);
		});
	UIdentService::Get().OnAutoAuthFailure.BindWeakLambda(this, [this]()
	{
		AttemptAutoAuth();
	});

	TokenPrompt->OnLoggedInSuccess = OnLoggedInSuccess;

	TokenPrompt->OnConsentsRequireSignature.BindWeakLambda(this, [this]()
	{
		WidgetSwitcher->SetActiveWidget(ConsentsPrompt);
		ConsentsPrompt->RetrieveConsentDetails("email");
	});

	ConsentsPrompt->OnLoggedInSuccess = OnLoggedInSuccess;
	EmailPrompt->SetParentScreen(this);
	TokenPrompt->SetParentScreen(this);
	ConsentsPrompt->SetParentScreen(this);

	URemoteImageSubsystem& RemoteImageSubsystem = URemoteImageSubsystem::Get();
	if (UTexture2D* CachedGameBG = RemoteImageSubsystem.GetCachedGameBackground())
	{
		LazyGameBackgroundImage->SetBrushFromTexture(CachedGameBG);
	}

	// Actually start auth process
	if (UDiscoverySessionManager::Get().IsPlayerSessionValid())
	{
		SetIsLoading(true);
		UIdentService::Get().VerifyPlayerSession(PlayerSessionCallback);
	}
	else
	{
		AttemptAutoAuth();
	}
}

/* Get the game details, and then attempt to auto auth if possible,
 * otherwise will fallback to manual auth if available,
 * otherwise will just show an error message */
void UAuthScreen::AttemptAutoAuth()
{
	FGameDetailsResponseDelegate OnGetGameDetails;
	OnGetGameDetails.BindWeakLambda(this, [this](UGameDataObject* GameDataObject, bool bSuccess, TOptional<FText> Error)
	{
		// Once we get game details, we are either going to move straight into the application if the player session is valid,
		// or we are going to try to do auto auth if available, and if not we'll prompt the user to log in manually (email)
		if (bSuccess)
		{
			LazyGameBackgroundImage->LoadImageAsync(GameDataObject->GetImageBackgroundURL(), NC_CACHED_GAME_BACKGROUND_FILENAME);

			if (UIdentService::Get().IsAutoAuthAvailable())
			{
				SetIsLoading(true);
				UIdentService::Get().AttemptAutoAuth();
			}
			else
			{
				// TODO: We need to actually check if email is available as an auth method, and if not, we need to show a different prompt
				// Show email prompt
				SetIsLoading(false);
			}
		}
		else
		{
			// TODO: Better error handling here. Show error message on screen
			SetIsLoading(false);
			UE_LOG(AuthScreen, Error, TEXT("Failed to get game details: %s"), *Error.GetValue().ToString());
		}
	});
	SetIsLoading(true);
	UGameService::Get().GetGameDetails(OnGetGameDetails);
}

void UAuthScreen::OnPromptCallFinished()
{
	SetIsLoading(false);
}

void UAuthScreen::OnPromptCallBegan()
{
	SetIsLoading(true);
}

void UAuthScreen::SetIsLoading(const bool bIsLoading)
{
	LoadingSpinner->SetVisibility(bIsLoading ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	WidgetSwitcher->SetVisibility(bIsLoading ? ESlateVisibility::Collapsed : ESlateVisibility::Visible);
}

void UAuthScreen::Cancel()
{
	WidgetSwitcher->SetActiveWidgetIndex(0);
}
