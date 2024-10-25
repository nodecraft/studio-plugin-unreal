// Nodecraft, Inc. © 2012-2024, All Rights Reserved.


#include "UI/ServerLists/ServerListViewModel.h"

#include "Stores/ServersStore.h"

EServerListButtonState UServerListViewModel::GetNextButtonState()
{
	return NextButtonState;
}

EServerListButtonState UServerListViewModel::GetPrevButtonState()
{
	return PrevButtonState;
}

bool UServerListViewModel::IsLoading()
{
	return bIsLoading;
}

TArray<UServerDataObject*> UServerListViewModel::GetServers()
{
	return Servers;
}

UServerDataObject* UServerListViewModel::GetPreviousServerPreview()
{
	return PreviousServerPreview;
}

UServerDataObject* UServerListViewModel::GetNextServerPreview()
{
	return NextServerPreview;
}

void UServerListViewModel::GoToNextPage(UWorld* World)
{
	UServersStore::Get().GetServerListViewPage(World, ListType, CurrentViewPage + 1,
	                                           FOnDisplayServerListPage::CreateUObject(
		                                           this, &UServerListViewModel::DisplayPage));
}

void UServerListViewModel::GoToPrevPage(UWorld* World)
{
	UServersStore::Get().GetServerListViewPage(World, ListType, CurrentViewPage - 1,
											   FOnDisplayServerListPage::CreateUObject(
												   this, &UServerListViewModel::DisplayPage));
}

void UServerListViewModel::SetListType(EServerListType InListType)
{
	ListType = InListType;
}

bool UServerListViewModel::ShouldShowCreateServerButton() const
{
	return bShouldShowCreateServerButton;
}

void UServerListViewModel::RefreshPage(UWorld* World)
{
	UServersStore::Get().GetServerListViewPage(World, ListType, CurrentViewPage,
											   FOnDisplayServerListPage::CreateUObject(
												   this, &UServerListViewModel::DisplayPage));
}

bool UServerListViewModel::ShouldHideList() const
{
	return bShouldHideList;
}

void UServerListViewModel::DisplayPage(FServerListPage ServerListPage)
{
	CurrentViewPage = ServerListPage.ViewPageNumber;
	Servers = ServerListPage.Servers;
	PreviousServerPreview = ServerListPage.PreviousServerPreview;
	NextServerPreview = ServerListPage.NextServerPreview;

	bShouldShowCreateServerButton = ServerListPage.bPrependCreateServerButton;

	bShouldHideList = ServerListPage.bShouldHideList;

	NextButtonState = ServerListPage.bIsLoadingNextPage ? EServerListButtonState::Loading : EServerListButtonState::Default;
	if (ServerListPage.bIsLoadingNextPage)
	{
		NextButtonState = EServerListButtonState::Loading;
	}
	else if (ServerListPage.bIsLastViewPage)
	{
		NextButtonState = EServerListButtonState::Disabled;
	}
	else
	{
		NextButtonState = EServerListButtonState::Default;
	}

	if (ServerListPage.bIsLoadingPrevPage)
	{
		PrevButtonState = EServerListButtonState::Loading;
	}
	else if (ServerListPage.ViewPageNumber == 1)
	{
		PrevButtonState = EServerListButtonState::Disabled;
	}
	else
	{
		PrevButtonState = EServerListButtonState::Default;
	}
	
	UServersStore::Get().OnLoadedViewPage.AddWeakLambda(this, [this](EServerListType ServerListType, int32 ViewPage, const FServerListPage& LoadedPage)
	{
		if (ServerListType == ListType && ViewPage == CurrentViewPage)
		{
			DisplayPage(LoadedPage);
		}
	});

	OnPageChanged.ExecuteIfBound();

}
