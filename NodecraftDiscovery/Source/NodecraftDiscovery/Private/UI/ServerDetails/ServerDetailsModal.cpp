// Nodecraft, Inc. © 2012-2024, All Rights Reserved.


#include "UI/ServerDetails/ServerDetailsModal.h"

#include "CommonBorder.h"
#include "API/NodecraftStudioSessionManagerSubsystem.h"
#include "Input/CommonUIActionRouterBase.h"
#include "Input/CommonUIInputTypes.h"
#include "Services/ServerQueueService.h"
#include "Services/ServersService.h"
#include "Stores/ServersStore.h"
#include "UI/Alerts/AlertMessage.h"

bool UServerDetailsModal::NativeOnHandleBackAction()
{
	// TODO: If any other tabs need to handle back action in the future, we should generalize this
	if (AllowedPlayers->ShouldProcessBackAction())
	{
		return AllowedPlayers->ProcessBackAction() ? true : Super::NativeOnHandleBackAction();
	}
	return Super::NativeOnHandleBackAction();
}

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
			if (bSuccess && UServersStore::Get().GetCurrentServerId() == ServerData->GetId())
			{
				ServerData->SetIsFavorite(!ServerData->IsFavorite());
				FavoriteButton->SetIsFavorited(ServerData->IsFavorite());
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
		PlayerDetailsPanel->SetPlayerRole(ServerDataObject->GetRole());

		if (LaunchArguments.IsSet())
		{
			if (LaunchArguments.GetValue().Contains("RulesError"))
			{
				NavigateToRulesTabWithError(LaunchArguments.GetValue().FindChecked("RulesError"));
			}
			else if (LaunchArguments.GetValue().Contains("Tags"))
			{
				NavigateToAboutTab();
			}
		}

		JoinButton->OnClicked().Clear();
		JoinButton->OnClicked().AddWeakLambda(this, [this, ServerDataObject]
		{
			JoinButton->SetIsLoading(true);
			UServersService::Get().JoinServer(ServerDataObject, GetWorld());
		});
	}
}

void UServerDetailsModal::LoadServerDetails(UServerDataObject* ServerDataObject, TOptional<FServerDetailsArgument> Argument)
{
	ServerData = ServerDataObject;

	LaunchArguments = Argument.IsSet() ? Argument.GetValue() : TOptional<FServerDetailsArgument>();

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
		JoinButton->SetIsLoading(true);
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
	
	OnlinePlayersList->LoadData(EPlayerListType::Online, ServerDataObject, FSimpleDelegate::CreateWeakLambda(this, [this]()
	{
		bHasLoadedRecentPlayersData = true;
		TryToActivatePlayerListScrolling();
	}));

	RecentPlayersList->LoadData(EPlayerListType::Recent, ServerDataObject, FSimpleDelegate::CreateWeakLambda(this, [this]()
	{
		bHasLoadedOnlinePlayersData = true;
		TryToActivatePlayerListScrolling();
	}));
	
	PlayerDetailsPanelContainer->SetVisibility(ESlateVisibility::Collapsed);

	FString FirstVisibleTabId =	AdjustTabsVisibility(ServerDataObject);
	// Navigate to the first tab that is visible
	TabsList->SelectTabByID(FName(*FirstVisibleTabId));
}

void UServerDetailsModal::TryToActivatePlayerListScrolling()
{
	if (bHasLoadedRecentPlayersData && bHasLoadedOnlinePlayersData)
	{
		
		if (RecentPlayersList->HasItems())
		{
			RecentPlayersList->SetListeningForScrollInput(true);
			OnlinePlayersList->SetListeningForScrollInput(false);
		}
		else if (OnlinePlayersList->HasItems())
		{
			OnlinePlayersList->SetListeningForScrollInput(true);
			RecentPlayersList->SetListeningForScrollInput(false);
		}

		// If both lists have items, activate our action that lets us switch between lists
		if (RecentPlayersList->HasItems() && OnlinePlayersList->HasItems())
		{
			if (UCommonUIActionRouterBase* ActionRouter = UCommonUIActionRouterBase::Get(*this))
			{
				TogglePlayerListActionHandle = ActionRouter->RegisterUIActionBinding(*this, FBindUIActionArgs(ToggleActivePlayerListInputActionData, true, FSimpleDelegate::CreateWeakLambda(this, [this]()
				{
					if (OnlinePlayersList->IsListeningForScrollInput())
					{
						OnlinePlayersList->SetListeningForScrollInput(false);
						RecentPlayersList->SetListeningForScrollInput(true);
					}
					else
					{
						OnlinePlayersList->SetListeningForScrollInput(true);
						RecentPlayersList->SetListeningForScrollInput(false);
					}
				})));
			}
		}
	}
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
		StopListeningForScrollingInput();
		PlayerDetailsPanelContainer->SetVisibility(ESlateVisibility::Visible);
		if (ModerationConsole)
		{
			ModerationConsole->LoadData(ServerData->GetId());
		}
	}
	else
	{
		TryToActivatePlayerListScrolling();
		OnlinePlayersList->SetVisibility(ESlateVisibility::Visible);
		RecentPlayersList->SetVisibility(ESlateVisibility::Visible);
		PlayerDetailsPanelContainer->SetVisibility(ESlateVisibility::Collapsed);
	}
	JoinButton->SetIsLoading(false);
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
	JoinButton->SetIsLoading(false);
	
	OnSelectedPlayersChanged = ModerationConsole->OnSelectedPlayersChanged.AddWeakLambda(this, [this](const TArray<UPlayerServerDetailsDataObject*>& SelectedPlayers)
	{
		PlayerDetailsPanel->SetSelectedPlayers(SelectedPlayers);
	});

	GetFocusTargetForNavFromPlayersList.BindWeakLambda(this, [this]() -> UWidget*
	{
		return PlayerDetailsPanel->GetDesiredFocusTarget();
	});
	
	ModerationConsole->SetupNavigation(GetFocusTargetForNavFromPlayersList);

	OnCompletedModerationAction = PlayerDetailsPanel->OnCompletedModerationActionDelegate.AddWeakLambda(this, [this]()
	{
		ModerationConsole->ClearPlayerSelection();
		ModerationConsole->ReloadData();
	});

	PlayerDetailsPanel->GetPlayerListDelegate.BindWeakLambda(this, [this]() -> UWidget*
	{
		return ModerationConsole->GetFirstFocusablePlayerWidget();
	});
}

void UServerDetailsModal::StopListeningForScrollingInput()
{
	OnlinePlayersList->SetListeningForScrollInput(false);
	RecentPlayersList->SetListeningForScrollInput(false);
}

void UServerDetailsModal::NativeOnDeactivated()
{
	StopListeningForScrollingInput();
	bHasLoadedOnlinePlayersData = false;
	bHasLoadedRecentPlayersData = false;
	
	TogglePlayerListActionHandle.Unregister();

	ModerationConsole->OnSelectedPlayersChanged.Remove(OnSelectedPlayersChanged);

	GetFocusTargetForNavFromPlayersList.Unbind();
	
	PlayerDetailsPanel->OnCompletedModerationActionDelegate.Remove(OnCompletedModerationAction);

	PlayerDetailsPanel->GetPlayerListDelegate.Unbind();
	
	Super::NativeOnDeactivated();
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
	
	// ideally, we want to do update the join button OnTabSelected (which we do),
	// but it doesn't fire when we select the tab we're already on.
	JoinButton->SetIsLoading(false);
}

void UServerDetailsModal::NavigateToAboutTab()
{
	TabsList->SelectTabByID("AboutTab");
	// TODO: Wait for the About Tab to be active, then navigate to the tags at the bottom of the About Tab
}