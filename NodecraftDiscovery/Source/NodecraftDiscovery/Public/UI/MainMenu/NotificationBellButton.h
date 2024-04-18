// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Stores/NotificationsStore.h"
#include "UI/Common/NodecraftBadgedButton.h"
#include "NotificationBellButton.generated.h"

/**
 * 
 */
UCLASS()
class NODECRAFTDISCOVERY_API UNotificationBellButton : public UNodecraftBadgedButton
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

private:
	FDelegateHandle LiveNotifsListenerHandle;

};
