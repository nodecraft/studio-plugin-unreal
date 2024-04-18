// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/IHttpRequest.h"
#include "Subsystems/EngineSubsystem.h"
#include "FriendsService.generated.h"

class UFriendDataObject;

DECLARE_DELEGATE_ThreeParams(FGetFriendsListDelegate, TArray<UFriendDataObject*>, /* Friends */ bool, /* Success */ TOptional<FText> /* Error */);

UCLASS()
class NODECRAFTDISCOVERY_API UFriendsService : public UEngineSubsystem
{
	GENERATED_BODY()

public:
	static UFriendsService& Get() { return *GEngine->GetEngineSubsystem<UFriendsService>(); }

	void CreateFriendsListDelegate(const FGetFriendsListDelegate& OnComplete, FHttpRequestCompleteDelegate& ReqCallbackOut);
	
	bool GetFriends(FGetFriendsListDelegate& OnComplete);
};
