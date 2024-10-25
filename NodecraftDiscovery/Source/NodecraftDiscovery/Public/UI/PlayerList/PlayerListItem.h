// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonTextBlock.h"
#include "UI/Common/AsyncImage.h"
#include "UI/Common/NodecraftListItemBase.h"
#include "UObject/Object.h"
#include "PlayerListItem.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class NODECRAFTDISCOVERY_API UPlayerListItem : public UNodecraftListItemBase
{
	GENERATED_BODY()

	// IUserObjectListEntry
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
	// IUserObjectListEntry
	
protected:
	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Player List Item", meta=(BindWidget))
	UAsyncImage* PlayerProfileImage;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Player List Item", meta=(BindWidget))
	UCommonTextBlock* PlayerNameTextBlock;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Player List Item", meta=(BindWidget))
	UCommonTextBlock* PlayerStatusTextBlock;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Player List Item", meta=(BindWidget))
	UImage* PlatformIcon;

private:
	void UpdateTimestamp(const FDateTime& Time, bool bIsStartTime);
};
