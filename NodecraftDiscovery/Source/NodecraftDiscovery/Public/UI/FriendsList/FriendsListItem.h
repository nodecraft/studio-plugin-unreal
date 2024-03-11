// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "FriendsListItem.generated.h"

class UCommonBorder;
class UServerInvitesModal;
class UCommonTextStyle;
class UNodecraftButtonBase;
class UImage;
class UCommonTextBlock;
class UAsyncImage;
/**
 * 
 */
UCLASS()
class NODECRAFTDISCOVERY_API UFriendsListItem : public UCommonActivatableWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

	// IUserObjectListEntry
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
	// IUserObjectListEntry

protected:
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UAsyncImage* FriendProfileImage;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UCommonTextBlock* FriendNameTextBlock;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UCommonTextBlock* FriendStatusTextBlock;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UNodecraftButtonBase* InviteToPlayButton;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UCommonBorder* JoinServerCardContainer;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UCommonTextBlock* ServerName;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UCommonTextBlock* PlayersOnlineText;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UNodecraftButtonBase* JoinButton;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TMap<FString, TSoftClassPtr<UCommonTextStyle>> StatusStyle;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TSoftClassPtr<UServerInvitesModal> ServerInvitesModalClass;
};
