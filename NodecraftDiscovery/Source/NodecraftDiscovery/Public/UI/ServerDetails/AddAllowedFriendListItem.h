// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
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
class NODECRAFTDISCOVERY_API UAddAllowedFriendListItem : public UCommonActivatableWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

protected:
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UAsyncImage* Avatar;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UCommonTextBlock* Status;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UImage* IdentImage;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UCommonTextBlock* PlayerIdent;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UNodecraftButtonBase* InviteButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UAlertMessage* Alert;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TMap<FString, TSoftClassPtr<UCommonTextStyle>> StatusStyle;

private:
	FDelegateHandle OnServerUpdated;
	FString CurrentServerId;
};
