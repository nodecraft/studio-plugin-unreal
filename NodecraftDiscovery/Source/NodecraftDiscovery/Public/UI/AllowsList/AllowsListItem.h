// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "AllowsListItem.generated.h"

class UNodecraftLoadGuard;
class UHorizontalBox;
class UNodecraftButtonBase;
class UCommonTextBlock;
class UImage;
class UAsyncImage;
/**
 * 
 */
UCLASS()
class NODECRAFTDISCOVERY_API UAllowsListItem : public UCommonActivatableWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

protected:
	// IUserObjectListEntry
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
	// IUserObjectListEntry

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UNodecraftLoadGuard* LoadGuard;
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UAsyncImage* PlayerImage;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UImage* IdentImage;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UCommonTextBlock* Username;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UCommonTextBlock* ServerName;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UCommonTextBlock* RevokedOrExpiredText;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UHorizontalBox* ButtonsBox;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UNodecraftButtonBase* AcceptButton;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UNodecraftButtonBase* DeclineButton;
};
