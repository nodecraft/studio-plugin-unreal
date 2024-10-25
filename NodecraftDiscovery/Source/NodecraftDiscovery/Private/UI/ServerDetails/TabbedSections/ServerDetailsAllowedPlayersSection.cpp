// Nodecraft, Inc. © 2012-2024, All Rights Reserved.


#include "UI/ServerDetails/TabbedSections/ServerDetailsAllowedPlayersSection.h"

#include "CommonBorder.h"
#include "CommonListView.h"
#include "Components/VerticalBox.h"
#include "Input/CommonUIInputTypes.h"
#include "Services/AllowsService.h"
#include "UI/Common/NodecraftLoadGuard.h"
#include "Models/AllowsDataObject.h"
#include "Models/AllowStatus.h"
#include "Models/AllowsViewModel.h"
#include "Models/FriendDataObject.h"
#include "Models/InviteFriendViewModel.h"
#include "Services/FriendsService.h"
#include "Stores/AllowsStore.h"
#include "Stores/ServersStore.h"
#include "UI/Alerts/AlertMessage.h"
#include "UI/Common/IconTextLoadingButton.h"
#include "UI/ServerDetails/AddAllowedFriendListItem.h"
#include "UI/ServerDetails/AllowedPlayersListItem.h"
#include "UI/ServerDetails/AllowedPlayersListView.h"

DEFINE_LOG_CATEGORY_STATIC(LogServerDetails, All, All);

void UServerDetailsAllowedPlayersSection::LoadAllowsFromServer()
{
	if (ServerDataObject)
	{
		FListPlayerAllowsResponseDelegate OnComplete;
		OnComplete.BindWeakLambda(this, [this](TArray<UAllowsDataObject*> Allows, bool bSuccess, TOptional<FText> Error)
		{
			if (bSuccess && UServersStore::Get().GetCurrentServerId() == ServerDataObject->GetId())
			{
				RefreshAllowsList(Allows);
				LoadGuard->SetIsLoading(false);

				RefreshHeaderVisibility();

				if (Allows.Num() > 0)
				{
					OwnerEmptyListContainer->SetVisibility(ESlateVisibility::Collapsed);
					NonOwnerEmptyListContainer->SetVisibility(ESlateVisibility::Collapsed);
				}
				else
				{
					OwnerEmptyListContainer->SetVisibility(ServerDataObject->GetRole() == EPlayerRole::Owner
						                                       ? ESlateVisibility::SelfHitTestInvisible
						                                       : ESlateVisibility::Collapsed);
					NonOwnerEmptyListContainer->SetVisibility(ServerDataObject->GetRole() == EPlayerRole::Player
						                                          ? ESlateVisibility::SelfHitTestInvisible
						                                          : ESlateVisibility::Collapsed);
				}
			}
			else
			{
				Alert->Show(Error.GetValue(), EAlertType::Error);
			}
		});

		LoadGuard->SetIsLoading(true);
		UAllowsService::Get().ListAllowsByServer(ServerDataObject->GetId(), OnComplete);
	}
}

void UServerDetailsAllowedPlayersSection::RefreshHeaderVisibility()
{
	ServerOwnerHeader->SetVisibility(ServerDataObject->GetRole() == EPlayerRole::Owner
														 ? ESlateVisibility::SelfHitTestInvisible
														 : ESlateVisibility::Collapsed);
}

void UServerDetailsAllowedPlayersSection::SetAddFriendsPopupVisibility(const ESlateVisibility InVisibility)
{
	AddFriendsPopupContainer->SetVisibility(InVisibility);

	if (InVisibility == ESlateVisibility::Collapsed || InVisibility == ESlateVisibility::Hidden)
	{
		RegisterAddFriendsUIAction();

		if (UAllowsViewModel* SelectedItem = AllAllowsListView->GetSelectedItem<UAllowsViewModel>())
		{
			AllAllowsListView->RequestNavigateToItem(SelectedItem);
		}
		else if (AllAllowsListView->GetDisplayedEntryWidgets().IsValidIndex(0))
		{
			AllAllowsListView->NavigateToIndex(0);
		}
	}
	else
	{
		if (AddFriendsUIActionHandle.IsValid())
		{
			AddFriendsUIActionHandle.Unregister();
		}
		
		if (AddFriendsListView->GetDisplayedEntryWidgets().IsValidIndex(0))
		{
			AddFriendsListView->NavigateToIndex(0);
		}
		else
		{
			 OnAddFriendsListWidgetGeneratedHandle = AddFriendsListView->OnEntryWidgetGenerated().AddWeakLambda(
				this, [&](UWidget& InEntryWidget) mutable
				{
					if (AddFriendsListView->GetDisplayedEntryWidgets().IsValidIndex(0) && OnAddFriendsListWidgetGeneratedHandle.IsValid())
					{
						AddFriendsListView->NavigateToIndex(0);
						OnAddFriendsListWidgetGeneratedHandle.Reset();
					}
				});
		}
	}
}

void UServerDetailsAllowedPlayersSection::SetServerData(UServerDataObject* InServerDataObject)
{
	if (InServerDataObject == nullptr)
    {
    	UE_LOG(LogServerDetails, Error, TEXT("UServerDetailsAllowedPlayersSection::SetServerData(): ServerData is null!"));
    	return;
    }

	const bool bIsSameServer = ServerDataObject && ServerDataObject->GetId() == InServerDataObject->GetId();

	FriendsWithPendingOrAcceptedAllow.Empty();
	ServerDataObject = InServerDataObject;

	// If we are just setting the same server, probably this is a version of the server
	// with more information, like Player Role for example, in which case we just want
	// to refresh the header visibility.
	// Otherwise, we need to load the allows from the server.
	if (bIsSameServer)
	{
		RefreshHeaderVisibility();
	}
	else
	{
		AddFriendsButton->OnClicked().Clear();
		AddFriendsButton->OnClicked().AddWeakLambda(this, [this]
		{
			if (AddFriendsPopupContainer->IsVisible())
			{
				SetAddFriendsPopupVisibility(ESlateVisibility::Collapsed);
			}
			else
			{
				AddFriendsButton->SetIsLoading(true);
				FGetFriendsListDelegate OnGetFriends;
				OnGetFriends.BindWeakLambda(
					this, [this](TArray<UFriendDataObject*> Friends, bool bSuccess, TOptional<FText> Error)
					{
						if (bSuccess && UServersStore::Get().GetCurrentServerId() == ServerDataObject->GetId())
						{
							if (Friends.Num() > 0)
							{
								TArray<UInviteFriendViewModel*> FriendsWithoutPendingOrAcceptedAllow;
								for (const UFriendDataObject* Friend : Friends)
								{
									if (FriendsWithPendingOrAcceptedAllow.Contains(Friend->GetPlayer()->GetId()) == false)
									{
										FSimpleDelegate OnInviteSent;
										OnInviteSent.BindWeakLambda(this, [this]
										{
											SetAddFriendsPopupVisibility(ESlateVisibility::Collapsed);
											AddFriendsButton->SetIsSelected(false);
										});
										UInviteFriendViewModel* ViewModel = UInviteFriendViewModel::FromDataObjects(
											ServerDataObject, Friend, OnInviteSent);
										FriendsWithoutPendingOrAcceptedAllow.Add(ViewModel);
									}
								}
								AddFriendsListView->SetListItems(FriendsWithoutPendingOrAcceptedAllow);
								if (FriendsWithoutPendingOrAcceptedAllow.Num() > 0)
								{
									SetAddFriendsPopupVisibility(ESlateVisibility::SelfHitTestInvisible);
								}
							}
							else
							{
								SetAddFriendsPopupVisibility(ESlateVisibility::Collapsed);
							}
						}
						AddFriendsButton->SetIsLoading(false);
					});
				UFriendsService::Get().GetFriends(OnGetFriends);
			}
		});

		LoadAllowsFromServer();
	}
	
	
}

bool UServerDetailsAllowedPlayersSection::ShouldProcessBackAction() const
{
	return AddFriendsPopupContainer->IsVisible();
}

bool UServerDetailsAllowedPlayersSection::ProcessBackAction()
{
	if (ShouldProcessBackAction())
	{
		SetAddFriendsPopupVisibility(ESlateVisibility::Collapsed);
		return true;
	}
	return false;
}

void UServerDetailsAllowedPlayersSection::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	AllAllowsListView->SetSelectionMode(ESelectionMode::Single);
	
	AddFriendsListView->OnEntryWidgetGenerated().AddWeakLambda(this, [this](UWidget& InEntryWidget)
	{
		if (UAddAllowedFriendListItem* ListItem = Cast<UAddAllowedFriendListItem>(&InEntryWidget))
		{
			ListItem->OnNavDelegate.BindWeakLambda(this, [this, &InEntryWidget](const FGeometry& MyGeometry, const FNavigationEvent& InNavigationEvent, const FNavigationReply& InDefaultReply)
			{
				if (InNavigationEvent.GetNavigationGenesis() == ENavigationGenesis::Controller)
				{
					const UUserWidget* FirstItem = AddFriendsListView->GetEntryWidgetFromItem(AddFriendsListView->GetItemAt(0));
					const UUserWidget* LastItem = AddFriendsListView->GetEntryWidgetFromItem(AddFriendsListView->GetItemAt(AddFriendsListView->GetNumItems() - 1));
					if (InNavigationEvent.GetNavigationType() == EUINavigation::Up && &InEntryWidget == FirstItem)
					{
						return FNavigationReply::Stop();
					}
					if (InNavigationEvent.GetNavigationType() == EUINavigation::Down && &InEntryWidget == LastItem)
					{
						return FNavigationReply::Stop();
					}
					if (InNavigationEvent.GetNavigationType() == EUINavigation::Left || InNavigationEvent.GetNavigationType() == EUINavigation::Right
						|| InNavigationEvent.GetNavigationType() == EUINavigation::Next || InNavigationEvent.GetNavigationType() == EUINavigation::Previous)
					{
						return FNavigationReply::Stop();
					}
				}

				return InDefaultReply;
			});
		}
	});

	RegisterAddFriendsUIAction();
}

void UServerDetailsAllowedPlayersSection::NativeConstruct()
{
	Super::NativeConstruct();

	AllowsListenerHandle = UAllowsStore::Get().AddAllowsListener(
		FOnAllowsUpdated::FDelegate::CreateWeakLambda(this, [this](const TArray<UAllowsDataObject*> Allows)
	{
		RefreshAllowsList(Allows);
	}));

	AllAllowsListView->ClearListItems();
	Alert->Hide();
}

void UServerDetailsAllowedPlayersSection::NativeDestruct()
{
	Super::NativeDestruct();
	UAllowsStore::Get().RemoveAllowsListener(AllowsListenerHandle);
}

void UServerDetailsAllowedPlayersSection::NativeOnActivated()
{
	Super::NativeOnActivated();
	// If we have a server data object, that means we navigated away and are now coming back.
	// We should reload the allows from the server.
	if (ServerDataObject)
	{
		LoadAllowsFromServer();
	}

	AllAllowsListView->OnEntryWidgetGenerated().AddWeakLambda(this, [this](UWidget& EntryWidget)
	{
		EntryWidget.SetFocus();
		AllAllowsListView->OnEntryWidgetGenerated().RemoveAll(this);
	});
}

void UServerDetailsAllowedPlayersSection::NativeOnDeactivated()
{
	Super::NativeOnDeactivated();
	AllAllowsListView->ClearListItems();
}

void UServerDetailsAllowedPlayersSection::RefreshAllowsList(const TArray<UAllowsDataObject*>& Allows)
{
	AllAllowsListView->ClearListItems();
	FriendsWithPendingOrAcceptedAllow.Empty();

	TArray<UAllowsViewModel*> AllowsViewModels;
	for (const UAllowsDataObject* Allow : Allows)
	{
		if (Allow->GetStatus() == EAllowStatus::Pending || Allow->GetStatus() == EAllowStatus::Accepted)
		{
			FriendsWithPendingOrAcceptedAllow.AddUnique(Allow->GetReceiver()->GetId());
		}
		UAllowsViewModel* AllowsViewModel = UAllowsViewModel::FromDataObjects(Allow, ServerDataObject);
		AllowsViewModels.Add(AllowsViewModel);
	}
	AllAllowsListView->SetListItems(AllowsViewModels);
}

void UServerDetailsAllowedPlayersSection::RegisterAddFriendsUIAction()
{
	if (AddFriendsInputActionData.IsNull() || AddFriendsUIActionHandle.IsValid())
	{
		return;
	}
	
	FBindUIActionArgs BindUIActionArgs = FBindUIActionArgs(AddFriendsInputActionData, true,
		FSimpleDelegate::CreateWeakLambda(this, [this]
		{
			AddFriendsButton->OnClicked().Broadcast();
		}));
	AddFriendsUIActionHandle = RegisterUIActionBinding(BindUIActionArgs);
}
