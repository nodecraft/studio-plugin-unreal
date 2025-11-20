// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/Common/NodecraftListItemBase.h"
#include "FriendsListItem.generated.h"

class UCommonBorder;
class UServerInvitesModal;
class UCommonTextStyle;
class UImage;
class UCommonTextBlock;
class UAsyncImage;
/**
 * 
 */
UCLASS()
class NODECRAFTDISCOVERY_API UFriendsListItem : public UNodecraftListItemBase
{
	GENERATED_BODY()

protected:
	// IUserObjectListEntry
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
	// IUserObjectListEntry

	virtual FReply NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent) override;
	virtual void NativeOnFocusLost(const FFocusEvent& InFocusEvent) override;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Friends List Item", meta=(BindWidget))
	UAsyncImage* FriendProfileImage;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Friends List Item", meta=(BindWidget))
	UCommonTextBlock* FriendNameTextBlock;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Friends List Item", meta=(BindWidget))
	UCommonTextBlock* FriendStatusTextBlock;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Friends List Item", meta=(BindWidget))
	UCommonBorder* JoinServerCardContainer;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Friends List Item", meta=(BindWidget))
	UCommonTextBlock* ServerName;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Friends List Item", meta=(BindWidget))
	UCommonTextBlock* PlayersOnlineText;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Nodecraft UI|Friends List Item|Styles")
	TMap<FString, TSoftClassPtr<UCommonTextStyle>> StatusStyle;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Nodecraft UI|Friends List Item")
	TSoftClassPtr<UServerInvitesModal> ServerInvitesModalClass;
};
