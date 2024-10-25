// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/Common/NodecraftListItemBase.h"
#include "ServerModerationHistoryListItem.generated.h"

class UPlayerPlatformIcon;
enum class EModerationDuration : uint8;
class UCommonTextStyle;
class UCommonDateTimeTextBlock;
class UCommonTextBlock;
/**
 * 
 */
UCLASS()
class NODECRAFTDISCOVERY_API UServerModerationHistoryListItem : public UNodecraftListItemBase
{
	GENERATED_BODY()

protected:
	// IUserObjectListEntry
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
	// IUserObjectListEntry

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Details|Moderation Console", meta = (BindWidget))
	UPlayerPlatformIcon* IdentImage;
	
	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Details|Moderation Console", meta = (BindWidget))
	UCommonTextBlock* Username;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Details|Moderation Console", meta = (BindWidget))
	UCommonTextBlock* ModerationType;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Details|Moderation Console", meta = (BindWidget))
	UCommonTextBlock* ModerationReason;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Details|Moderation Console", meta = (BindWidget))
	UCommonTextBlock* ModerationDate;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Details|Moderation Console", meta = (BindWidget))
	UCommonTextBlock* Expires;

	UPROPERTY(EditDefaultsOnly, Category = "Nodecraft UI|Server Details|Styles")
	TMap<EModerationDuration, TSoftClassPtr<UCommonTextStyle>> ExpirationTextStyle;

private:
	void UpdateExpirationTextStyle(EModerationDuration ModerationDuration);
};
