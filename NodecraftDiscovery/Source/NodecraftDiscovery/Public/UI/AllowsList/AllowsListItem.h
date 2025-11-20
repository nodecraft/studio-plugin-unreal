// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/Common/NodecraftListItemBase.h"
#include "AllowsListItem.generated.h"

class UAllowsDataObject;
class UNodecraftLoadGuard;
class UHorizontalBox;
class UCommonTextBlock;
class UImage;
class UAsyncImage;
/**
 * 
 */
UCLASS()
class NODECRAFTDISCOVERY_API UAllowsListItem : public UNodecraftListItemBase
{
	GENERATED_BODY()

protected:
	// IUserObjectListEntry
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
	// IUserObjectListEntry

	virtual FReply NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent) override;
	virtual void NativeOnFocusLost(const FFocusEvent& InFocusEvent) override;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Allows", meta=(BindWidget))
	UNodecraftLoadGuard* LoadGuard;
	
	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Allows", meta=(BindWidget))
	UAsyncImage* PlayerImage;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Allows", meta=(BindWidget))
	UImage* IdentImage;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Allows", meta=(BindWidget))
	UCommonTextBlock* Username;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Allows", meta=(BindWidget))
	UCommonTextBlock* ServerName;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Allows", meta=(BindWidget))
	UCommonTextBlock* RevokedOrExpiredText;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Allows", meta=(BindWidget))
	UHorizontalBox* ButtonsBox;

private:
	void UpdateActionBar();
	bool bShowActionsInActionBar = false;
};
