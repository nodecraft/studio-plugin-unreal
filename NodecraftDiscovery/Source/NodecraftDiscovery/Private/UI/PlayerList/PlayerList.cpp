// Nodecraft, Inc. © 2012-2024, All Rights Reserved.


#include "UI/PlayerList/PlayerList.h"

#include "CommonLoadGuard.h"
#include "Models/PlayerListDataObject.h"
#include "Services/ServersService.h"
#include "Stores/ServersStore.h"

#define LOCTEXT_NAMESPACE "PlayerList"

void UPlayerList::NativePreConstruct()
{
	Super::NativePreConstruct();
	if (NoPlayersImageAsset)
	{
		NoPlayersImage->SetBrushFromTexture(NoPlayersImageAsset);
	}
}

void UPlayerList::NativeConstruct()
{
	Super::NativeConstruct();
	NoPlayersImage->SetVisibility(ESlateVisibility::Hidden);
	NoPlayersTextBlock->SetVisibility(ESlateVisibility::Hidden);
	if (NoPlayersSubTextBlock)
	{
		NoPlayersSubTextBlock->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UPlayerList::SetTitleText(const FText Text)
{
	TitleTextBlock->SetText(Text);
}

void UPlayerList::SetListeningForScrollInput(bool bIsListeningForInput)
{
	ScrollBox->SetListeningForInput(bIsListeningForInput, ActionDisplayName);
}

bool UPlayerList::IsListeningForScrollInput() const
{
	return ScrollBox->GetListeningForInput();
}

void UPlayerList::LoadData(EPlayerListType PlayerListType, UServerDataObject* ServerDataObject, FSimpleDelegate InOnComplete)
{
	// Get the servers service
	UServersService& ServersService = UServersService::Get();
	
	// Use it to request the popular servers
	FGetPlayersListDelegate OnComplete;
	// Call BindLambda on OnComplete, and pass in a lambda that calls PopulateWithServerJson
	LoadGuard->SetIsLoading(true);
	OnComplete.BindWeakLambda(this, [this, PlayerListType, ServerDataObject, InOnComplete]
		(UPlayerListDataObject* PlayerListDataObject, bool Success, TOptional<FText> Error)
	{
		if (Success && UServersStore::Get().GetCurrentServerId() == ServerDataObject->GetId())
		{
			if (PlayerListDataObject->GetPlayerDataObjects().Num() > 0)
			{
				NoPlayersImage->SetVisibility(ESlateVisibility::Hidden);
				NoPlayersTextBlock->SetVisibility(ESlateVisibility::Hidden);
				if (NoPlayersSubTextBlock)
				{
					NoPlayersSubTextBlock->SetVisibility(ESlateVisibility::Hidden);
				}
			}
			else
			{
				NoPlayersImage->SetVisibility(ESlateVisibility::HitTestInvisible);
				NoPlayersTextBlock->SetVisibility(ESlateVisibility::HitTestInvisible);
				if (PlayerListType == EPlayerListType::Recent)
				{
					if (NoPlayersSubTextBlock)
					{
						NoPlayersSubTextBlock->SetVisibility(ESlateVisibility::Hidden);
					}
				}
				else if (NoPlayersSubTextBlock)
				{
					NoPlayersSubTextBlock->SetVisibility(ESlateVisibility::HitTestInvisible);
				}
			}
			SetServerData(PlayerListType, PlayerListDataObject);

			LoadGuard->SetIsLoading(false);
			InOnComplete.ExecuteIfBound();
		}
	});

	switch (PlayerListType)
	{
	case EPlayerListType::Online:
		ServersService.GetOnlinePlayers(ServerDataObject->GetId(), OnComplete);
		NoPlayersTextBlock->SetText(LOCTEXT("NoOnlinePlayers", "No Players Online"));
		if (NoPlayersSubTextBlock)
		{
			NoPlayersSubTextBlock->SetText(LOCTEXT("JoinToBeFirstInServer", "Join to be the first player in this server!"));
		}
		break;
		
	case EPlayerListType::Recent:
		ServersService.GetRecentPLayers(ServerDataObject->GetId(), OnComplete);
		NoPlayersTextBlock->SetText(LOCTEXT("NoServerHistory", "No Server History"));
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

bool UPlayerList::HasItems()
{
	return PlayerListView->GetNumItems() > 0;
}

#undef LOCTEXT_NAMESPACE
