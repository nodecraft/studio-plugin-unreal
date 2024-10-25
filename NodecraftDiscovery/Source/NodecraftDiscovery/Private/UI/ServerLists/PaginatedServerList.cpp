// Nodecraft, Inc. © 2012-2024, All Rights Reserved.


#include "UI/ServerLists/PaginatedServerList.h"

#include "Stores/ServersStore.h"
#include "Subsystems/NodecraftUIManagerSubsystem.h"
#include "UI/ServerLists/CreateServerPlaceholder.h"
#include "UI/ServerLists/LoadingServersPlaceholder.h"
#include "UI/ServerLists/NoServerPlaceholder.h"
#include "UI/ServerLists/PaginatedServerListNavButton.h"
#include "UI/ServerLists/ServerCardBase.h"
#include "UI/ServerLists/ServerListViewModel.h"

FReply UPaginatedServerList::NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent)
{
	if (UUserWidget* FirstWidget = GetFirstWidgetInList())
	{
		FReply Reply = FReply::Handled();
		Reply.SetNavigation(FirstWidget->TakeWidget(), ENavigationGenesis::User, ENavigationSource::FocusedWidget);
		return Reply;
	}
	else
	{
		return Super::NativeOnFocusReceived(InGeometry, InFocusEvent);
	}
}

void UPaginatedServerList::ForwardNavigationDelegate(UWidget& EntryWidget)
{
	if (UServerCardBase* ServerCard = Cast<UServerCardBase>(&EntryWidget))
	{
		ServerCard->OnNavDelegate.BindWeakLambda(this, [this](const FGeometry& MyGeometry, const FNavigationEvent& InNavigationEvent, const FNavigationReply& InDefaultReply)
		{
			return ListNavDelegate.Execute(MyGeometry, InNavigationEvent, InDefaultReply);
		});
	}
}

void UPaginatedServerList::NativeConstruct()
{
	Super::NativeConstruct();

	ViewModel = NewObject<UServerListViewModel>();

	ViewModel->SetListType(ServerListType);

	NextButton->OnClicked().AddWeakLambda(this, [this]()
	{
		ViewModel->GoToNextPage(GetWorld());
	});

	PrevButton->OnClicked().AddWeakLambda(this, [this]()
	{
		ViewModel->GoToPrevPage(GetWorld());
	});

	// TODO: Move this into its own function
	ViewModel->OnPageChanged.BindWeakLambda(this, [this]()
	{
		ServerListView->ClearListItems();
		// Hide the loading placeholders and show the server list
		LoadingServersListView->SetVisibility(ESlateVisibility::Collapsed);
		ServerListView->SetVisibility(ESlateVisibility::Visible);

		// Set the visibility of the entire widget based on our view model
		SetVisibility(ViewModel->ShouldHideList() ? ESlateVisibility::Collapsed : ESlateVisibility::Visible);
		
		// Set the server list view to display the servers
		TArray<UObject*> ServerListItems;
		

		if (ViewModel->ShouldShowCreateServerButton())
		{
			ServerListItems.Add(NewObject<UCreateServerPlaceholder>());
		}
		
		ServerListItems.Append(ViewModel->GetServers());
		// For any remaining slots, add a placeholder
		if (const UWorld* World = GetWorld())
		{
			if (const UGameInstance* GameInstance = World->GetGameInstance())
			{
				if (UNodecraftUIManagerSubsystem* UIManager = GameInstance->GetSubsystem<UNodecraftUIManagerSubsystem>())
				{
					for (int i = ServerListItems.Num(); i < UIManager->GetNumTilesInServerList(); i++)
					{
						ServerListItems.Add(NewObject<UNoServerPlaceholder>());
					}
				}
			}
		}

		ServerListView->OnEntryWidgetGenerated().AddWeakLambda(this, [this](UWidget& EntryWidget)
		{
			if (UServerCardBase* ServerCard = Cast<UServerCardBase>(&EntryWidget)) {
				ServerCard->OnFocusedDelegate = OnServerCardFocused;
			}
			ForwardNavigationDelegate(EntryWidget);
		});
		
		ServerListView->SetListItems(ServerListItems);

		// Set the next and previous server previews
		NextPreviewCard->SetServerData(ViewModel->GetNextServerPreview());
		PreviousPreviewCard->SetServerData(ViewModel->GetPreviousServerPreview());

		NextButton->SetButtonState(ViewModel->GetNextButtonState());
		PrevButton->SetButtonState(ViewModel->GetPrevButtonState());
	});

	UServersStore::Get().OnFavoriteServersUpdated.AddWeakLambda(
		this, [this](UServerDataObject* UpdatedServer, const TArray<UServerDataObject*>& FavoriteServers)
	{
			ViewModel->RefreshPage(GetWorld());
	});

	UServersStore::Get().GetServerListViewPage(GetWorld(), ServerListType, 1,
		FOnDisplayServerListPage::CreateWeakLambda(this, [this](const FServerListPage& PageInfo)
	{
		ViewModel->DisplayPage(PageInfo);
	}));

	// To start with, hide the server list and show the loading placeholders
	ServerListView->SetVisibility(ESlateVisibility::Collapsed);
	// Hide the next and previous server previews
	NextPreviewCard->SetVisibility(ESlateVisibility::Collapsed);
	PreviousPreviewCard->SetVisibility(ESlateVisibility::Collapsed);
	// Disable the prev / next buttons
	NextButton->SetIsEnabled(false);
	PrevButton->SetIsEnabled(false);
	
	// We need to create a TArray of UObject pointers to pass to the SetListItems function,
	// because nullptrs wont work.
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

	ServerListView->OnGetEntryClassForItem().BindWeakLambda(this, [this](UObject* Item)
	{
		if (Item->IsA<UNoServerPlaceholder>())
		{
			return NoServerWidgetClass;
		}

		if (Item->IsA<UCreateServerPlaceholder>())
		{
			return CreateServerButtonWidgetClass;
		}

		return ServerListView->GetDefaultEntryClass();
	});
}

UUserWidget* UPaginatedServerList::GetFirstWidgetInList()
{
	if (const UObject* FirstItem = ServerListView->GetItemAt(0))
	{
		if (UUserWidget* FirstWidget = ServerListView->GetEntryWidgetFromItem(FirstItem))
		{
			return FirstWidget;
		}
	}
	return nullptr;
}

void UPaginatedServerList::MoveFocusToLastWidgetInList()
{
	TArray<UUserWidget*> DisplayedWidgets = ServerListView->GetDisplayedEntryWidgets();

	for (int32 j = DisplayedWidgets.Num() - 1; j >= 0; j--)
	{
		if (UWidget* Widget = DisplayedWidgets[j])
		{
			Widget->SetFocus();
			return;
		}
	}
}
