// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Engine/World.h"
#include "FriendsStore.generated.h"

class UFriendDataObject;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnFriendsUpdated, const TArray<UFriendDataObject*>&);

/**
 * 
 */
UCLASS()
class NODECRAFTDISCOVERY_API UFriendsStore : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	static UFriendsStore* Get(const UWorld* World)
	{
		if (World == nullptr)
		{
			return nullptr;
		}
		
		return World->GetSubsystem<UFriendsStore>();
	}

	virtual void OnWorldBeginPlay(UWorld& InWorld) override;

	void StartPollingForFriends();
	void StopPollingForFriends();

	FDelegateHandle AddFriendsUpdatedListener(const FOnFriendsUpdated::FDelegate& Delegate);
	bool RemoveFriendsUpdatedListener(const FDelegateHandle& Handle);

private:
	// the default value is used only if we fail to get the API settings
	float PollingFrequency = 30.f;
	
	FTimerHandle PollingTimerHandle;

	FOnFriendsUpdated OnFriendsUpdated;

	UPROPERTY()
	TArray<UFriendDataObject*> Friends;
};
