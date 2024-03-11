// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "FriendsList.generated.h"

class UCommonTextBlock;
class UFriendDataObject;
class UCommonListView;
class UCommonLoadGuard;
/**
 * 
 */
UCLASS()
class NODECRAFTDISCOVERY_API UFriendsList : public UCommonActivatableWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UCommonLoadGuard* LoadGuard;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UCommonListView* FriendListView;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UCommonTextBlock* FriendCountText;
	
public:

	virtual void NativeConstruct() override;
	
	void LoadData();
};
