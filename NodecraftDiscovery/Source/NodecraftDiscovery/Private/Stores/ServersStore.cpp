// Nodecraft, Inc. © 2012-2024, All Rights Reserved.


#include "Stores/ServersStore.h"

#include "NodecraftLogCategories.h"
#include "API/ServerListType.h"
#include "Services/ServersService.h"
#include "Subsystems/NodecraftUIManagerSubsystem.h"
#include "UI/ServerLists/ServerListPage.h"

void UServersStore::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	PaginationDetailsMap.Empty();
	PaginationDetailsMap.Add(EServerListType::Favorite, NewObject<UPaginationDataObject>());
	PaginationDetailsMap.Add(EServerListType::Owned, NewObject<UPaginationDataObject>());
	PaginationDetailsMap.Add(EServerListType::Popular, NewObject<UPaginationDataObject>());
	PaginationDetailsMap.Add(EServerListType::Recommended, NewObject<UPaginationDataObject>());

	// Whenever the world is initialized, i.e. when we move into a game or when we return to the main menu, we want to clear the cache.
	FWorldDelegates::OnPostWorldInitialization.AddWeakLambda(this, [this](UWorld* World, const UWorld::InitializationValues IV)
	{
		ClearCache();
	});
}

TArray<UServerDataObject*> UServersStore::GetFavoriteServersData() const
{
	TArray<UServerDataObject*> FavServers;
	for (const FString& ServerId : FavoriteServers)
	{
		if (UServerDataObject* Serv = ServersMap[ServerId])
		{
			FavServers.Add(Serv);
		}
	}
	return FavServers;
}

void UServersStore::AddFavoriteServer(UServerDataObject* InServer)
{
	if (ServersMap.Contains(InServer->GetId()))
	{
		ServersMap.Remove(InServer->GetId());
		ServersMap.Add(InServer->GetId(), InServer);
	}
	else
	{
		ServersMap.Add(InServer->GetId(), InServer);
	}
	FavoriteServers.Add(InServer->GetId());
	OnFavoriteServersUpdated.Broadcast(InServer, GetFavoriteServersData());
}

void UServersStore::RemoveFavoriteServer(UServerDataObject* InServer)
{
	if (ServersMap.Contains(InServer->GetId()))
	{
		ServersMap.Remove(InServer->GetId());
		ServersMap.Add(InServer->GetId(), InServer);
	}
	else
	{
		ServersMap.Add(InServer->GetId(), InServer);
	}
	FavoriteServers.Remove(InServer->GetId());
	OnFavoriteServersUpdated.Broadcast(InServer, GetFavoriteServersData());
}

void UServersStore::AddServers(const EServerListType List, const TArray<UServerDataObject*>& Servers, UPaginationDataObject* PageDetails)
{
	if (PageDetails->GetPage() > PaginationDetailsMap[List]->GetPage())
	{
		PaginationDetailsMap[List] = PageDetails;
	}

	if (PagesBeingLoaded.Contains(TTuple<EServerListType, int32>(List, PageDetails->GetPage())))
	{
		PagesBeingLoaded.Remove(TTuple<EServerListType, int32>(List, PageDetails->GetPage()));
	}

	TArray<FString>& TargetServerIDs = GetServerIDsForList(List);
	
	for (UServerDataObject* Server : Servers)
	{
		ServersMap.Remove(Server->GetId());
		ServersMap.Add(Server->GetId(), Server);
		TargetServerIDs.AddUnique(Server->GetId());
	}

	if (List == EServerListType::Favorite)
	{
		OnFavoriteServersUpdated.Broadcast(nullptr, GetFavoriteServersData());
	}
}

TArray<FString>& UServersStore::GetServerIDsForList(const EServerListType List)
{
	switch (List)
	{
	case EServerListType::Popular:
		return PopularServers;
	case EServerListType::Favorite:
		return FavoriteServers;
	case EServerListType::Recommended:
		return RecommendedServers;
	case EServerListType::Owned:
		return FriendsServers;
	default:
		unimplemented()
		return PopularServers;
	}
}

FString UServersStore::ServerListTypeToString(EServerListType List)
{
	switch (List)
	{
	case EServerListType::Favorite:
		return "Favorite";
	case EServerListType::Owned:
		return "Private";
	case EServerListType::Recommended:
		return "Recommended";
	case EServerListType::Popular:
		return "Popular";
	default:
		return "Unknown";
	}
}

void UServersStore::GetServerListViewPage(UWorld* World, EServerListType List, int32 ViewPage, FOnDisplayServerListPage ResultsCallback)
{
	ensureMsgf(ViewPage > 0, TEXT("ViewPage must be greater than 0. Page is indexed to 1."));
	FServerListPage ServerListPageDetails = FServerListPage();
	
	// ViewPage is the "player's current page" in the list, but this doesn't correspond to the actual page number in the list.
	// We need to calculate the first and last item in the list based on the ViewPage.
	if (World)
	{
		// When we have only one more (client) page of tiles to render, we want to load more tiles for that list
		if (UGameInstance* GI = World->GetGameInstance())
		{
			const UPaginationDataObject* PaginationDetails = PaginationDetailsMap[List];

			ServerListPageDetails.bPrependCreateServerButton = List == EServerListType::Owned && ViewPage == 1;

			const int32 ItemsPerViewPage = UNodecraftUIManagerSubsystem::Get(GI)->GetNumTilesInServerList();
			const int32 MaxViewPage = FMath::CeilToInt(static_cast<float>(PaginationDetails->GetCount()) / ItemsPerViewPage);
			const int32 ViewPageNum = ViewPage > MaxViewPage ? MaxViewPage : ViewPage;
			const int32 TileOffset = (ViewPageNum - 1) * ItemsPerViewPage >= 0 ? (ViewPageNum - 1) * ItemsPerViewPage : 0;
			// If we are on the first page of the owned list, we want to show the create server button.
			// If we are on any viewpage past the first view page of the owned list, we need to account for the create server button
			// by subtracting 1 from the first item index.
			const int32 FirstItemIndex = TileOffset - (List == EServerListType::Owned && ViewPage > 1 ? 1 : 0);
			const int32 LastItemIndex = TileOffset + ItemsPerViewPage - 1 - (ServerListPageDetails.bPrependCreateServerButton ? 1 : 0);

			ServerListPageDetails.bIsLastViewPage = ViewPageNum == MaxViewPage;

			// Check to see if this is the first time we are loading the list
			if (PaginationDetails->IsUninitialized())
			{
				// We haven't loaded any pages yet
				FGetServersListDelegate OnComplete = FGetServersListDelegate::CreateWeakLambda(
					this, [this, World, List, ViewPage, ResultsCallback](
					TArray<UServerDataObject*> Servers, UPaginationDataObject* PaginationDetails,
					bool bSuccess, TOptional<FText> Error)
				{
						if (bSuccess)
						{
							GetServerListViewPage(World, List, ViewPage, ResultsCallback);
						}
						else
						{
							// TODO: Likely want to implement more robust error handling here in future
							UE_LOG(LogNodecraft_Servers, Error, TEXT("Failed to load initial servers for list %s"), *ServerListTypeToString(List));
						}
				});

				PagesBeingLoaded.Add(TTuple<EServerListType, int32>(List, 1));
				UServersService::Get().GetServersForList(List, OnComplete, 1);
				// Exit early because we are loading the first page
				return;
			}
			
			ServerListPageDetails.ViewPageNumber = ViewPage;

			TArray<UServerDataObject*> Servers;

			TArray<FString> TargetServers = GetServerIDsForList(List);

			// If FirstItemIndex < 0 it's because we have nothing to show
			if (FirstItemIndex >= 0 && TargetServers.Num() > 0)
			{
				for (int32 i = FirstItemIndex; i <= LastItemIndex; i++)
				{
					if (i < TargetServers.Num())
					{
						if (ServersMap.Contains(TargetServers[i]))
						{
							Servers.Add(ServersMap[TargetServers[i]]);
						}
					}
				}
			}
			else if (List == EServerListType::Favorite)
			{
				// If this is an empty favorites list, we should hide it
				ServerListPageDetails.bShouldHideList = true;
			}
			
			
			ServerListPageDetails.Servers = Servers;

			// We need to check if we have a previous page and a next page
			UServerDataObject* PreviousServerPreview = nullptr;
			UServerDataObject* NextServerPreview = nullptr;
			// If our first time is greater than 0, our previous page is just the one before the first item,
			// but if it's 0, the preview item should be the item at the end of the list unless that item is already the LastItem

			if (FirstItemIndex > 0)
			{
				if (ServersMap.Contains(TargetServers[FirstItemIndex - 1]))
				{
					PreviousServerPreview = ServersMap[TargetServers[FirstItemIndex - 1]];
				}
			}
	
			if (LastItemIndex + 1 < TargetServers.Num())
			{
				if (ServersMap.Contains(TargetServers[LastItemIndex + 1]))
				{
					NextServerPreview = ServersMap[TargetServers[LastItemIndex + 1]];
				}
			}

			ServerListPageDetails.NextServerPreview = NextServerPreview;
			ServerListPageDetails.PreviousServerPreview = PreviousServerPreview;

			// If we are loading the next page or prev page, we need to set the loading state

			// Calculate what data page the next view page will be on
			const int32 CurrentDataPage = FMath::CeilToInt(static_cast<float>(LastItemIndex + 1) / PaginationDetails->GetEntriesPerPage());
			const int32 DataPageForNextViewPage = FMath::CeilToInt(static_cast<float>(LastItemIndex + 1 + ItemsPerViewPage) / PaginationDetails->GetEntriesPerPage());
			// If the next view page will take us to a data page we don't have, load that page
			if (DataPageForNextViewPage > PaginationDetails->GetPage() && CurrentDataPage != PaginationDetails->GetPagesMax())
			{
				// Check if we are already loading the next page
				if (!PagesBeingLoaded.Contains(TTuple<EServerListType, int32>(List, CurrentDataPage + 1)))
				{
					// // Load the next page
					FGetServersListDelegate OnComplete = FGetServersListDelegate::CreateWeakLambda(
						this, [this, ServerListPageDetails, World, List, ViewPage, ResultsCallback, LastItemIndex](
						TArray<UServerDataObject*> Servers, UPaginationDataObject* PaginationDetails,
						bool bSuccess, TOptional<FText> Error)
						{
							PagesBeingLoaded.Remove(TTuple<EServerListType, int32>(List, PaginationDetails->GetPage()));
							if (bSuccess)
							{
								FServerListPage UpdatedServerListPage = ServerListPageDetails;
								// Populate the next server preview
								TArray<FString>& LatestIdsForList = GetServerIDsForList(List);
								if (LastItemIndex + 1 < LatestIdsForList.Num())
								{
									if (ServersMap.Contains(LatestIdsForList[LastItemIndex + 1]))
									{
										UpdatedServerListPage.NextServerPreview = ServersMap[LatestIdsForList[LastItemIndex + 1]];
									}
								}
								UpdatedServerListPage.bIsLoadingNextPage = false;
								// When looking at this again, you need to update OnLoadedViewPage to take in these three arguments
								// and then you need to update the view model to listen for this event and update the view page
								OnLoadedViewPage.Broadcast(List, ViewPage, UpdatedServerListPage);
							}
							else
							{
								// TODO: Likely want to implement more robust error handling here in future
								UE_LOG(LogNodecraft_Servers, Error, TEXT("Failed to load next page for list %s"), *ServerListTypeToString(List));
							}
						});
					PagesBeingLoaded.Add(TTuple<EServerListType, int32>(List, CurrentDataPage + 1));
					UServersService::Get().GetServersForList(List, OnComplete, CurrentDataPage + 1);
				}
			}
			
			// if the next data page is in the list of pages being loaded, we are loading the next page
			ServerListPageDetails.bIsLoadingNextPage = PagesBeingLoaded.Contains(TTuple<EServerListType, int32>(List, CurrentDataPage + 1));
			ServerListPageDetails.bIsLoadingPrevPage = PagesBeingLoaded.Contains(TTuple<EServerListType, int32>(List, CurrentDataPage - 1));
		}
	}
	
	ResultsCallback.ExecuteIfBound(ServerListPageDetails);
}

void UServersStore::SetCurrentServerId(FString ServerId)
{
	CurrentServerId = ServerId;
}

FString UServersStore::GetCurrentServerId() const
{
	return CurrentServerId;
}

void UServersStore::ClearCurrentServerId()
{
	CurrentServerId = "";
}

void UServersStore::ClearCache()
{
	PaginationDetailsMap.Empty();
	PaginationDetailsMap.Add(EServerListType::Favorite, NewObject<UPaginationDataObject>());
	PaginationDetailsMap.Add(EServerListType::Owned, NewObject<UPaginationDataObject>());
	PaginationDetailsMap.Add(EServerListType::Popular, NewObject<UPaginationDataObject>());
	PaginationDetailsMap.Add(EServerListType::Recommended, NewObject<UPaginationDataObject>());

	FavoriteServers.Empty();
	FriendsServers.Empty();
	RecommendedServers.Empty();
	PopularServers.Empty();
	ServersMap.Empty();
	PagesBeingLoaded.Empty();
}
