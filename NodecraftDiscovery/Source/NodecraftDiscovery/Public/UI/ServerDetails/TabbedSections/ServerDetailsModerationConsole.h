// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "CommonActivatableWidgetSwitcher.h"
#include "ServerDetailsModerationHistorySection.h"
#include "Common/HTTP/HTTPRequestQueue.h"
#include "UI/Foundation/NodecraftButtonBase.h"
#include "UI/ServerDetails/ModerationConsolePlayerSelector.h"
#include "ServerDetailsModerationConsole.generated.h"

UCLASS()
class NODECRAFTDISCOVERY_API UServerDetailsModerationConsole : public UCommonActivatableWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UServerDetailsModerationHistorySection* ModerationHistorySection;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UModerationConsolePlayerSelector* PlayerSelector;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UCommonActivatableWidgetSwitcher* TabContentSwitcher;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UNodecraftButtonBase* AllButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UNodecraftButtonBase* StaffButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UNodecraftButtonBase* PlayersButton;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UNodecraftButtonBase* BannedButton;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UNodecraftButtonBase* LogButton;
	
	TSharedPtr<FHTTPRequestQueue> RequestQueue;

public:
	virtual void NativeOnInitialized() override;
	void SelectButton(UNodecraftButtonBase* Button);
	void LoadData(const FString& InServerId);

	FOnSelectedPlayersChanged OnSelectedPlayersChanged;
};
