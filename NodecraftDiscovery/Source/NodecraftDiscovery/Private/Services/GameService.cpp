// Fill out your copyright notice in the Description page of Project Settings.


#include "Services/GameService.h"

#include "JsonObjectWrapper.h"
#include "API/DiscoveryAPI.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "Models/GameDataObject.h"
#include "Models/ServerRegionDataObject.h"
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
				if (const TSharedPtr<FJsonObject> Data = ResJson.JsonObject->GetObjectField("data"); Data.IsValid())
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
				const FText ErrorMessage = UNodecraftUtility::ParseError(Res, FString(__FUNCTION__));
				OnComplete.ExecuteIfBound({}, false, TOptional<FText>(ErrorMessage));
			}
		}
		else
		{
			OnComplete.ExecuteIfBound({}, false, FText::FromString("UGameService::GetGameDetails: Failed to connect to server"));
		}
	});

	return UDiscoveryAPI::GameDetails(this, ReqCallback)->ProcessRequest();
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

bool UGameService::IsGameDetailsCached() const
{
	return GameDataObject != nullptr;
}
