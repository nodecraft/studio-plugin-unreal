// Nodecraft, Inc. © 2012-2024, All Rights Reserved.


#include "Stores/ServersStore.h"

void UServersStore::SetFavoriteServers(const TArray<UServerDataObject*>& Servers)
{
	FavoriteServers = Servers;
	OnFavoriteServersUpdated.Broadcast(Servers);
}

void UServersStore::AddFavoriteServer(UServerDataObject* Server)
{
	FavoriteServers.Add(Server);
	OnFavoriteServersUpdated.Broadcast(FavoriteServers);
}

void UServersStore::RemoveFavoriteServer(UServerDataObject* Server)
{
	auto Favs = FavoriteServers;
	for (UServerDataObject* Fav : Favs)
	{
		if (Fav->GetId() == Server->GetId())
		{
			FavoriteServers.Remove(Fav);
			break;
		}
	}
	OnFavoriteServersUpdated.Broadcast(FavoriteServers);
}
