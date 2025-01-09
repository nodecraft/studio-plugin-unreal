// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/Common/NodecraftListItemBase.h"
#include "ModerationConsoleNoPlayersFoundWidget.generated.h"

class UCommonTextBlock;
/**
 * 
 */
UCLASS()
class NODECRAFTDISCOVERY_API UModerationConsoleNoPlayersFoundWidget : public UNodecraftListItemBase
{
	GENERATED_BODY()

protected:
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
	
	UPROPERTY(meta = (BindWidget))
	UCommonTextBlock* NoPlayersFoundText;
};
