// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "CommonButtonBase.h"
#include "CommonTextBlock.h"
#include "NotificationList.h"
#include "Stores/NotificationsStore.h"
#include "UI/Common/NodecraftLoadGuard.h"
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
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual UWidget* NativeGetDesiredFocusTarget() const override;

	void ShowNotifsPage(ENotificationListType NotifListType);

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Notification Drawer", meta=(BindWidget))
	UNotificationList* ActiveNotificationsList;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Notification Drawer", meta=(BindWidget))
	UNotificationList* HistoricNotificationsList;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Notification Drawer", meta=(BindWidget))
	UCommonTextBlock* HeaderTextBlock;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Notification Drawer", meta=(BindWidget))
	UCommonButtonBase* ClearAllButton;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Notification Drawer", meta=(BindWidget))
	UNodecraftButtonBase* SwitchNotifsPageButton;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Notification Drawer", meta=(BindWidget))
	UNodecraftLoadGuard* LoadGuard;

	FDelegateHandle LiveNotifsListenerHandle;
};
