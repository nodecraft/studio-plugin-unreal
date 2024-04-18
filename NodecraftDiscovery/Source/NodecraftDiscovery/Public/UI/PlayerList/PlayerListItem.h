// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "CommonTextBlock.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "UI/Common/AsyncImage.h"
#include "UObject/Object.h"
#include "PlayerListItem.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class NODECRAFTDISCOVERY_API UPlayerListItem : public UCommonActivatableWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

	// IUserObjectListEntry
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
	// IUserObjectListEntry
	
protected:
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UAsyncImage* PlayerProfileImage;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UCommonTextBlock* PlayerNameTextBlock;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UCommonTextBlock* PlayerStatusTextBlock;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UImage* PlatformIcon;

private:
	void UpdateTimestamp(const FDateTime& Time, bool bIsStartTime);
};
