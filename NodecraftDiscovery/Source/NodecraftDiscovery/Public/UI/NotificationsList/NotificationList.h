// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "CommonListView.h"
#include "CommonTextBlock.h"
#include "API/NotificationListType.h"
#include "Models/NotificationDataObject.h"
#include "UI/Alerts/AlertMessage.h"
#include "UI/Common/NodecraftLoadGuard.h"
#include "NotificationList.generated.h"



/**
 * 
 */
UCLASS()
class NODECRAFTDISCOVERY_API UNotificationList : public UCommonActivatableWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UCommonListView* NotifsListView;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UNamedSlot* NoNotificationsSlot;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UAlertMessage* AlertMessageWidget;

public:
	void LoadData(ENotificationListType NotifListType, FSimpleDelegate OnComplete);
	void SetListItems(const TArray<UNotificationDataObject*>& Notifs);
	virtual void NativeConstruct() override;
};
