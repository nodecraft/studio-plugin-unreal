// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ServerLists/ServerList.h"

#include "Models/ServerDataObject.h"
#include "Services/ServersService.h"

// void UServerList::PopulateWithServerJson(const TArray<TSharedPtr<FJsonObject>> Json)
// {
// 	TArray<UServerDataObject*> ServerDataObjects;
// 	for (TSharedPtr<FJsonObject> JsonObject : Json)
// 	{
// 		ServerDataObjects.Add(UServerDataObject::FromJson(JsonObject.ToSharedRef()));
// 	}
// }

void UServerList::NativeConstruct()
{
	Super::NativeConstruct();

	// Get the servers service
	UServersService& ServersService = UServersService::Get();
	
	// Use it to request the popular servers
	FGetServersListDelegate OnComplete;
	// Call BindLambda on OnComplete, and pass in a lambda that calls PopulateWithServerJson
	//
	OnComplete.BindWeakLambda(this, [this](TArray<UServerDataObject*> Servers, bool Success, TOptional<FText> Error)
	{
		if (Success)
		{
			ServerListView->SetListItems(Servers);
		}
	});

	switch (ServerListType)
	{
	case EServerListType::Popular:
		ServersService.GetPopularServers(OnComplete);
		break;

	case EServerListType::Favorite:
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
