// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "CommonActivatableWidgetSwitcher.h"
#include "ModerationConsolePlayerDetailsPanel.h"
#include "ServerDetailsHeaderCard.h"
#include "ToggleFavoriteButton.h"
#include "TabbedSections/ServerDetailsAboutSection.h"
#include "TabbedSections/ServerDetailsAllowedPlayersSection.h"
#include "TabbedSections/ServerDetailsModerationConsole.h"
#include "TabbedSections/ServerDetailsModerationHistorySection.h"
#include "TabbedSections/ServerDetailsOverviewSection.h"
#include "TabbedSections/ServerDetailsOwnerSettingsSection.h"
#include "TabbedSections/ServerDetailsRulesSection.h"
#include "UI/Common/NodecraftTabListWidgetBase.h"
#include "UI/Foundation/NodecraftLoadingButton.h"
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
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UPanelWidget* CommunityServerSection;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UNodecraftTabListWidgetBase* TabsList;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UCommonActivatableWidgetSwitcher* ServerTabContentSwitcher;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UPlayerList* OnlinePlayersList;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UPlayerList* RecentPlayersList;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UPanelWidget* PlayerDetailsPanelContainer;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UModerationConsolePlayerDetailsPanel* PlayerDetailsPanel;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UCommonButtonBase* CloseButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UCommonButtonBase* JoinButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UToggleFavoriteButton* FavoriteButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UServerDetailsHeaderCard* HeaderCard;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UAlertMessage* AlertMessage;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UCommonBorder* PasswordRequiredPill;

	// Community server sections
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UServerDetailsRulesSection* RulesSection;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UServerDetailsAboutSection* AboutSection;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UServerDetailsModerationHistorySection* ModerationHistorySection;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UServerDetailsOverviewSection* OverviewSection;
	// END Community server sections

	// Private server sections
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UServerDetailsModerationConsole* ModerationConsole;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UServerDetailsAllowedPlayersSection* AllowedPlayers;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UServerDetailsOwnerSettingsSection* OwnerSettings;
	// END Private server sections
	
private:
	UPROPERTY()
	UServerDataObject* ServerData;

	void ToggleIsFavorite();

	// Returns the tab id of the first visible tab
	FString AdjustTabsVisibility(const UServerDataObject* ServerDataObject);

public:
	void OnGetServerDetailsComplete(UServerDataObject* ServerDataObject, bool bSuccess, TOptional<FText> Error);
	void LoadServerDetails(UServerDataObject* ServerDataObject);

	UFUNCTION()
	void OnTabContentCreated(FName TabId, UCommonUserWidget* TabWidget);

	UFUNCTION()
	void OnSelectedTab(FName TabId);

	virtual void NativeOnInitialized() override;
	virtual void NativeOnActivated() override;
	
	void SetCloseDelegate(FSimpleDelegate InOnCloseDelegate);
	
	void NavigateToRulesTabWithError(const FText& ErrorText);
};
