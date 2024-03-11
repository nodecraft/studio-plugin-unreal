// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "CommonButtonBase.h"
#include "NotificationList.h"
#include "Stores/NotificationsStore.h"
#include "UI/Foundation/NodecraftButtonBase.h"
#include "NotificationsDrawerWidget.generated.h"

/**
 * 
 */
UCLASS()
class NODECRAFTDISCOVERY_API UNotificationsDrawerWidget : public UCommonActivatableWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UNotificationList* ActiveNotificationsList;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UNotificationList* HistoricNotificationsList;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UCommonTextBlock* HeaderTextBlock;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UCommonButtonBase* ClearAllButton;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UNodecraftButtonBase* SwitchNotifsPageButton;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UNodecraftLoadGuard* LoadGuard;

	FDelegateHandle LiveNotifsListenerHandle;

public:
	virtual void NativeOnInitialized() override;

	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;

	void ShowNotifsPage(ENotificationListType NotifListType);
};
