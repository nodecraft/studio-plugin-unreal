// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/Common/NodecraftListItemBase.h"
#include "AccountReputationListItem.generated.h"

class UCommonTextStyle;
enum class EModerationDuration : uint8;
class UCommonTextBlock;
/**
 * 
 */
UCLASS()
class NODECRAFTDISCOVERY_API UAccountReputationListItem : public UNodecraftListItemBase
{
	GENERATED_BODY()

protected:
	// IUserObjectListEntry
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
	// IUserObjectListEntry

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Account Reputation", meta = (BindWidget))
	UCommonTextBlock* BanScope;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Account Reputation", meta = (BindWidget))
	UCommonTextBlock* ServerOrCommunityName;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Account Reputation", meta = (BindWidget))
	UCommonTextBlock* ModerationDate;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Account Reputation", meta = (BindWidget))
	UCommonTextBlock* ExpirationText;

	UPROPERTY(EditDefaultsOnly, Category = "Nodecraft UI|Styles")
	TMap<EModerationDuration, TSoftClassPtr<UCommonTextStyle>> ExpirationTextStyle;
};
