// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "ServerModerationHistoryListItem.generated.h"

enum class EModerationDuration : uint8;
class UCommonTextStyle;
class UCommonDateTimeTextBlock;
class UCommonTextBlock;
/**
 * 
 */
UCLASS()
class NODECRAFTDISCOVERY_API UServerModerationHistoryListItem : public UCommonActivatableWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

protected:
	// IUserObjectListEntry
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
	// IUserObjectListEntry

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UCommonTextBlock* Username;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UCommonTextBlock* ModerationDate;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UCommonTextBlock* Expires;

	UPROPERTY(EditDefaultsOnly)
	TMap<EModerationDuration, TSoftClassPtr<UCommonTextStyle>> ExpirationTextStyle;
};
