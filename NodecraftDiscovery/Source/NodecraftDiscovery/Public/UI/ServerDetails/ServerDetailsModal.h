// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "CommonActivatableWidgetSwitcher.h"
#include "ModerationConsolePlayerDetailsPanel.h"
#include "ServerDetailsHeaderCard.h"
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
	// Community Server
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UPanelWidget* CommunityServerSection;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UNodecraftTabListWidgetBase* CommunityServerTabList;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UCommonActivatableWidgetSwitcher* CommunityServerTabContentSwitcher;

	// END Community Server
	
	// Private Server
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UPanelWidget* PrivateServerSection;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UNodecraftTabListWidgetBase* PrivateServerTabList;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UCommonActivatableWidgetSwitcher* PrivateServerTabContentSwitcher;
	// END Private Server

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
	UCommonButtonBase* FavoriteButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UServerDetailsHeaderCard* HeaderCard;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UAlertMessage* AlertMessage;

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
	
public:
	void OnGetServerDetailsComplete(UServerDataObject* ServerDataObject, bool bSuccess, TOptional<FText> Error);
	void LoadServerDetails(UServerDataObject* ServerDataObject);

	UFUNCTION()
	void OnTabContentCreated(FName TabId, UCommonUserWidget* TabWidget);

	UFUNCTION()
	void OnPrivateTabSelected(FName TabId);

	virtual void NativeOnInitialized() override;
	virtual void NativeOnActivated() override;
};
