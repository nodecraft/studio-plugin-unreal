// Nodecraft, Inc. © 2012-2024, All Rights Reserved.


#include "UI/ServerDetails/TabbedSections/ServerDetailsAllowedPlayersSection.h"

#include "CommonBorder.h"
#include "CommonListView.h"
#include "Components/VerticalBox.h"
#include "Services/AllowsService.h"
#include "UI/Common/NodecraftLoadGuard.h"
#include "Models/AllowsDataObject.h"
#include "Models/AllowStatus.h"
#include "Models/AllowsViewModel.h"
#include "Models/FriendDataObject.h"
#include "Models/InviteFriendViewModel.h"
#include "Services/FriendsService.h"
#include "Stores/AllowsStore.h"
#include "UI/Alerts/AlertMessage.h"
#include "UI/Common/IconTextLoadingButton.h"

DEFINE_LOG_CATEGORY_STATIC(LogServerDetails, All, All);

void UServerDetailsAllowedPlayersSection::LoadAllowsFromServer()
{
	if (ServerDataObject)
	{
		FListPlayerAllowsResponseDelegate OnComplete;
		OnComplete.BindWeakLambda(this, [this](TArray<UAllowsDataObject*> Allows, bool bSuccess, TOptional<FText> Error)
		{
			if (bSuccess)
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
				AddFriendsPopupContainer->SetVisibility(ESlateVisibility::Collapsed);
			}
			else
			{
				AddFriendsButton->SetIsLoading(true);
				FGetFriendsListDelegate OnGetFriends;
				OnGetFriends.BindWeakLambda(
					this, [this](TArray<UFriendDataObject*> Friends, bool bSuccess, TOptional<FText> Error)
					{
						if (bSuccess)
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
											AddFriendsPopupContainer->SetVisibility(ESlateVisibility::Collapsed);
											AddFriendsButton->SetIsSelected(false);
										});
										UInviteFriendViewModel* ViewModel = UInviteFriendViewModel::FromDataObjects(
											ServerDataObject, Friend, OnInviteSent);
										FriendsWithoutPendingOrAcceptedAllow.Add(ViewModel);
									}
								}
								AddFriendsListView->SetListItems(FriendsWithoutPendingOrAcceptedAllow);
								AddFriendsPopupContainer->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
							}
							else
							{
								AddFriendsPopupContainer->SetVisibility(ESlateVisibility::Collapsed);
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

void UServerDetailsAllowedPlayersSection::NativeConstruct()
{
	Super::NativeConstruct();

	AllowsListenerHandle = UAllowsStore::Get().AddAllowsListener(
		FOnAllowsUpdated::FDelegate::CreateWeakLambda(this, [this](const TArray<UAllowsDataObject*> Allows)
	{
		RefreshAllowsList(Allows);
	}));

	ListView->ClearListItems();
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
}

void UServerDetailsAllowedPlayersSection::NativeOnDeactivated()
{
	Super::NativeOnDeactivated();
	ListView->ClearListItems();
	AddFriendsButton->OnClicked().Clear();
}

void UServerDetailsAllowedPlayersSection::RefreshAllowsList(const TArray<UAllowsDataObject*>& Allows)
{
	ListView->ClearListItems();
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
	ListView->SetListItems(AllowsViewModels);
}
