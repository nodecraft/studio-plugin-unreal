// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/EngineSubsystem.h"
#include "Engine/Engine.h"
#include "IdentityCommon.h"
#include "JsonObjectWrapper.h"
#include "Interfaces/IHttpRequest.h"
#include "Models/Consents.h"
#include "Models/PlayerSession.h"
#include "UI/Auth/Auth_TermsOfServicePrompt.h"
#include "IdentService.generated.h"

class UPlayerSettings;

DECLARE_DELEGATE_ThreeParams(FPlayerSessionResponseDelegate, FPlayerSession /*Session*/, bool /*bSuccess*/, FText /*Error*/);

/**
 * 
 */
UCLASS()
class NODECRAFTDISCOVERY_API UIdentService : public UEngineSubsystem
{
	GENERATED_BODY()

public:
	static UIdentService& Get() { return *GEngine->GetEngineSubsystem<UIdentService>(); }

	EIdentityType GetNextAutoAuthPlatformToTry();
	bool IsAutoAuthAvailable();
	void AttemptAutoAuth();
	
#if NC_AUTH_STEAM_ENABLED
	void AttemptSteamAuth();
	bool SendIdentTokenSteam(const FString& Ticket, const FString& Identity, FSendIdentTokenResponseDelegate& OnComplete, TOptional<FGameConsents>
							 GameConsents = TOptional<FGameConsents>());
#endif
	
	bool SendEmailToken(FString Email, FString Token, FSendIdentTokenResponseDelegate& OnComplete, TOptional<FGameConsents> GameConsents = TOptional<FGameConsents>());

	bool VerifyPlayerSession(FPlayerSessionResponseDelegate OnComplete);
	bool ResendPreviousTokenWithConsents(const FSendIdentTokenResponseDelegate& Delegate,
	                                     const FGameConsents& InConsents);

	// For use in other subsystems
	FSimpleDelegate OnPlayerLoggedIn;
	// TODO: THIS STILL NEEDS TO BE IMPLEMENTED FOR UI TO WORK
	FSimpleDelegate OnPlayerLoggedOut;
	// End of use in other subsystems
	
	FSimpleDelegate OnAutoAuthSuccess;
	FOnAuthRequiresConsentsDelegate OnAutoAuthRequiresConsent;
	FSimpleDelegate OnAutoAuthFailure;

	void SetHasFailedWithIdentityType(EIdentityType IdentityType);
	void ClearFailedIdentityTypes();

private:
	// Cached consents from the last token request so that we can resend the token with the consents if necessary
	FJsonObjectWrapper LastIdentTokenJsonPayload;
	// Cached send consents delegate from the last token request so that we can reuse it if, even after sending consents, we get a 409 conflict again
	// This hopefully never happens, but at least this way there will be a response of some kind
	FAcceptConsentsDelegate LastAcceptConsentsDelegate;

	FHttpRequestCompleteDelegate CreateSendTokenCompleteDelegate(
		FAcceptConsentsDelegate AcceptConsentsDelegate, FSendIdentTokenResponseDelegate OnComplete);

	// Identity types which we have failed to authenticate with. 
	TArray<EIdentityType> FailedIdentityTypes;
};
