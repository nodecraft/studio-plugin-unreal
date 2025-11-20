// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Common/NodecraftDelegates.h"
#include "UI/Common/NodecraftListItemBase.h"
#include "ServerInvitesListItem.generated.h"

class UCommonTextBlock;
class UAsyncImage;
/**
 * 
 */
UCLASS()
class NODECRAFTDISCOVERY_API UServerInvitesListItem : public UNodecraftListItemBase
{
	GENERATED_BODY()

public:
	FOnNavigationDelegate OnNavDelegate;

protected:
	// IUserObjectListEntry
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
	// IUserObjectListEntry

	virtual FNavigationReply NativeOnNavigation(const FGeometry& MyGeometry, const FNavigationEvent& InNavigationEvent, const FNavigationReply& InDefaultReply) override;
	virtual FReply NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent) override;
	virtual void NativeOnFocusLost(const FFocusEvent& InFocusEvent) override;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Invites", meta=(BindWidget))
	UAsyncImage* ServerImage;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Invites", meta=(BindWidget))
	UCommonTextBlock* ServerTitle;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Invites", meta=(BindWidget))
	UCommonTextBlock* ServerDescription;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Invites", meta=(BindWidget))
	UCommonTextBlock* PlayerCount;
};
