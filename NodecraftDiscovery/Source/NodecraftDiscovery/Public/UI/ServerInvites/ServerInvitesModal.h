// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "ServerInvitesModal.generated.h"

class UFriendDataObject;
class UCommonActivatableWidgetStack;
class UServerDataObject;
class UNodecraftButtonBase;
class UCommonTextBlock;
class UCommonListView;
class UNodecraftLoadGuard;
/**
 * 
 */
UCLASS()
class NODECRAFTDISCOVERY_API UServerInvitesModal : public UCommonActivatableWidget
{
	GENERATED_BODY()

public:
	void Configure(const TArray<UServerDataObject*>& Servers, const UFriendDataObject* Invitee, UCommonActivatableWidgetStack* PopupStack);

protected:
	virtual void NativeConstruct() override;
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UNodecraftLoadGuard* LoadGuard;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UCommonListView* ListView;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UCommonTextBlock* Title;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UNodecraftButtonBase* CloseButtonTop;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UNodecraftButtonBase* CloseButtonBottom;
};
