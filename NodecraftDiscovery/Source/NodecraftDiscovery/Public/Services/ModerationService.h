// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/IHttpRequest.h"
#include "Subsystems/EngineSubsystem.h"
#include "Models/PlayerServerDetails.h"
#include "ModerationService.generated.h"

/**
 * 
 */

DECLARE_DELEGATE_ThreeParams(FGetPlayerServerDetails, UPlayerServerDetailsDataObject* /* Player */, bool /* bSuccess */, TOptional<FText> /* Error */);
DECLARE_DELEGATE_ThreeParams(FGetPlayerServerDetailsList, TArray<UPlayerServerDetailsDataObject*> /* Players */, bool /* bSuccess */, TOptional<FText> /* Error */);

UCLASS()
class NODECRAFTDISCOVERY_API UModerationService : public UEngineSubsystem
{
	GENERATED_BODY()

public:
	//getter
	static UModerationService& Get() { return *GEngine->GetEngineSubsystem<UModerationService>(); }

	void CreatePlayersListModerationDelegate(const FGetPlayerServerDetailsList& OnComplete, FHttpRequestCompleteDelegate& ReqCallbackOut);

	void CreateServerPlayersDetailDelegate(const FGetPlayerServerDetails& OnComplete, FHttpRequestCompleteDelegate& ReqCallbackOut);

	TSharedRef<IHttpRequest> GetModerators(const FString& ServerId, FGetPlayerServerDetailsList OnComplete);

	TSharedRef<IHttpRequest> GetOnlinePlayers(const FString& ServerId, FGetPlayerServerDetailsList OnComplete);

	TSharedRef<IHttpRequest> GetOfflinePlayers(const FString& ServerId, FGetPlayerServerDetailsList OnComplete);

	TSharedRef<IHttpRequest> GetBannedPlayers(const FString& ServerId, FGetPlayerServerDetailsList OnComplete);

	TSharedRef<IHttpRequest> GetOwner(const FString& ServerId, FGetPlayerServerDetails OnComplete);
};
