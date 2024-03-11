// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerReputationService.generated.h"

class UAccountReputationEntryDataObject;

DECLARE_DELEGATE_ThreeParams(FGetPlayerReputationDelegate, TArray<UAccountReputationEntryDataObject*> /*AccountReputation*/, bool /*bSuccess*/, TOptional<FText> /*Error*/);

/**
 * 
 */
UCLASS()
class NODECRAFTDISCOVERY_API UPlayerReputationService : public UEngineSubsystem
{
	GENERATED_BODY()

public:
	static UPlayerReputationService& Get() { return *GEngine->GetEngineSubsystem<UPlayerReputationService>(); }

	bool GetPlayerReputation(FGetPlayerReputationDelegate& OnComplete);
};
