// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PlayerList/PlayerList.h"

#include "CommonLoadGuard.h"
#include "Models/PlayerDataObject.h"
#include "Models/PlayerListDataObject.h"
#include "Services/ServersService.h"

#define LOCTEXT_NAMESPACE "PlayerList"

void UPlayerList::SetTitleText(const FText Text)
{
	TitleTextBlock->SetText(Text);
}

void UPlayerList::LoadData(EPlayerListType PlayerListType, UServerDataObject* ServerDataObject)
{
	// Get the servers service
	UServersService& ServersService = UServersService::Get();
	
	// Use it to request the popular servers
	FGetPlayersListDelegate OnComplete;
	// Call BindLambda on OnComplete, and pass in a lambda that calls PopulateWithServerJson
	//
	LoadGuard->SetIsLoading(true);
	OnComplete.BindWeakLambda(this, [this, PlayerListType]
		(UPlayerListDataObject* PlayerListDataObject, bool Success, TOptional<FText> Error)
	{
		if (Success)
		{
			if (PlayerListDataObject->GetPlayerDataObjects().Num() > 0)
			{
				NoPlayersImage->SetVisibility(ESlateVisibility::Hidden);
				NoPlayersTextBlock->SetVisibility(ESlateVisibility::Hidden);
			}
			else
			{
				NoPlayersImage->SetVisibility(ESlateVisibility::HitTestInvisible);
				NoPlayersTextBlock->SetVisibility(ESlateVisibility::HitTestInvisible);
			}
			SetServerData(PlayerListType, PlayerListDataObject);

			LoadGuard->SetIsLoading(false);
		}
	});

	switch (PlayerListType)
	{
	case EPlayerListType::Online:
		ServersService.GetOnlinePlayers(ServerDataObject->GetId(), OnComplete);
		break;
		
	case EPlayerListType::Recent:
		ServersService.GetRecentPLayers(ServerDataObject->GetId(), OnComplete);
		break;
		
	default:
		break;
	}
}

void UPlayerList::SetServerData(EPlayerListType PlayerListType, UPlayerListDataObject* PlayerListDataObject) const
{
	if (PlayerListDataObject)
	{
		switch (PlayerListType)
		{
		case EPlayerListType::Online:
			TitleTextBlock->SetText(FText::Format(LOCTEXT("NumOnlinePlayers", "{0}/{1} players online"),
				PlayerListDataObject->GetPlayersCount(), PlayerListDataObject->GetMaxPlayersCount()));
			break;
			
		case EPlayerListType::Recent:
			TitleTextBlock->SetText(LOCTEXT("RecentPlayers", "Recent players"));
			break;

		default:
			break;
		}
		PlayerListView->SetListItems(PlayerListDataObject->GetPlayerDataObjects());
	}
}

#undef LOCTEXT_NAMESPACE
