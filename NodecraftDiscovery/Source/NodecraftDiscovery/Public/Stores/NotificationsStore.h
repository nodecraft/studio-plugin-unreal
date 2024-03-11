// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Models/NotificationDataObject.h"
#include "Subsystems/WorldSubsystem.h"
#include "NotificationsStore.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnNotificationsUpdated, const TArray<UNotificationDataObject*>&);

/**
 * 
 */
UCLASS()
class NODECRAFTDISCOVERY_API UNotificationsStore : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	void StartPollingForNotifications();
	void StopPollingForNotifications();

	virtual void OnWorldBeginPlay(UWorld& InWorld) override;

	FDelegateHandle AddLiveNotificationListener(const FOnNotificationsUpdated::FDelegate& Delegate);
	bool RemoveLiveNotificationListener(const FDelegateHandle& Handle);

	TArray<FString> GetActiveNotificationIds();

	// TODO: Can remove this / consolidate
	void RemoveLiveNotification(UNotificationDataObject* Notification);
	void ClearAllNotifications();
	void ClearNotifications(const TArray<FString>& Array);

	static UNotificationsStore* Get(const UWorld* World)
	{
		if (World == nullptr)
		{
			return nullptr;
		}
		
		return World->GetSubsystem<UNotificationsStore>();
	}

private:
	FTimerHandle PollingTimerHandle;
	
	FOnNotificationsUpdated OnLiveNotificationsUpdated;
	FOnNotificationsUpdated OnHistoricalNotificationsUpdated;
	
	UPROPERTY()
	TArray<UNotificationDataObject*> LiveNotifications;

	UPROPERTY()
	TArray<UNotificationDataObject*> HistoricalNotifications;
};
