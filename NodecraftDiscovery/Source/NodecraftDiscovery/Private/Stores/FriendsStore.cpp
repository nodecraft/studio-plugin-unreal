// Nodecraft, Inc. © 2012-2024, All Rights Reserved.


#include "Stores/FriendsStore.h"

#include "NodecraftLogCategories.h"
#include "API/NodecraftStudioSessionManagerSubsystem.h"
#include "DeveloperSettings/NodecraftStudioApiSettings.h"
#include "Services/FriendsService.h"
#include "Services/GameService.h"
#include "TimerManager.h"

void UFriendsStore::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);

	// the current in-game state of the player is broadcasted by the game service when we subscribe to it
	FDelegateHandle OnPlayerInGameStateChanged = UGameService::Get().AddPlayerInGameStateListener(FOnPlayerInGameStateChanged::FDelegate::CreateWeakLambda(
		this, [this](bool bIsInGame)
		{
			if (const UNodecraftStudioApiSettings* DiscoveryAPISettings = GetDefault<UNodecraftStudioApiSettings>())
			{
				PollingFrequency = bIsInGame ? DiscoveryAPISettings->GetFriendsPollingIntervalInGame() : DiscoveryAPISettings->GetFriendsPollingIntervalOutOfGame();
			}
			else
			{
				UE_LOG(LogNodecraft_Friends, Error, TEXT("UFriendsStore::OnWorldBeginPlay: Failed to get discovery api settings. Will poll every %f seconds."), PollingFrequency);
			}
			
			StopPollingForFriends();
			StartPollingForFriends();
		}));
}

void UFriendsStore::StartPollingForFriends()
{
	FTimerDelegate TimerDelegate;

	TimerDelegate.BindWeakLambda(this, [this]()
	{
		if (UNodecraftStudioSessionManager::Get().IsPlayerSessionValid())
		{
			FGetFriendsListDelegate FriendsListReceivedDelegate = FGetFriendsListDelegate::CreateWeakLambda(this, [this]
				(TArray<UFriendDataObject*> FriendDataObjects, bool bSuccess, TOptional<FText> Error)
			{
				if (bSuccess)
				{
					Friends = FriendDataObjects;
					OnFriendsUpdated.Broadcast(FriendDataObjects);
				}
			});
			UFriendsService::Get().GetFriends(FriendsListReceivedDelegate);
		}
		else
		{
			// Cancel timer here?
		}
	});

	if (const UWorld* World = GetWorld())
	{
		World->GetTimerManager().SetTimer(PollingTimerHandle, TimerDelegate, PollingFrequency, true, 0.0f);
	}
	else
	{
		UE_LOG(LogNodecraft_Friends, Error, TEXT("UFriendsStore::StartPollingForFriends: Failed to get world. Polling will not be started."));
	}
}

void UFriendsStore::StopPollingForFriends()
{
	if (const UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(PollingTimerHandle);
	}
	else
	{
		UE_LOG(LogNodecraft_Friends, Error, TEXT("UFriendsStore::StopPollingForFriends: Failed to get world. Polling will not be stopped."));
	}
}

FDelegateHandle UFriendsStore::AddFriendsUpdatedListener(const FOnFriendsUpdated::FDelegate& Delegate)
{
	Delegate.ExecuteIfBound(Friends);
	return  OnFriendsUpdated.Add(Delegate);
}

bool UFriendsStore::RemoveFriendsUpdatedListener(const FDelegateHandle& Handle)
{
	return OnFriendsUpdated.Remove(Handle);
}
