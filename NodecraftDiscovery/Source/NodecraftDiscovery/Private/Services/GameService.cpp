// Nodecraft, Inc. © 2012-2024, All Rights Reserved.


#include "Services/GameService.h"

#include "JsonObjectWrapper.h"
#include "API/NodecraftStudioApi.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "Models/GameDataObject.h"
#include "Models/ServerRegionDataObject.h"
#include "Subsystems/MessageRouterSubsystem.h"
#include "Subsystems/RemoteImageSubsystem.h"
#include "Utility/NodecraftUtility.h"


bool UGameService::GetGameDetails(FGameDetailsResponseDelegate OnComplete)
{
	FHttpRequestCompleteDelegate ReqCallback;
	ReqCallback.BindWeakLambda(this, [this, OnComplete](FHttpRequestPtr Req, FHttpResponsePtr Res, bool bConnectedSuccessfully)
	{
		if (bConnectedSuccessfully)
		{
			if (EHttpResponseCodes::IsOk(Res.Get()->GetResponseCode()))
			{
				FJsonObjectWrapper ResJson;
				ResJson.JsonObjectFromString(Res.Get()->GetContentAsString());
				if (const TSharedPtr<FJsonObject> Data = ResJson.JsonObject->GetObjectField(TEXT("data")); Data.IsValid())
				{
					GameDataObject = UGameDataObject::FromJson(Data.ToSharedRef());
					// Cache the game background image for the next time the user logs in, in case we've never received it
					URemoteImageSubsystem::Get().CacheGameBackgroundImage(false);
					OnComplete.ExecuteIfBound(GameDataObject, true, FText());
				}
				else
				{
					OnComplete.ExecuteIfBound({}, false, FText::FromString("UGameService::GetGameDetails: Error while grabbing data from response"));
				}
			}
			else
			{
				const FText ErrorMessage = UNodecraftUtility::ParseMessage(Res, FString(__FUNCTION__));
				OnComplete.ExecuteIfBound({}, false, TOptional<FText>(ErrorMessage));
			}
			UMessageRouterSubsystem::Get().RouteHTTPResult(Res, __FUNCTION__);
		}
		else
		{
			UMessageRouterSubsystem::Get().RouteFailureToConnect(__FUNCTION__);
			OnComplete.ExecuteIfBound({}, false, FText::FromString("UGameService::GetGameDetails: Failed to connect to server"));
		}
	});

	return UNodecraftStudioApi::GameDetails(this, ReqCallback)->ProcessRequest();
}

TArray<UServerRegionDataObject*> UGameService::GetCachedServerRegions() const
{
	return GameDataObject->GetServerRegions();
}

TArray<EIdentityType> UGameService::GetCachedIdentTypes() const
{
	return GameDataObject->GetSupportedIdentTypes();
}

FString UGameService::GetCachedGameBackgroundURL() const
{
	return GameDataObject->GetImageBackgroundURL();
}

FString UGameService::GetCachedGameLogoURL() const
{
	return GameDataObject->GetGameLogoURL();
}

bool UGameService::IsGameDetailsCached() const
{
	return GameDataObject != nullptr;
}

FDelegateHandle UGameService::AddPlayerInGameStateListener(const FOnPlayerInGameStateChanged::FDelegate& Delegate)
{
	Delegate.ExecuteIfBound(bIsPlayerInGame);
	return OnPlayerInGameStateChanged.Add(Delegate);
}

void UGameService::SetIsPlayerInGame(bool bInGame)
{
	bIsPlayerInGame = bInGame;
	OnPlayerInGameStateChanged.Broadcast(bIsPlayerInGame);
}

#if !UE_BUILD_SHIPPING
static FAutoConsoleCommand ConsoleCommandUpdateState(
	TEXT("NC.GameService.UpdatePlayerInGameState"),
	TEXT("Arguments: [0]:state (0/1)")
	TEXT("Set the player's in game state\n"),
	FConsoleCommandWithArgsDelegate::CreateLambda([](const TArray<FString>& Args)
	{
		if (Args.Num() > 0 && (Args[0] == "0" || Args[0] == "1"))
		{
			const bool PlayerInGame = Args[0] == "1";

			UGameService::Get().SetIsPlayerInGame(PlayerInGame);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Need 1 argument (0 or 1) for this command to run."));
		}
	}),
	ECVF_Cheat);
#endif
