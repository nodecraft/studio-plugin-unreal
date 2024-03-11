// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "CommonButtonBase.h"
#include "CommonLazyImage.h"
#include "CommonTextBlock.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "Models/ServerDataObject.h"
#include "UI/Common/AsyncImage.h"
#include "ServerCard.generated.h"

/**
 * 
 */
UCLASS()
class NODECRAFTDISCOVERY_API UServerCard : public UCommonActivatableWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

	// IUserObjectListEntry
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
	// IUserObjectListEntry

protected:
	UPROPERTY(meta=(BindWidget))
	UCommonTextBlock* ServerNameTextBlock;

	UPROPERTY(meta=(BindWidget))
	UCommonTextBlock* ServerPlayersTextBlock;

	UPROPERTY(meta=(BindWidget))
	UCommonTextBlock* ServerDescriptionTextBlock;	

	UPROPERTY(meta=(BindWidget))
	UAsyncImage* ServerImage;

	UPROPERTY(meta=(BindWidget))
	UCommonButtonBase* JoinButton;

	UPROPERTY(meta=(BindWidget))
	UUserWidget* FavoriteTag;

	UPROPERTY(meta=(BindWidget))
	UUserWidget* PrivateTag;

	UPROPERTY()
	UServerDataObject* ServerDataObject = nullptr;

	UFUNCTION(BlueprintCallable)
	void OpenServerDetails();

};
