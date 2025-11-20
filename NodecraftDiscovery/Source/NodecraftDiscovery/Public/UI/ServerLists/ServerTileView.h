// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonTileView.h"
#include "ServerTileView.generated.h"

/**
 * Used to display the server tiles in the server list.
 * We need this to ensure that the navigation rules are set correctly when we have placeholders for (no-server) present.
 */
UCLASS()
class NODECRAFTDISCOVERY_API UServerTileView : public UCommonTileView
{
	GENERATED_BODY()

public:
	virtual UUserWidget& OnGenerateEntryWidgetInternal(UObject* Item, TSubclassOf<UUserWidget> DesiredEntryClass, const TSharedRef<STableViewBase>& OwnerTable) override;
};
