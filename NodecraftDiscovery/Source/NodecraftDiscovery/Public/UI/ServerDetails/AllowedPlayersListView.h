// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonListView.h"
#include "AllowedPlayersListView.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class NODECRAFTDISCOVERY_API UAllowedPlayersListView : public UCommonListView
{
	GENERATED_BODY()

protected:
	virtual UUserWidget& OnGenerateEntryWidgetInternal(UObject* Item, TSubclassOf<UUserWidget> DesiredEntryClass, const TSharedRef<STableViewBase>& OwnerTable) override;
};
