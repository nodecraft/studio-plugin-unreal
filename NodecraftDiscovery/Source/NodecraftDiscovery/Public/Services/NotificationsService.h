// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ServiceDelegates.h"
#include "Models/NotificationDataObject.h"
#include "Subsystems/EngineSubsystem.h"
#include "NotificationsService.generated.h"

DECLARE_DELEGATE_ThreeParams(FGetNotificationsResponseDelegate, TArray<UNotificationDataObject*> /*Notifications*/, bool /*bSuccess*/, FText /*Error*/);

/**
 * 
 */
UCLASS()
class NODECRAFTDISCOVERY_API UNotificationsService : public UEngineSubsystem
{
	GENERATED_BODY()

public:
	static UNotificationsService& Get() { return *GEngine->GetEngineSubsystem<UNotificationsService>(); }

	bool GetNotifications(FGetNotificationsResponseDelegate& OnComplete);
	bool GetNotificationsHistory(FGetNotificationsResponseDelegate& OnComplete);
	bool DismissNotification(const UWorld* World, const FString& NotificationId);
	bool DismissNotification(const UWorld* World, const FString& NotificationId, FSimpleServiceResponseDelegate& OnComplete);
	bool DismissAllNotifications(UWorld* World, FSimpleServiceResponseDelegate& OnComplete);
};
