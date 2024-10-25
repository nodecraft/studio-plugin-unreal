// Nodecraft, Inc. © 2012-2024, All Rights Reserved.


#include "UI/ServerDetails/TabbedSections/ServerDetailsModerationConsole.h"

#include "API/NodecraftMessageCodes.h"
#include "Services/ModerationService.h"
#include "Stores/ModerationStore.h"
#include "Stores/ServersStore.h"
#include "Subsystems/MessageRouterSubsystem.h"
#include "UI/Common/NodecraftLoadGuard.h"

#define MSG_ROUTE_ID "ServerDetailsModConsole"

DEFINE_LOG_CATEGORY_STATIC(LogServerDetailsModerationConsole, All, All);

void UServerDetailsModerationConsole::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	UMessageRouterSubsystem::Get().AddMessageReceiver(MSG_ROUTE_ID,
		{ REPUTATION_WILDCARD, PLAYER_ACCESS_TOKEN_WILDCARD, DISCOVERY_SERVER_WILDCARD, GENERIC_WILDCARD_FAILURE},
		FOnRoutedMessageReceived::CreateWeakLambda(this, [this](const FString& ErrorIdentifier, const FText& ErrorMessage, EAlertType AlertType)
	{
			// We're only interested in errors
			if (IsActivated() && AlertType == EAlertType::Error)
			{
				AlertMessage->Show(ErrorMessage, AlertType);
			}
	}));

	AlertMessage->Hide();

	PlayerSelector->OnSelectedPlayersChanged.AddWeakLambda(this, [this](const TArray<UPlayerServerDetailsDataObject*>& SelectedPlayers)
	{
		OnSelectedPlayersChanged.Broadcast(SelectedPlayers);
		UE_LOG(LogServerDetailsModerationConsole, Display, TEXT("PlayerSelector OnSelectedPlayersChanged"));
	});

	FiltersTabList->SetLinkedSwitcher(TabContentSwitcher);
	SelectFilterTab("All");

	PlayerSelector->OnFocusSetToPlayer.BindWeakLambda(this, [this](UWidget* PlayerWidget)
	{
		OnPlayerSelected(PlayerWidget);
		UE_LOG(LogServerDetailsModerationConsole, Display, TEXT("PlayerSelector OnFocusSetToPlayer"));
	});
}

void UServerDetailsModerationConsole::NativeConstruct()
{
	Super::NativeConstruct();
	if (const UWorld* World = GetWorld())
	{
		UModerationStore& ModerationStore = *UModerationStore::Get(World);
		ModeratorsChangedListenerHandle = ModerationStore.AddModeratorsChangedListener(FOnModeratorsUpdated::FDelegate::CreateWeakLambda(this, [this](const TArray<UPlayerServerDetailsDataObject*>& Moderators)
		{
			PlayerSelector->SetModerators(Moderators);
		}));
		OnlinePlayersChangedListenerHandle = ModerationStore.AddOnlinePlayersChangedListener(FOnOnlinePlayersUpdated::FDelegate::CreateWeakLambda(this, [this](const TArray<UPlayerServerDetailsDataObject*>& OnlinePlayers)
		{
			PlayerSelector->SetOnlinePlayers(OnlinePlayers);
		}));
		OfflinePlayersChangedListenerHandle = ModerationStore.AddOfflinePlayersChangedListener(FOnOfflinePlayersUpdated::FDelegate::CreateWeakLambda(this, [this](const TArray<UPlayerServerDetailsDataObject*>& OfflinePlayers)
		{
			PlayerSelector->SetOfflinePlayers(OfflinePlayers);
		}));
		BannedPlayersChangedListenerHandle = ModerationStore.AddBannedPlayersChangedListener(FOnBannedPlayersUpdated::FDelegate::CreateWeakLambda(this, [this](const TArray<UPlayerServerDetailsDataObject*>& BannedPlayers)
		{
			PlayerSelector->SetBannedPlayers(BannedPlayers);
		}));
		OwnerChangedListenerHandle = ModerationStore.AddOwnerChangedListener(FOnOwnerUpdated::FDelegate::CreateWeakLambda(this, [this](UPlayerServerDetailsDataObject* Owner)
		{
			PlayerSelector->SetOwner(Owner);
		}));
	}
	else
	{
		UE_LOG(LogServerDetailsModerationConsole, Error, TEXT("UServerDetailsModerationConsole::NativeConstruct: Failed to get world"));
	}

	
}

void UServerDetailsModerationConsole::NativeDestruct()
{
	if (const UWorld* World = GetWorld())
	{
		UModerationStore& ModerationStore = *UModerationStore::Get(World);
		ModerationStore.RemoveModeratorsChangedListener(ModeratorsChangedListenerHandle);
		ModerationStore.RemoveOnlinePlayersChangedListener(OnlinePlayersChangedListenerHandle);
		ModerationStore.RemoveOfflinePlayersChangedListener(OfflinePlayersChangedListenerHandle);
		ModerationStore.RemoveBannedPlayersChangedListener(BannedPlayersChangedListenerHandle);
		ModerationStore.RemoveOwnerChangedListener(OwnerChangedListenerHandle);
	}
	else
	{
		UE_LOG(LogServerDetailsModerationConsole, Error, TEXT("UServerDetailsModerationConsole::NativeDestruct: Failed to get world"));
	}
	Super::NativeDestruct();
}

void UServerDetailsModerationConsole::NativeOnActivated()
{
	Super::NativeOnActivated();
	AlertMessage->Hide();
	FiltersTabList->OnTabSelected.AddUniqueDynamic(this, &ThisClass::SelectFilterTab);
	PlayerSelector->ActivateWidget();
}

void UServerDetailsModerationConsole::NativeOnDeactivated()
{
	PlayerSelector->ClearSelection();
	FiltersTabList->OnTabSelected.RemoveAll(this);
	RequestQueue.Get()->CancelRequeusts();
	Super::NativeOnDeactivated();
}

void UServerDetailsModerationConsole::LoadData(const FString& InServerId)
{
	ServerId = InServerId;
	ReloadData();
}

void UServerDetailsModerationConsole::ReloadData()
{
	LoadGuard->SetIsLoading(true);

	ModerationHistorySection->SetServerId(ServerId);
	ModerationHistorySection->LoadData();
	
	UModerationService &ModerationService = UModerationService::Get();
	if (const UWorld* World = GetWorld())
	{
		if (RequestQueue)
		{
			RequestQueue->CancelRequeusts();
		}
		RequestQueue = MakeShared<FHTTPRequestQueue>();
		TArray<TSharedRef<IHttpRequest>> Requests;
		Requests.Add(ModerationService.GetModeratorsRequest(World, ServerId));
		Requests.Add(ModerationService.GetOnlinePlayersRequest(World, ServerId));
		Requests.Add(ModerationService.GetOfflinePlayersRequest(World, ServerId));
		Requests.Add(ModerationService.GetBannedPlayersRequest(World, ServerId));
		Requests.Add(ModerationService.GetOwnerRequest(World, ServerId));
		RequestQueue.Get()->RunQueue(this, Requests, FAllRequestsFinishedDelegate::CreateWeakLambda(
			this, [this](bool bSuccess)
		{
			if (bSuccess && UServersStore::Get().GetCurrentServerId() == ServerId)
			{
				LoadGuard->SetIsLoading(false);
				PlayerSelector->SetFocusOnFirstPlayer();
			}
		}));
	}
}

void UServerDetailsModerationConsole::SetupNavigation(const FGetFocusDestination& InDelegate)
{
	PlayerSelector->SetupNavigation(InDelegate);
}

UWidget* UServerDetailsModerationConsole::GetFirstFocusablePlayerWidget()
{
	return PlayerSelector->GetFirstFocusablePlayerWidget();
}

void UServerDetailsModerationConsole::ClearPlayerSelection()
{
	PlayerSelector->ClearSelection();
}

void UServerDetailsModerationConsole::SelectFilterTab(FName TabId)
{
	// Clearing here works but only because we happen to be doing so in the PlayerSelector.
	// TODO: We really ought to actually have the VM layer determine if an item is checked or not, but we don't have time to do that right now.
	ClearPlayerSelection();
	if (TabId == "All")
	{
		TabContentSwitcher->SetActiveWidgetIndex(0);
		PlayerSelector->SetDisplayMode(EPlayerSelectorMode::All);
		UE_LOG(LogServerDetailsModerationConsole, Display, TEXT("AllButton clicked"));
	}
	else if (TabId == "Staff")
	{
		TabContentSwitcher->SetActiveWidgetIndex(0);
		PlayerSelector->SetDisplayMode(EPlayerSelectorMode::Staff);
		UE_LOG(LogServerDetailsModerationConsole, Display, TEXT("StaffButton clicked"));
	}
	else if (TabId == "Players")
	{
		TabContentSwitcher->SetActiveWidgetIndex(0);
		PlayerSelector->SetDisplayMode(EPlayerSelectorMode::Players);
		UE_LOG(LogServerDetailsModerationConsole, Display, TEXT("PlayersButton clicked"));
	}
	else if (TabId == "Bans")
	{
		TabContentSwitcher->SetActiveWidgetIndex(0);
		PlayerSelector->SetDisplayMode(EPlayerSelectorMode::Banned);
		UE_LOG(LogServerDetailsModerationConsole, Display, TEXT("BannedButton clicked"));
	}
	else if (TabId == "Log")
	{
		TabContentSwitcher->SetActiveWidgetIndex(1);
		UE_LOG(LogServerDetailsModerationConsole, Display, TEXT("LogButton clicked"));
	}
}

void UServerDetailsModerationConsole::OnPlayerSelected(UWidget* PlayerWidget)
{
	ScrollBox->ScrollWidgetIntoView(PlayerWidget);
}
