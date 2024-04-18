// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "CommonListView.h"
#include "API/NotificationListType.h"
#include "Models/NotificationDataObject.h"
#include "UI/Alerts/AlertMessage.h"
#include "NotificationList.generated.h"



/**
 * 
 */
UCLASS()
class NODECRAFTDISCOVERY_API UNotificationList : public UCommonActivatableWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UCommonListView* NotifsListView;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UNamedSlot* NoNotificationsSlot;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UAlertMessage* AlertMessageWidget;

public:
	void LoadData(ENotificationListType NotifListType, FSimpleDelegate OnComplete);
	void SetListItems(const TArray<UNotificationDataObject*>& Notifs);
};
