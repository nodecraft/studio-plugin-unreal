// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "AccountReputationListItem.generated.h"

class UCommonTextStyle;
enum class EModerationDuration : uint8;
class UCommonTextBlock;
/**
 * 
 */
UCLASS()
class NODECRAFTDISCOVERY_API UAccountReputationListItem : public UCommonActivatableWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

protected:
	// IUserObjectListEntry
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
	// IUserObjectListEntry

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UCommonTextBlock* BanScope;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UCommonTextBlock* ServerOrCommunityName;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UCommonTextBlock* ModerationDate;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UCommonTextBlock* ExpirationText;

	UPROPERTY(EditDefaultsOnly)
	TMap<EModerationDuration, TSoftClassPtr<UCommonTextStyle>> ExpirationTextStyle;
};
