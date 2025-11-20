// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "CommonActivatableWidgetSwitcher.h"
#include "ModerationConsolePlayerDetailsPanel.h"
#include "ServerDetailsHeaderCard.h"
#include "ToggleFavoriteButton.h"
#include "Subsystems/MenuManagerSubsystem.h"
#include "TabbedSections/ServerDetailsAboutSection.h"
#include "TabbedSections/ServerDetailsAllowedPlayersSection.h"
#include "TabbedSections/ServerDetailsModerationConsole.h"
#include "TabbedSections/ServerDetailsModerationHistorySection.h"
#include "TabbedSections/ServerDetailsOverviewSection.h"
#include "TabbedSections/ServerDetailsOwnerSettingsSection.h"
#include "TabbedSections/ServerDetailsRulesSection.h"
#include "UI/Common/NodecraftTabListWidgetBase.h"
#include "UI/PlayerList/PlayerList.h"
#include "UObject/Object.h"
#include "ServerDetailsModal.generated.h"

class UAlertMessage;
class UServerQueueTokenDataObject;
/**
 * 
 */
UCLASS()
class NODECRAFTDISCOVERY_API UServerDetailsModal : public UCommonActivatableWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeOnActivated() override;
	virtual void NativeOnDeactivated() override;
	virtual bool NativeOnHandleBackAction() override;
	
	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Details", meta = (BindWidget))
	UPanelWidget* CommunityServerSection;
	
	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Details", meta = (BindWidget))
	UNodecraftTabListWidgetBase* TabsList;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Details", meta = (BindWidget))
	UCommonActivatableWidgetSwitcher* ServerTabContentSwitcher;
	
	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Details", meta = (BindWidget))
	UPlayerList* OnlinePlayersList;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Details", meta = (BindWidget))
	UPlayerList* RecentPlayersList;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Details", meta = (BindWidget))
	UPanelWidget* PlayerDetailsPanelContainer;
	
	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Details", meta = (BindWidget))
	UModerationConsolePlayerDetailsPanel* PlayerDetailsPanel;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Details", meta = (BindWidget))
	UCommonButtonBase* CloseButton;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Details", meta = (BindWidget))
	UNodecraftLoadingButton* JoinButton;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Details", meta = (BindWidget))
	UToggleFavoriteButton* FavoriteButton;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Details", meta = (BindWidget))
	UServerDetailsHeaderCard* HeaderCard;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Details", meta = (BindWidget))
	UAlertMessage* AlertMessage;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Details", meta = (BindWidget))
	UCommonBorder* PasswordRequiredPill;

	// Community server sections
	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Details", meta = (BindWidget))
	UServerDetailsRulesSection* RulesSection;
	
	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Details", meta = (BindWidget))
	UServerDetailsAboutSection* AboutSection;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Details", meta = (BindWidget))
	UServerDetailsModerationHistorySection* ModerationHistorySection;
	
	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Details", meta = (BindWidget))
	UServerDetailsOverviewSection* OverviewSection;
	// END Community server sections

	// Private server sections
	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Details", meta = (BindWidget))
	UServerDetailsModerationConsole* ModerationConsole;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Details", meta = (BindWidget))
	UServerDetailsAllowedPlayersSection* AllowedPlayers;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Details", meta = (BindWidget))
	UServerDetailsOwnerSettingsSection* OwnerSettings;
	// END Private server sections

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, AdvancedDisplay, Category = "Nodecraft UI|Server Details|Input", meta = (RowType = "/Script/CommonUI.CommonInputActionDataBase"))
	FDataTableRowHandle ToggleActivePlayerListInputActionData;

	FGetFocusDestination GetFocusTargetForNavFromPlayersList;
	TOptional<FServerDetailsArgument> LaunchArguments;
	
	FDelegateHandle OnSelectedPlayersChanged;
	FDelegateHandle OnCompletedModerationAction;

private:
	UPROPERTY()
	UServerDataObject* ServerData;

	void ToggleIsFavorite();

	// Returns the tab id of the first visible tab
	FString AdjustTabsVisibility(const UServerDataObject* ServerDataObject);

	FUIActionBindingHandle TogglePlayerListActionHandle;

	bool bHasLoadedOnlinePlayersData = false;
	bool bHasLoadedRecentPlayersData = false;

	void TryToActivatePlayerListScrolling();

	void StopListeningForScrollingInput();

public:
	void OnGetServerDetailsComplete(UServerDataObject* ServerDataObject, bool bSuccess, TOptional<FText> Error);
	void LoadServerDetails(UServerDataObject* ServerDataObject, TOptional<FServerDetailsArgument> Argument = TOptional<TMap<FString, FText>>());

	UFUNCTION()
	void OnTabContentCreated(FName TabId, UCommonUserWidget* TabWidget);

	UFUNCTION()
	void OnSelectedTab(FName TabId);
	
	void SetCloseDelegate(FSimpleDelegate InOnCloseDelegate);
	
	void NavigateToRulesTabWithError(const FText& ErrorText);
	void NavigateToAboutTab();
};
