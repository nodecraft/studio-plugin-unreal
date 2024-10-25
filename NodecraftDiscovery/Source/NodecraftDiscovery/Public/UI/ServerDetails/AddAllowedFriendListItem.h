// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Common/NodecraftDelegates.h"
#include "UI/Common/NodecraftListItemBase.h"
#include "AddAllowedFriendListItem.generated.h"

class UCommonTextStyle;
class UAlertMessage;
class UNodecraftButtonBase;
class UImage;
class UCommonTextBlock;
class UAsyncImage;
/**
 * 
 */
UCLASS()
class NODECRAFTDISCOVERY_API UAddAllowedFriendListItem : public UNodecraftListItemBase
{
	GENERATED_BODY()

public:
	FOnNavigationDelegate OnNavDelegate;

protected:
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
	virtual FNavigationReply NativeOnNavigation(const FGeometry& MyGeometry, const FNavigationEvent& InNavigationEvent, const FNavigationReply& InDefaultReply) override;
	virtual FReply NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent) override;
	virtual void NativeOnFocusLost(const FFocusEvent& InFocusEvent) override;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Details", meta = (BindWidget))
	UAsyncImage* Avatar;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Details", meta = (BindWidget))
	UCommonTextBlock* Status;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Details", meta = (BindWidget))
	UImage* IdentImage;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Details", meta = (BindWidget))
	UCommonTextBlock* PlayerIdent;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Details", meta = (BindWidget))
	UAlertMessage* Alert;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Nodecraft UI|Styles")
	TMap<FString, TSoftClassPtr<UCommonTextStyle>> StatusStyle;

private:
	FDelegateHandle OnServerUpdated;
	FString CurrentServerId;
};
