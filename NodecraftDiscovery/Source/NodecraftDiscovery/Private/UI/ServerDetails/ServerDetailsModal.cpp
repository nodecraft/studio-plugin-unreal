// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ServerDetails/ServerDetailsModal.h"

#include "Services/ServersService.h"
#include "Subsystems/MenuManagerSubsystem.h"
#include "UI/Alerts/AlertMessage.h"

void UServerDetailsModal::ToggleIsFavorite()
{
	UServersService& ServersService = UServersService::Get();
	FSimpleServiceResponseDelegate SimpleServiceResponseDelegate;
	SimpleServiceResponseDelegate.BindWeakLambda(this, [this](bool bSuccess, TOptional<FText> Error)
	{
		if (bSuccess)
		{
			ServerData->SetIsFavorite(!ServerData->IsFavorite());
			FavoriteButton->SetIsSelected(ServerData->IsFavorite());

			// todo: UE-253 update server lists in main menu with newly (un)favorited server cards
		}
	});
	if (ServerData->IsFavorite())
	{
		ServersService.UnfavoriteServer(ServerData->GetId(), SimpleServiceResponseDelegate);
	}
	else
	{
		ServersService.FavoriteServer(ServerData->GetId(), SimpleServiceResponseDelegate);
	}
}

void UServerDetailsModal::OnGetServerDetailsComplete(UServerDataObject* ServerDataObject, bool bSuccess, TOptional<FText> Error)
{
	if (bSuccess)
	{
		ServerData = ServerDataObject;
		HeaderCard->SetServerData(ServerDataObject);
		OverviewSection->SetServerData(ServerDataObject);
		RulesSection->SetServerData(ServerDataObject);
		ModerationHistorySection->SetServerData(ServerDataObject);
		AboutSection->SetServerData(ServerDataObject);

		// sets the style and state of "star" favorite button to (un)selected
		FavoriteButton->SetIsSelected(ServerDataObject->IsFavorite());

		switch (ServerDataObject->GetRole())
		{
		case EPlayerRole::Unknown:
			break;
		case EPlayerRole::Player:
			PrivateServerTabList->SetTabVisibility("ModerationLogTab", ESlateVisibility::Visible);
			break;
		case EPlayerRole::Moderator:
			PrivateServerTabList->SetTabVisibility("ModerationConsoleTab", ESlateVisibility::Visible);
			break;
		case EPlayerRole::Owner:
			PrivateServerTabList->SetTabVisibility("ModerationConsoleTab", ESlateVisibility::Visible);
			PrivateServerTabList->SetTabVisibility("SettingsTab", ESlateVisibility::Visible);
			break;
		}

		JoinButton->OnClicked().AddWeakLambda(this, [this, ServerDataObject]
		{
			if (ServerDataObject->HasPassword())
			{
				UMenuManagerSubsystem::Get().ShowServerPasswordModal(ServerDataObject);
			}
			else
			{
				UMenuManagerSubsystem::Get().ShowJoiningServerQueue(GetWorld(), ServerDataObject);
			}
		});
	}
}

void UServerDetailsModal::LoadServerDetails(UServerDataObject* ServerDataObject)
{
	const FString ServerID = ServerDataObject->GetId();
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

	switch (ServerDataObject->GetType())
	{
	case EServerType::Community:
		CommunityServerSection->SetVisibility(ESlateVisibility::Visible);
		PrivateServerSection->SetVisibility(ESlateVisibility::Collapsed);
		break;
	case EServerType::Private:
		CommunityServerSection->SetVisibility(ESlateVisibility::Collapsed);
		PrivateServerSection->SetVisibility(ESlateVisibility::Visible);
		// Hide these until we know what the player's role is
		PrivateServerTabList->SetTabVisibility("ModerationLogTab", ESlateVisibility::Collapsed);
		PrivateServerTabList->SetTabVisibility("ModerationConsoleTab", ESlateVisibility::Collapsed);
		PrivateServerTabList->SetTabVisibility("SettingsTab", ESlateVisibility::Collapsed);
		if (ModerationConsole)
		{
			ModerationConsole->LoadData(ServerID);
		}
		break;
	default:
		break;
	}
}

void UServerDetailsModal::OnTabContentCreated(FName TabId, UCommonUserWidget* TabWidget)
{
	if (TabWidget->Implements<UServerDetailsSection>())
	{
		Cast<IServerDetailsSection>(TabWidget)->SetServerData(ServerData);
	}
	
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

void UServerDetailsModal::OnPrivateTabSelected(FName TabId)
{
	if (TabId == "ModerationConsoleTab")
	{
		OnlinePlayersList->SetVisibility(ESlateVisibility::Collapsed);
		RecentPlayersList->SetVisibility(ESlateVisibility::Collapsed);
		PlayerDetailsPanelContainer->SetVisibility(ESlateVisibility::Visible);
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
	CommunityServerTabList->SetLinkedSwitcher(CommunityServerTabContentSwitcher);
	CommunityServerTabList->OnTabContentCreated.AddDynamic(this, &ThisClass::OnTabContentCreated);

	PrivateServerTabList->SetLinkedSwitcher(PrivateServerTabContentSwitcher);
	PrivateServerTabList->OnTabContentCreated.AddDynamic(this, &ThisClass::OnTabContentCreated);

	FavoriteButton->OnClicked().AddUObject(this, &ThisClass::ToggleIsFavorite);

	PrivateServerTabList->OnTabSelected.AddDynamic(this, &ThisClass::OnPrivateTabSelected);

	ModerationConsole->OnSelectedPlayersChanged.AddWeakLambda(this, [this](const TArray<UPlayerServerDetailsDataObject*>& SelectedPlayers)
	{
		PlayerDetailsPanel->SetSelectedPlayers(SelectedPlayers);
	});
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
}
