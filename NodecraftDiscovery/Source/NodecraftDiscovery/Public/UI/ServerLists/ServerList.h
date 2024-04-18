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
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UCommonTextBlock* ServerListTitleTextBlock;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UCommonTileView* ServerListView;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UCommonTileView* LoadingServersListView;

	UPROPERTY(BlueprintReadOnly, EditInstanceOnly)
	EServerListType ServerListType;

public:
	// void PopulateWithServerJson(const TArray<TSharedPtr<FJsonObject>> Json);

	virtual void NativeConstruct() override;
};
