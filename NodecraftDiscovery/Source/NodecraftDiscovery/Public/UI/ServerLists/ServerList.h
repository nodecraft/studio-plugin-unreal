// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "CommonTextBlock.h"
#include "CommonTileView.h"
#include "API/ServerListType.h"
#include "ServerList.generated.h"

class UServerDataObject;
/**
 * 
 */
UCLASS()
class NODECRAFTDISCOVERY_API UServerList : public UCommonActivatableWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	
	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server List", meta=(BindWidget))
	UCommonTextBlock* ServerListTitleTextBlock;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server List", meta=(BindWidget))
	UCommonTileView* ServerListView;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server List", meta=(BindWidget))
	UCommonTileView* LoadingServersListView;

	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "Nodecraft UI|Server List")
	EServerListType ServerListType;
};
