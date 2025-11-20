// Nodecraft, Inc. © 2012-2024, All Rights Reserved.


#include "Stores/SearchStore.h"

#include "Services/ServersService.h"


FDelegateHandle USearchStore::AddSearchQueryUpdatedListener(const FSearchQueryUpdatedDelegate::FDelegate& Delegate)
{
	Delegate.ExecuteIfBound(SearchQuery);
	return OnSearchQueryUpdated.Add(Delegate);
}

bool USearchStore::RemoveSearchQueryUpdatedListener(const FDelegateHandle& Handle)
{
	return OnSearchQueryUpdated.Remove(Handle);
}

FDelegateHandle USearchStore::AddSearchResultsUpdatedListener(const FSearchResultsUpdatedDelegate::FDelegate& Delegate)
{
	Delegate.ExecuteIfBound(SearchResults);
	return OnSearchResultsUpdated.Add(Delegate);
}

bool USearchStore::RemoveSearchResultsUpdatedListener(const FDelegateHandle& Handle)
{
	return OnSearchResultsUpdated.Remove(Handle);
}

FDelegateHandle USearchStore::AddRequestShowSearchResultsListener(const FSimpleMulticastDelegate::FDelegate& Delegate)
{
	Delegate.ExecuteIfBound();
	return OnRequestShowSearchResults.Add(Delegate);
}

bool USearchStore::RemoveRequestShowSearchResultsListener(const FDelegateHandle& Handle)
{
	return OnRequestShowSearchResults.Remove(Handle);
}

FDelegateHandle USearchStore::AddRequestClearSearchListener(const FSimpleMulticastDelegate::FDelegate& Delegate)
{
	Delegate.ExecuteIfBound();
	return OnRequestClearSearch.Add(Delegate);
}

bool USearchStore::RemoveRequestClearSearchListener(const FDelegateHandle& Handle)
{
	return OnRequestClearSearch.Remove(Handle);
}

void USearchStore::SetSearchQuery(const FString& NewSearchQuery)
{
	SearchQuery = NewSearchQuery;
	OnSearchQueryUpdated.Broadcast(SearchQuery);
}

void USearchStore::SetSearchResults(const TArray<UServerDataObject*>& NewSearchResults)
{
	SearchResults.Empty(NewSearchResults.Num());
	SearchResults = NewSearchResults;
	OnSearchResultsUpdated.Broadcast(SearchResults);
}

void USearchStore::SetCurrentPageNumber(const uint32 NewCurrentPage)
{
	CurrentPage = NewCurrentPage;
}

uint32 USearchStore::GetCurrentPageNumber() const
{
	return CurrentPage;
}

void USearchStore::SetMaxPages(const uint32 NewMaxPages)
{
	MaxPages = NewMaxPages;
}

uint32 USearchStore::GetMaxPages() const
{
	return MaxPages;
}

void USearchStore::ShowSearchResults()
{
	OnRequestShowSearchResults.Broadcast();
}

void USearchStore::ClearSearch()
{
	SetSearchQuery("");
	SetSearchResults({});
	OnRequestClearSearch.Broadcast();
}

