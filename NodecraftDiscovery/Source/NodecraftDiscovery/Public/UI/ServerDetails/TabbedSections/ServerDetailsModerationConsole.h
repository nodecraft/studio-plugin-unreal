// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "CommonActivatableWidgetSwitcher.h"
#include "ServerDetailsModerationHistorySection.h"
#include "Common/HTTP/HTTPRequestQueue.h"
#include "UI/Alerts/AlertMessage.h"
#include "UI/Common/NodecraftLoadGuard.h"
#include "UI/Foundation/NodecraftButtonBase.h"
#include "UI/ServerDetails/ModerationConsolePlayerSelector.h"
#include "ServerDetailsModerationConsole.generated.h"

UCLASS()
class NODECRAFTDISCOVERY_API UServerDetailsModerationConsole : public UCommonActivatableWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UNodecraftLoadGuard* LoadGuard;
	
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

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UAlertMessage* AlertMessage;
	
	TSharedPtr<FHTTPRequestQueue> RequestQueue;

	FDelegateHandle ModeratorsChangedListenerHandle;
	FDelegateHandle OnlinePlayersChangedListenerHandle;
	FDelegateHandle OfflinePlayersChangedListenerHandle;
	FDelegateHandle BannedPlayersChangedListenerHandle;
	FDelegateHandle OwnerChangedListenerHandle;
	
	FString ServerId;

public:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeOnActivated() override;
	void SelectButton(UNodecraftButtonBase* Button);
	void LoadData(const FString& InServerId);
	void ClearPlayerSelection();
	void ReloadData();

	FOnSelectedPlayersChanged OnSelectedPlayersChanged;
};
