// Nodecraft, Inc. © 2012-2024, All Rights Reserved.


#include "UI/ServerDetails/TabbedSections/ServerDetailsModerationConsole.h"

#include "API/NodecraftMessageCodes.h"
#include "Services/ModerationService.h"
#include "Stores/ModerationStore.h"
#include "Subsystems/MessageRouterSubsystem.h"

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

	BannedButton->OnClicked().AddWeakLambda(this, [this]()
	{
		SelectButton(BannedButton);
		TabContentSwitcher->SetActiveWidgetIndex(0);
		PlayerSelector->SetDisplayMode(EPlayerSelectorMode::Banned);
		UE_LOG(LogServerDetailsModerationConsole, Display, TEXT("BannedButton clicked"));
	});

	// Do the same kind of binding as above for the rest of the buttons
	AllButton->OnClicked().AddWeakLambda(this, [this]()
	{
		SelectButton(AllButton);
		TabContentSwitcher->SetActiveWidgetIndex(0);
		PlayerSelector->SetDisplayMode(EPlayerSelectorMode::All);
		UE_LOG(LogServerDetailsModerationConsole, Display, TEXT("AllButton clicked"));
	});

	PlayersButton->OnClicked().AddWeakLambda(this, [this]()
	{
		SelectButton(PlayersButton);
		TabContentSwitcher->SetActiveWidgetIndex(0);
		PlayerSelector->SetDisplayMode(EPlayerSelectorMode::Players);
		UE_LOG(LogServerDetailsModerationConsole, Display, TEXT("PlayersButton clicked"));
	});

	StaffButton->OnClicked().AddWeakLambda(this, [this]()
	{
		SelectButton(StaffButton);
		TabContentSwitcher->SetActiveWidgetIndex(0);
		PlayerSelector->SetDisplayMode(EPlayerSelectorMode::Staff);
		UE_LOG(LogServerDetailsModerationConsole, Display, TEXT("StaffButton clicked"));
	});

	LogButton->OnClicked().AddWeakLambda(this, [this]()
	{
		SelectButton(LogButton);
		TabContentSwitcher->SetActiveWidgetIndex(1);
		UE_LOG(LogServerDetailsModerationConsole, Display, TEXT("LogButton clicked"));
	});

	PlayerSelector->OnSelectedPlayersChanged.AddWeakLambda(this, [this](const TArray<UPlayerServerDetailsDataObject*>& SelectedPlayers)
	{
		OnSelectedPlayersChanged.Broadcast(SelectedPlayers);
		UE_LOG(LogServerDetailsModerationConsole, Display, TEXT("PlayerSelector OnSelectedPlayersChanged"));
	});

	SelectButton(AllButton);
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
}

void UServerDetailsModerationConsole::SelectButton(UNodecraftButtonBase* Button)
{
	AllButton->ClearSelection();
	StaffButton->ClearSelection();
	PlayersButton->ClearSelection();
	BannedButton->ClearSelection();
	LogButton->ClearSelection();
	AllButton->SetIsSelected(AllButton == Button);
	StaffButton->SetIsSelected(StaffButton == Button);
	PlayersButton->SetIsSelected(PlayersButton == Button);
	BannedButton->SetIsSelected(BannedButton == Button);
	LogButton->SetIsSelected(LogButton == Button);
}

void UServerDetailsModerationConsole::LoadData(const FString& InServerId)
{
	ServerId = InServerId;
	ReloadData();
}

void UServerDetailsModerationConsole::ReloadData()
{
	LoadGuard->SetIsLoading(true);
	
	UModerationService &ModerationService = UModerationService::Get();
	if (const UWorld* World = GetWorld())
	{
		RequestQueue = MakeShared<FHTTPRequestQueue>();
		TArray<TSharedRef<IHttpRequest>> Requests;
		Requests.Add(ModerationService.GetModeratorsRequest(World, ServerId));
		Requests.Add(ModerationService.GetOnlinePlayersRequest(World, ServerId));
		Requests.Add(ModerationService.GetOfflinePlayersRequest(World, ServerId));
		Requests.Add(ModerationService.GetBannedPlayersRequest(World, ServerId));
		Requests.Add(ModerationService.GetOwnerRequest(World, ServerId));
		RequestQueue.Get()->RunQueue(this, Requests, FAllRequestsFinishedDelegate::CreateWeakLambda(this, [this](bool bSuccess)
		{
			LoadGuard->SetIsLoading(false);
		}));
	}
}

void UServerDetailsModerationConsole::ClearPlayerSelection()
{
	PlayerSelector->ClearSelection();
}
