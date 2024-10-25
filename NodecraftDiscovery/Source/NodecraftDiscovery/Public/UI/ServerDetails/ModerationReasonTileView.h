// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonTileView.h"
#include "ModerationReasonTileView.generated.h"

/**
 * 
 */
UCLASS()
class NODECRAFTDISCOVERY_API UModerationReasonTileView : public UCommonTileView
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FCustomWidgetNavigationDelegate BottomRowDownNavigationDelegate;
	UPROPERTY()
	FCustomWidgetNavigationDelegate TopRowUpNavigationDelegate;
	UPROPERTY()
	FCustomWidgetNavigationDelegate LeftColumnNavLeftDelegate;

protected:
	virtual UUserWidget& OnGenerateEntryWidgetInternal(UObject* Item, TSubclassOf<UUserWidget> DesiredEntryClass,
	                                                   const TSharedRef<STableViewBase>& OwnerTable) override;
};
