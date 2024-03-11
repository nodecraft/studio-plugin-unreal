// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ServerDetails/TabbedSections/ServerDetailsModerationConsole.h"

#include "Services/ModerationService.h"

DEFINE_LOG_CATEGORY_STATIC(LogServerDetailsModerationConsole, All, All);

void UServerDetailsModerationConsole::NativeOnInitialized()
{
	Super::NativeOnInitialized();

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
	UModerationService &ModerationService = UModerationService::Get();
	auto Moderators = ModerationService.GetModerators(InServerId, FGetPlayerServerDetailsList::CreateWeakLambda(this, [this](const TArray<UPlayerServerDetailsDataObject*>& Players, bool bSuccess, TOptional<FText> Error)
	{
		if (bSuccess)
		{
			PlayerSelector->SetModerators(Players);
			UE_LOG(LogServerDetailsModerationConsole, Display, TEXT("GetModerators succeeded"));
		}
		else
		{
			UE_LOG(LogServerDetailsModerationConsole, Error, TEXT("GetModerators failed: %s"), *Error.GetValue().ToString());
		}
	}));

	auto Banned = ModerationService.GetBannedPlayers(InServerId, FGetPlayerServerDetailsList::CreateWeakLambda(this, [this](const TArray<UPlayerServerDetailsDataObject*>& Players, bool bSuccess, TOptional<FText> Error)
	{
		if (bSuccess)
		{
			PlayerSelector->SetBannedPlayers(Players);
			UE_LOG(LogServerDetailsModerationConsole, Display, TEXT("GetBannedPlayers succeeded"));
		}
		else
		{
			UE_LOG(LogServerDetailsModerationConsole, Error, TEXT("GetBannedPlayers failed: %s"), *Error.GetValue().ToString());
		}
	}));

	auto Online = ModerationService.GetOnlinePlayers(InServerId, FGetPlayerServerDetailsList::CreateWeakLambda(this, [this](const TArray<UPlayerServerDetailsDataObject*>& Players, bool bSuccess, TOptional<FText> Error)
	{
		if (bSuccess)
		{
			PlayerSelector->SetOnlinePlayers(Players);
			UE_LOG(LogServerDetailsModerationConsole, Display, TEXT("GetOnlinePlayers succeeded"));
		}
		else
		{
			UE_LOG(LogServerDetailsModerationConsole, Error, TEXT("GetOnlinePlayers failed: %s"), *Error.GetValue().ToString());
		}
	}));

	auto Offline = ModerationService.GetOfflinePlayers(InServerId, FGetPlayerServerDetailsList::CreateWeakLambda(this, [this](const TArray<UPlayerServerDetailsDataObject*>& Players, bool bSuccess, TOptional<FText> Error)
	{
		if (bSuccess)
		{
			PlayerSelector->SetOfflinePlayers(Players);
			UE_LOG(LogServerDetailsModerationConsole, Display, TEXT("GetOfflinePlayers succeeded"));
		}
		else
		{
			UE_LOG(LogServerDetailsModerationConsole, Error, TEXT("GetOfflinePlayers failed: %s"), *Error.GetValue().ToString());
		}
	}));

	auto Owner = ModerationService.GetOwner(InServerId, FGetPlayerServerDetails::CreateWeakLambda(this, [this](UPlayerServerDetailsDataObject* Player, bool bSuccess, TOptional<FText> Error)
	{
		if (bSuccess)
		{
			PlayerSelector->SetOwner(Player);
			UE_LOG(LogServerDetailsModerationConsole, Display, TEXT("GetOwner succeeded"));
		}
		else
		{
			UE_LOG(LogServerDetailsModerationConsole, Error, TEXT("GetOwner failed: %s"), *Error.GetValue().ToString());
		}
	}));

	TArray<TSharedRef<IHttpRequest>> AllRequests = TArray<TSharedRef<IHttpRequest>>();
	AllRequests.Add(Moderators);
	AllRequests.Add(Banned);
	AllRequests.Add(Online);
	AllRequests.Add(Offline);
	AllRequests.Add(Owner);

	RequestQueue = MakeShared<FHTTPRequestQueue>();
	RequestQueue->RunQueue(this, AllRequests, FAllRequestsFinishedDelegate::CreateWeakLambda(this, [this](bool bSuccess)
	{
		UE_LOG(LogServerDetailsModerationConsole, Display, TEXT("All requests completed"));
	}));
}
