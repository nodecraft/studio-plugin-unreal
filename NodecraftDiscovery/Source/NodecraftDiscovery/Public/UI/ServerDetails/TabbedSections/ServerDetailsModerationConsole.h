// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "CommonActivatableWidgetSwitcher.h"
#include "ServerDetailsModerationHistorySection.h"
#include "Common/HTTP/HTTPRequestQueue.h"
#include "UI/Alerts/AlertMessage.h"
#include "UI/Common/NodecraftScrollBox.h"
#include "UI/Common/NodecraftTabListWidgetBase.h"
#include "UI/ServerDetails/ModerationConsolePlayerSelector.h"
#include "ServerDetailsModerationConsole.generated.h"

UCLASS()
class NODECRAFTDISCOVERY_API UServerDetailsModerationConsole : public UCommonActivatableWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Details", meta = (BindWidget))
	UNodecraftLoadGuard* LoadGuard;
	
	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Details", meta = (BindWidget))
	UServerDetailsModerationHistorySection* ModerationHistorySection;
	
	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Details", meta = (BindWidget))
	UModerationConsolePlayerSelector* PlayerSelector;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Details", meta = (BindWidget))
	UCommonActivatableWidgetSwitcher* TabContentSwitcher;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Details", meta = (BindWidget))
	UAlertMessage* AlertMessage;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Details", meta = (BindWidget))
	UNodecraftTabListWidgetBase* FiltersTabList;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Details", meta = (BindWidget))
	UNodecraftScrollBox* ScrollBox;
	
	TSharedPtr<FHTTPRequestQueue> RequestQueue;

	FDelegateHandle ModeratorsChangedListenerHandle;
	FDelegateHandle OnlinePlayersChangedListenerHandle;
	FDelegateHandle OfflinePlayersChangedListenerHandle;
	FDelegateHandle BannedPlayersChangedListenerHandle;
	FDelegateHandle OwnerChangedListenerHandle;
	
	FString ServerId;

public:
	UFUNCTION()
	void SelectFilterTab(FName TabId);

	UFUNCTION()
	void OnPlayerSelected(UWidget* PlayerWidget);

	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeOnActivated() override;
	virtual void NativeOnDeactivated() override;
	void LoadData(const FString& InServerId);
	void ClearPlayerSelection();
	void ReloadData();
	void SetupNavigation(const FGetFocusDestination& Delegate);
	
	UWidget* GetFirstFocusablePlayerWidget();

	FOnSelectedPlayersChanged OnSelectedPlayersChanged;
};
