// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/EngineSubsystem.h"
#include "Engine/Engine.h"
#include "SearchStore.generated.h"

class UServerDataObject;

DECLARE_MULTICAST_DELEGATE_OneParam(FSearchQueryUpdatedDelegate, const FString& /*SearchQuery*/);
DECLARE_MULTICAST_DELEGATE_OneParam(FSearchResultsUpdatedDelegate, const TArray<UServerDataObject*>& /*SearchResults*/);

/**
 * 
 */
UCLASS()
class NODECRAFTDISCOVERY_API USearchStore : public UEngineSubsystem
{
	GENERATED_BODY()

public:
	static USearchStore& Get()
	{
		return *GEngine->GetEngineSubsystem<USearchStore>();
	}
	
	FDelegateHandle AddSearchQueryUpdatedListener(const FSearchQueryUpdatedDelegate::FDelegate& Delegate);
	bool RemoveSearchQueryUpdatedListener(const FDelegateHandle& Handle);
	FDelegateHandle AddSearchResultsUpdatedListener(const FSearchResultsUpdatedDelegate::FDelegate& Delegate);
	bool RemoveSearchResultsUpdatedListener(const FDelegateHandle& Handle);
	FDelegateHandle AddRequestShowSearchResultsListener(const FSimpleMulticastDelegate::FDelegate& Delegate);
	bool RemoveRequestShowSearchResultsListener(const FDelegateHandle& Handle);
	FDelegateHandle AddRequestClearSearchListener(const FSimpleMulticastDelegate::FDelegate& Delegate);
	bool RemoveRequestClearSearchListener(const FDelegateHandle& Handle);

	void SetSearchQuery(const FString& NewSearchQuery);
	void SetSearchResults(const TArray<UServerDataObject*>& NewSearchResults);
	void SetCurrentPageNumber(const uint32 NewCurrentPage);
	uint32 GetCurrentPageNumber() const;
	void SetMaxPages(const uint32 NewMaxPages);
	uint32 GetMaxPages() const;
	void ShowSearchResults();
	void ClearSearch();

private:
	FSearchQueryUpdatedDelegate OnSearchQueryUpdated;
	FSearchResultsUpdatedDelegate OnSearchResultsUpdated;
	FSimpleMulticastDelegate OnRequestShowSearchResults;
	FSimpleMulticastDelegate OnRequestClearSearch;

	FString SearchQuery;
	uint32 CurrentPage;
	uint32 MaxPages;

	UPROPERTY()
	TArray<UServerDataObject*> SearchResults;
};
