// Nodecraft, Inc. © 2012-2024, All Rights Reserved.


#include "UI/ServerLists/ServerList.h"

#include "Models/ServerDataObject.h"
#include "Services/ServersService.h"
#include "Stores/ServersStore.h"
#include "UI/ServerLists/CreateServerPlaceholder.h"
#include "UI/ServerLists/LoadingServersPlaceholder.h"

void UServerList::NativeConstruct()
{
	Super::NativeConstruct();

	// Get the servers service
	UServersService& ServersService = UServersService::Get();
	
	// Use it to request the popular servers
	FGetServersListDelegate OnComplete;
	// Call BindLambda on OnComplete, and pass in a lambda that calls PopulateWithServerJson
	//
	OnComplete.BindWeakLambda(this, [this](TArray<UServerDataObject*> Servers, UPaginationDataObject* PaginationDataObject, bool Success, TOptional<FText> Error)
	{
		if (Success)
		{
			// If this is a Private server list, prepend an empty server entry to show the create server button
			// This is a little bit of a cheat, but it's very cheap and effective
			if (ServerListType == EServerListType::Owned)
			{
				UServerDataObject* EmptyServer = NewObject<UServerDataObject>();
				Servers.Insert(EmptyServer, 0);
			}
			ServerListView->SetVisibility(ESlateVisibility::Visible);
			ServerListView->SetListItems(Servers);
			LoadingServersListView->SetVisibility(ESlateVisibility::Collapsed);
		}
	});

	// To start with, hide the server list and show the loading placeholders
	ServerListView->SetVisibility(ESlateVisibility::Collapsed);
	// We need to create a TArray of UObject pointers to pass to the SetListItems function,
	// because nullptrs won't work.
	// We use the CreateServerPlaceholder class to indicate that we want to show a create server button at the first index.
	// The rest of the objects are just empty objects, which will be shown as animated placeholders
	TArray<UObject*> LoadingPlaceholders = { NewObject<ULoadingServersPlaceholder>(), NewObject<ULoadingServersPlaceholder>(), NewObject<ULoadingServersPlaceholder>(), NewObject<ULoadingServersPlaceholder>(), NewObject<ULoadingServersPlaceholder>() };
	if (ServerListType == EServerListType::Owned)
	{
		// If this is a private server list, we want to show the create server button at the first index
		LoadingPlaceholders[0] = NewObject<UCreateServerPlaceholder>();
	}
	LoadingServersListView->SetListItems(LoadingPlaceholders);
	LoadingServersListView->SetVisibility(ESlateVisibility::Visible);

	UServersStore::Get().OnFavoriteServersUpdated.AddWeakLambda(
		this, [this](UServerDataObject* UpdatedServer, TArray<UServerDataObject*> FavoriteServers)
	{
		// TODO: UE-497 clean up this split behavior between favorite and non-favorite lists  
		if (ServerListType == EServerListType::Favorite)
		{
			ServerListView->SetVisibility(ESlateVisibility::Visible);
			ServerListView->SetListItems(FavoriteServers);
			LoadingServersListView->SetVisibility(ESlateVisibility::Collapsed);
			// If we have no favorite servers, hide the widget
			SetVisibility(FavoriteServers.IsEmpty() ? ESlateVisibility::Collapsed : ESlateVisibility::Visible);
		}
		else if (UpdatedServer)
		{
			TArray<UObject*> ListItems = ServerListView->GetListItems();
			for (int32 i = 0; i < ListItems.Num(); i++)
			{
				UServerDataObject* ServerListItem = Cast<UServerDataObject>(ListItems[i]);
				if (ServerListItem && ServerListItem->GetId() == UpdatedServer->GetId())
				{
					ListItems[i] = UpdatedServer;
					ServerListView->SetListItems(ListItems);
					break;
				}
			}
		}
	});

	// Load the servers based on the type of list we're showing
	switch (ServerListType)
	{
	case EServerListType::Popular:
		ServersService.GetPopularServers(OnComplete);
		break;

	case EServerListType::Favorite:
		// Favorites list should start collapsed
		SetVisibility(ESlateVisibility::Collapsed);
		ServersService.GetFavoriteServers(OnComplete);
		break;

	case EServerListType::Recommended:
		ServersService.GetRecommendedServers(OnComplete);
		break;

	case EServerListType::Owned:
		ServersService.GetOwnedServers(OnComplete);
		break;

	default:
		break;
	}
}
