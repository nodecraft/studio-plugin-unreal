// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "ServerInvitesListItem.generated.h"

class UNodecraftButtonBase;
class UCommonTextBlock;
class UAsyncImage;
/**
 * 
 */
UCLASS()
class NODECRAFTDISCOVERY_API UServerInvitesListItem : public UCommonActivatableWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

protected:
	// IUserObjectListEntry
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
	// IUserObjectListEntry

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UAsyncImage* ServerImage;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UCommonTextBlock* ServerTitle;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UCommonTextBlock* ServerDescription;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UCommonTextBlock* PlayerCount;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UNodecraftButtonBase* InviteButton;
};
