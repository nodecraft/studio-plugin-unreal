// Nodecraft, Inc. © 2012-2024, All Rights Reserved.


#include "UI/ServerDetails/ServerDetailsModal.h"

#include "CommonBorder.h"
#include "API/NodecraftStudioSessionManagerSubsystem.h"
#include "Services/ServersService.h"
#include "Subsystems/MenuManagerSubsystem.h"
#include "UI/Alerts/AlertMessage.h"

void UServerDetailsModal::ToggleIsFavorite()
{
	if (FavoriteButton->GetIsLoading())
	{
		return;
	}
	
	if (ServerData)
	{
		UServersService& ServersService = UServersService::Get();
		FSimpleServiceResponseDelegate SimpleServiceResponseDelegate;
		SimpleServiceResponseDelegate.BindWeakLambda(this, [this](bool bSuccess, TOptional<FText> Error)
		{
			FavoriteButton->SetIsLoading(false);
			if (bSuccess)
			{
				ServerData->SetIsFavorite(!ServerData->IsFavorite());
				FavoriteButton->SetIsFavorited(ServerData->IsFavorite());

				// todo: UE-253 update server lists in main menu with newly (un)favorited server cards
			}
		});
		
		FavoriteButton->SetIsLoading(true);
		if (ServerData->IsFavorite())
		{
			ServersService.UnfavoriteServer(ServerData, SimpleServiceResponseDelegate);
		}
		else
		{
			ServersService.FavoriteServer(ServerData, SimpleServiceResponseDelegate);
		}
	}
}

FString UServerDetailsModal::AdjustTabsVisibility(const UServerDataObject* ServerDataObject)
{
	/*
	Players (Allows) - shows if the server is private (subject to change pending discussion)
	Overview - shows if the server is a Community server
	Rules - shows if the server is a Community server
	Moderation (log) - shows if the player is just a normal player
	Moderation (console) - shows if the player is the owner or a moderator
	About - Shows if the server is a Community server
	Settings - shows if the player is the owner
	*/

	
	const bool bShowOverview = ServerDataObject->GetType() == EServerType::Community;
	const bool bShowPlayerAllows = ServerDataObject->GetType() == EServerType::Private;
	const bool bShowRules = ServerDataObject->GetType() == EServerType::Community;
	const bool bShowModerationLog = ServerDataObject->GetRole() == EPlayerRole::Player;
	const bool bShowModerationConsole = ServerDataObject->GetRole() == EPlayerRole::Owner || ServerDataObject->GetRole() == EPlayerRole::Moderator;
	const bool bShowAbout = ServerDataObject->GetType() == EServerType::Community;
	const bool bShowSettings = ServerDataObject->GetRole() == EPlayerRole::Owner;

	TabsList->SetTabVisibility("PlayerTab", bShowPlayerAllows ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	TabsList->SetTabVisibility("OverviewTab", bShowOverview ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	TabsList->SetTabVisibility("RulesTab", bShowRules ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	TabsList->SetTabVisibility("ModerationLogTab", bShowModerationLog ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	TabsList->SetTabVisibility("ModerationConsoleTab", bShowModerationConsole ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	TabsList->SetTabVisibility("AboutTab", bShowAbout ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	TabsList->SetTabVisibility("SettingsTab", bShowSettings ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);

	// First visible tab is either going to be the player tab or the overview tab
	FString FirstVisibleTabId = "PlayerTab";
	if (bShowOverview)
	{
		FirstVisibleTabId = "OverviewTab";
	}

	return FirstVisibleTabId;
}

void UServerDetailsModal::OnGetServerDetailsComplete(UServerDataObject* ServerDataObject, bool bSuccess, TOptional<FText> Error)
{
	if (bSuccess)
	{		
		ServerData = ServerDataObject;
		HeaderCard->SetServerData(ServerData, false);
		OverviewSection->SetServerData(ServerData);
		// We are now expecting to have rules
		RulesSection->SetServerData(ServerData);
		ModerationHistorySection->SetServerData(ServerData);
		AboutSection->SetServerData(ServerData);
		AllowedPlayers->SetServerData(ServerData);

		// sets the style and state of "star" favorite button to (un)selected
		FavoriteButton->SetIsLoading(false);
		FavoriteButton->SetIsFavorited(ServerData->IsFavorite());

		PasswordRequiredPill->SetVisibility(ServerData->HasPassword() ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);

		AdjustTabsVisibility(ServerDataObject);

		JoinButton->OnClicked().Clear();
		JoinButton->OnClicked().AddWeakLambda(this, [this, ServerDataObject]
		{
			UServersService::Get().JoinServer(ServerDataObject, GetWorld());
		});
	}
}

void UServerDetailsModal::LoadServerDetails(UServerDataObject* ServerDataObject)
{
	ServerData = ServerDataObject;

	// We were previously setting server data in OnTabContentCreated
	// but ServerData will be stale at that point
	FavoriteButton->SetIsLoading(true);
	HeaderCard->SetServerData(ServerData, true);
	OverviewSection->SetServerData(ServerData);
	// We're not expecting to have rules at this point, but in case API changes in the future, check anyway
	if (ServerData->GetRules())
	{
		RulesSection->SetServerData(ServerData);
	}
	ModerationHistorySection->SetServerData(ServerData);
	AboutSection->SetServerData(ServerData);
	AllowedPlayers->SetServerData(ServerData);
	OwnerSettings->SetServerData(ServerData);

	JoinButton->OnClicked().Clear();
	JoinButton->OnClicked().AddWeakLambda(this, [this, ServerDataObject]
	{
		UServersService::Get().JoinServer(ServerDataObject, GetWorld());
	});
	
	const FString ServerID = ServerDataObject->GetId();
	PlayerDetailsPanel->SetServerId(ServerID);
	ModerationHistorySection->SetServerId(ServerID);
	// ModerationSection->LoadData();
	AboutSection->SetServerId(ServerID);

	OverviewSection->SetLoading(true);
	RulesSection->SetLoading(true);
	
	// Get ServersService
	UServersService& ServersService = UServersService::Get();
	// Call GetServerDetails
	FGetServerDetailsDelegate GetServerDetailsDelegate;
	GetServerDetailsDelegate.BindUObject(this, &ThisClass::OnGetServerDetailsComplete);
	ServersService.GetServerDetails(GetServerDetailsDelegate, ServerID);
	
	OnlinePlayersList->LoadData(EPlayerListType::Online, ServerDataObject);
	RecentPlayersList->LoadData(EPlayerListType::Recent, ServerDataObject);
	PlayerDetailsPanelContainer->SetVisibility(ESlateVisibility::Collapsed);

	FString FirstVisibleTabId =	AdjustTabsVisibility(ServerDataObject);
	// Navigate to the first tab that is visible
	TabsList->SelectTabByID(FName(*FirstVisibleTabId));
}

void UServerDetailsModal::OnTabContentCreated(FName TabId, UCommonUserWidget* TabWidget)
{
	if (UServerDetailsRulesSection* Rules = Cast<UServerDetailsRulesSection>(TabWidget))
	{
		RulesSection = Rules;
	}
	else if (UServerDetailsAboutSection* About = Cast<UServerDetailsAboutSection>(TabWidget))
	{
		AboutSection = About;
	}
	else if (UServerDetailsModerationHistorySection* Moderation = Cast<UServerDetailsModerationHistorySection>(TabWidget))
	{
		ModerationHistorySection = Moderation;
	}
	else if (UServerDetailsOverviewSection* Overview = Cast<UServerDetailsOverviewSection>(TabWidget))
	{
		OverviewSection = Overview;
	}
	else if (UServerDetailsModerationConsole* ModConsole = Cast<UServerDetailsModerationConsole>(TabWidget))
	{
		ModerationConsole = ModConsole;
	}
	else if (UServerDetailsAllowedPlayersSection* Players = Cast<UServerDetailsAllowedPlayersSection>(TabWidget))
	{
		AllowedPlayers = Players;
	}
	else if (UServerDetailsOwnerSettingsSection* Settings = Cast<UServerDetailsOwnerSettingsSection>(TabWidget))
	{
		OwnerSettings = Settings;
	}
}

void UServerDetailsModal::OnSelectedTab(FName TabId)
{
	if (TabId == "ModerationConsoleTab")
	{
		OnlinePlayersList->SetVisibility(ESlateVisibility::Collapsed);
		RecentPlayersList->SetVisibility(ESlateVisibility::Collapsed);
		PlayerDetailsPanelContainer->SetVisibility(ESlateVisibility::Visible);
		if (ModerationConsole)
		{
			ModerationConsole->LoadData(ServerData->GetId());
		}
	}
	else
	{
		OnlinePlayersList->SetVisibility(ESlateVisibility::Visible);
		RecentPlayersList->SetVisibility(ESlateVisibility::Visible);
		PlayerDetailsPanelContainer->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UServerDetailsModal::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	TabsList->SetLinkedSwitcher(ServerTabContentSwitcher);
	TabsList->OnTabContentCreated.AddDynamic(this, &ThisClass::OnTabContentCreated);
	TabsList->OnTabSelected.AddDynamic(this, &ThisClass::OnSelectedTab);
	
	FavoriteButton->OnClicked().AddUObject(this, &ThisClass::ToggleIsFavorite);
}

void UServerDetailsModal::NativeOnActivated()
{
	Super::NativeOnActivated();
	AlertMessage->Hide();

	UMenuManagerSubsystem::Get().OnServerQueueError.Unbind();
	UMenuManagerSubsystem::Get().OnServerQueueError.BindWeakLambda(this, [this](const FText& ErrorText)
	{
		AlertMessage->Show(ErrorText, EAlertType::Error);
	});

	ModerationConsole->OnSelectedPlayersChanged.AddWeakLambda(this, [this](const TArray<UPlayerServerDetailsDataObject*>& SelectedPlayers)
	{
		PlayerDetailsPanel->SetSelectedPlayers(SelectedPlayers);
	});

	PlayerDetailsPanel->OnCompletedModerationActionDelegate.AddWeakLambda(this, [this]()
	{
		ModerationConsole->ClearPlayerSelection();
		ModerationConsole->ReloadData();
	});
}

void UServerDetailsModal::SetCloseDelegate(FSimpleDelegate InOnCloseDelegate)
{
	CloseButton->OnClicked().Clear();
	CloseButton->OnClicked().Add(InOnCloseDelegate);
}

void UServerDetailsModal::NavigateToRulesTabWithError(const FText& ErrorText)
{
	TabsList->SelectTabByID("RulesTab");
	AlertMessage->Show(ErrorText, EAlertType::Error);
}
