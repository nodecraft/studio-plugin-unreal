// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

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
