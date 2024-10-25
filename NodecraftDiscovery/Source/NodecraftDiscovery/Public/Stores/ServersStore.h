// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "API/ServerListType.h"
#include "Models/PaginationDataObject.h"
#include "Models/ServerDataObject.h"
#include "Subsystems/EngineSubsystem.h"
#include "UI/ServerLists/ServerListPage.h"
#include "Engine/Engine.h"
#include "ServersStore.generated.h"

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnFavoriteServersUpdated, UServerDataObject* /*UpdatedServer*/, const TArray<UServerDataObject*> /*FavoriteServers*/);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnServerListUpdated, const TArray<UServerDataObject*>);
DECLARE_DELEGATE_OneParam(FOnDisplayServerListPage, FServerListPage /*ServerListPage*/);

DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnFinishedLoadingForViewPage, EServerListType /*ServerListType*/, int32 /*ViewPage*/, const FServerListPage& /*ServerListPage*/);

UCLASS()
class NODECRAFTDISCOVERY_API UServersStore : public UEngineSubsystem
{
	GENERATED_BODY()

public:
	static UServersStore& Get() { return *GEngine->GetEngineSubsystem<UServersStore>(); }

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	void AddFavoriteServer(UServerDataObject* InServer);
	void RemoveFavoriteServer(UServerDataObject* InServer);

	void AddServers(const EServerListType List, const TArray<UServerDataObject*>& Servers, UPaginationDataObject* PageDetails);

	void GetServerListViewPage(UWorld* World, EServerListType List, int32 ViewPage, FOnDisplayServerListPage ResultsCallback);

	void SetCurrentServerId(FString ServerId);
	FString GetCurrentServerId() const;
	void ClearCurrentServerId();
	void ClearCache();
	FString ServerListTypeToString(EServerListType List);

	FOnFavoriteServersUpdated OnFavoriteServersUpdated;
	// FOnServerListUpdated OnFavoriteServersUpdated;
	FOnServerListUpdated OnPopularServersUpdated;

	FOnFinishedLoadingForViewPage OnLoadedViewPage;
	

private:
	UPROPERTY()
	TArray<FString> FavoriteServers;

	UPROPERTY()
	TArray<FString> FriendsServers;

	UPROPERTY()
	TArray<FString> RecommendedServers;

	UPROPERTY()
	TArray<FString> PopularServers;

	TArray<FString>& GetServerIDsForList(EServerListType List);

	UPROPERTY()
	// Maps server ID to server data object
	TMap<FString, UServerDataObject*> ServersMap;

	FString CurrentServerId;

	TArray<TTuple<EServerListType, int32>> PagesBeingLoaded; 

	// For now, we are assuming that the pagination data objecte represents the 'latest' page that has been loaded.
	// For this reason, we are currently not supporting moving from the first page to the "last" page by moving backwards.
	// This would require that we keep track of all pages that have been loaded for a given list type.
	// Our current implementation is less rich in this sense but simpler to implement, so we are going with it for now.
	UPROPERTY()
	TMap<EServerListType, UPaginationDataObject*> PaginationDetailsMap;

	TArray<UServerDataObject*> GetFavoriteServersData() const;
};

