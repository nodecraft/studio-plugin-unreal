// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "UI/ServerLists/ServerList.h"
#include "Widgets/CommonActivatableWidgetContainer.h"
#include "DiscoveryMainMenu.generated.h"

/**
 * 
 */
UCLASS()
class NODECRAFTDISCOVERY_API UDiscoveryMainMenu : public UCommonActivatableWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UServerList* PrivateServers;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UServerList* RecommendedServers;
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UServerList* FavoriteServers;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UServerList* PopularServers;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UCommonActivatableWidgetStack* MenuStack;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UCommonActivatableWidgetStack* PopupMenuStack;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TSubclassOf<UCommonActivatableWidget> ServerDetailsClass;

public:
	virtual void NativeConstruct() override;

	UServerList* GetFavoriteServersList() const;
};
