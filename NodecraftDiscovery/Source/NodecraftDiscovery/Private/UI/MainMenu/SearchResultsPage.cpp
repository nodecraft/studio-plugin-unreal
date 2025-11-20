// Nodecraft, Inc. © 2012-2024, All Rights Reserved.


#include "UI/MainMenu/SearchResultsPage.h"

#include "CommonTextBlock.h"
#include "CommonTileView.h"
#include "Components/HorizontalBox.h"
#include "Components/VerticalBox.h"
#include "Stores/SearchStore.h"
#include "Models/ServerDataObject.h"
#include "Services/ServersService.h"
#include "UI/Common/NodecraftLoadGuard.h"
#include "UI/Foundation/NodecraftButtonBase.h"
#include "UI/Foundation/NodecraftLoadingButton.h"

void USearchResultsPage::NativeConstruct()
{
	Super::NativeConstruct();
	
	USearchStore& SearchStore = USearchStore::Get();
	ClearSearchButton->OnClicked().AddWeakLambda(this, [this, &SearchStore]
	{
		SearchStore.ClearSearch();
	});
	
	OnSearchQueryUpdated = SearchStore.AddSearchQueryUpdatedListener(
		FSearchQueryUpdatedDelegate::FDelegate::CreateWeakLambda(this, [this](const FString& SearchQuery)
	{
		SearchQueryText->SetText(FText::FromString(SearchQuery));
	}));

	OnSearchStarted = UServersService::Get().AddSearchStartedListener(FSimpleMulticastDelegate::FDelegate::CreateWeakLambda(
		this, [this]
	{
		SearchResultsLoadGuard->SetIsLoading(true);
		PaginationButtonsContainer->SetVisibility(ESlateVisibility::Collapsed);
		ClearSearchButton->SetVisibility(ESlateVisibility::Collapsed);
		NoSearchResultsContainer->SetVisibility(ESlateVisibility::Collapsed);
	}));

	OnSearchResultsUpdated = SearchStore.AddSearchResultsUpdatedListener(
		FSearchResultsUpdatedDelegate::FDelegate::CreateWeakLambda(this, [this, &SearchStore]
			(const TArray<UServerDataObject*>& InSearchResults)
	{
		SearchResultsLoadGuard->SetIsLoading(false);
		PreviousPageButton->SetIsLoading(false);
		NextPageButton->SetIsLoading(false);
		if (InSearchResults.Num() > 0)
		{
			SearchResults->SetListItems(InSearchResults);
			PaginationButtonsContainer->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			ClearSearchButton->SetVisibility(ESlateVisibility::Visible);
			NoSearchResultsContainer->SetVisibility(ESlateVisibility::Collapsed);
			CurrentPage = SearchStore.GetCurrentPageNumber();
			PreviousPageButton->SetIsEnabled(CurrentPage > 1);
			NextPageButton->SetIsEnabled(CurrentPage < SearchStore.GetMaxPages());
		}
		else
		{
			SearchResults->ClearListItems();
			PaginationButtonsContainer->SetVisibility(ESlateVisibility::Collapsed);
			NoSearchResultsContainer->SetVisibility(ESlateVisibility::HitTestInvisible);
			ClearSearchButton->SetVisibility(ESlateVisibility::Visible);
		}
	}));

	NextPageButton->OnClicked().AddWeakLambda(this, [this]
	{
		NextPageButton->SetIsLoading(true);
		PreviousPageButton->SetIsEnabled(false);
		UServersService::Get().SearchServers(GetWorld(), SearchQueryText->GetText().ToString(), CurrentPage + 1);
	});

	PreviousPageButton->OnClicked().AddWeakLambda(this, [this]
	{
		PreviousPageButton->SetIsLoading(true);
		NextPageButton->SetIsEnabled(false);
		UServersService::Get().SearchServers(GetWorld(), SearchQueryText->GetText().ToString(), CurrentPage - 1);
	});
}

void USearchResultsPage::NativeDestruct()
{
	Super::NativeDestruct();

	USearchStore& SearchStore = USearchStore::Get();
	SearchStore.RemoveSearchQueryUpdatedListener(OnSearchQueryUpdated);
	SearchStore.RemoveSearchResultsUpdatedListener(OnSearchResultsUpdated);
	UServersService::Get().RemoveSearchStartedListener(OnSearchStarted);
}
