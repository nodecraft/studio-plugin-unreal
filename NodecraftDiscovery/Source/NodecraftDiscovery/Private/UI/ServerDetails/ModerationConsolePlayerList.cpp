// Nodecraft, Inc. © 2012-2024, All Rights Reserved.


#include "UI/ServerDetails/ModerationConsolePlayerList.h"
#include "UI/ServerDetails/ModerationConsolePlayerSelector.h"
#include "Input/CommonUIInputTypes.h"
#include "Input/CommonUIActionRouterBase.h"
#include "UI/ServerDetails/ServerModerationConsolePlayerListItem.h"

#define LOCTEXT_NAMESPACE "ModerationConsolePlayerList"

DEFINE_LOG_CATEGORY_STATIC(LogModerationConsolePlayerList, All, All);

TSubclassOf<UUserWidget> UModerationConsolePlayerList::GetDesiredEntryClassForItem(UObject* Item) const
{
	if (Item != nullptr)
	{
		if (Item->IsA<UPlayerServerDetailsViewModel>())
		{
			return PlayerWidgetClass;
		}
		else if (Item->IsA<UPlayerListSectionHeaderViewModel>())
		{
			return SectionHeaderWidgetClass;
		}
		else if (Item->IsA<UNoPlayersFoundItemData>())
		{
			return NoPlayersFoundWidgetClass;
		}
	}
	return Super::GetDesiredEntryClassForItem(Item);
}

void UModerationConsolePlayerList::SetFocusOnFirstPlayer()
{
	for (UUserWidget* DisplayedEntryWidget : GetDisplayedEntryWidgets())
	{
		if (DisplayedEntryWidget->IsA(PlayerWidgetClass))
		{
			DisplayedEntryWidget->SetFocus();
			break;
		}
	}
}

void UModerationConsolePlayerList::SelectAllPlayers()
{
	for (const UObject* ListItem : GetListItems())
	{
		if (const UPlayerServerDetailsViewModel* PlayerViewModel = Cast<UPlayerServerDetailsViewModel>(ListItem))
		{
			if (PlayerViewModel->bIsSelectable)
			{
				Cast<UServerModerationConsolePlayerListItem>(GetEntryWidgetFromItem(ListItem))->Select();
			}
		}
	}
}

void UModerationConsolePlayerList::ClearSelectedPlayers()
{
	for (const UObject* ListItem : GetListItems())
	{
		if (const UPlayerServerDetailsViewModel* PlayerViewModel = Cast<UPlayerServerDetailsViewModel>(ListItem))
		{
			if (PlayerViewModel->bIsSelectable)
			{
				if (UServerModerationConsolePlayerListItem* PlayerListItem = Cast<UServerModerationConsolePlayerListItem>(GetEntryWidgetFromItem(ListItem)))
				{
					PlayerListItem->Deselect();
				}
			}
		}
	}
}

void UModerationConsolePlayerList::UpdateActions() const
{
	if (SelectOrDeselectSelectedItemActionHandle.IsValid())
	{
		SelectOrDeselectSelectedItemActionHandle.Unregister();
	}
	
	// get the current selection
	volatile auto SelectedItem = GetSelectedItem();
	if (UPlayerServerDetailsViewModel* SelectedPlayer = Cast<UPlayerServerDetailsViewModel>(SelectedItem))
	{
		// Generally, it's the owner that isn't selectable.
		if (SelectedPlayer->bIsSelectable)
		{
			if (UCommonUIActionRouterBase* ActionRouter = UCommonUIActionRouterBase::Get(*this))
			{
				FBindUIActionArgs BindActionArgs = FBindUIActionArgs(SelectOrDeselectPlayerInputActionData, true, FSimpleDelegate::CreateWeakLambda(this, [this, SelectedPlayer]()
				{
					if (UServerModerationConsolePlayerListItem* PlayerListItem = Cast<
						UServerModerationConsolePlayerListItem>(GetEntryWidgetFromItem(SelectedPlayer)))
					{
						if (PlayerListItem->IsSelected())
						{
							PlayerListItem->Deselect();
						}
						else
						{
							PlayerListItem->Select();
						}
						PlayerListItem->SetFocus();
						UpdateActions();
					}
				}));

				if (UServerModerationConsolePlayerListItem* PlayerListItem = Cast<
						UServerModerationConsolePlayerListItem>(GetEntryWidgetFromItem(SelectedPlayer)))
				{
					if (PlayerListItem->IsSelected())
					{
						BindActionArgs.OverrideDisplayName = LOCTEXT("DeselectPlayer", "Deselect Player");
					}
					else
					{
						BindActionArgs.OverrideDisplayName = LOCTEXT("SelectPlayer", "Select Player");
					}
				}

				SelectOrDeselectSelectedItemActionHandle = ActionRouter->RegisterUIActionBinding(*this, BindActionArgs);
				ActionRouter->OnBoundActionsUpdated().Broadcast();
			}
		}
	}
}

ITypedUMGListView<UObject*>::FOnItemSelectionChanged& UModerationConsolePlayerList::OnItemSelectionChanged() const
{
	UpdateActions();
	return Super::OnItemSelectionChanged();
}

void UModerationConsolePlayerList::ListenForSelectedPlayersChange(FOnSelectedPlayersChanged& OnSelectedPlayersChanged)
{
	OnSelectedPlayersChanged.AddWeakLambda(this, [this](const TArray<UPlayerServerDetailsDataObject*>& InSelectedPlayers)
	{
		for (UObject* ListItem : GetListItems())
		{
			const UPlayerServerDetailsViewModel* VM = Cast<UPlayerServerDetailsViewModel>(ListItem);
			if (VM)
			{
				if (UServerModerationConsolePlayerListItem* PlayerListItem = Cast<UServerModerationConsolePlayerListItem>(GetEntryWidgetFromItem(ListItem)))
				{
					PlayerListItem->SetSelected(InSelectedPlayers.Contains(VM->GetPlayerServerDetails()));
				}
			}
		}
	});
}

void UModerationConsolePlayerList::SetupNavigation(const FGetFocusDestination& Delegate)
{
	GetRightNavFocusDestination = Delegate;
}

bool UModerationConsolePlayerList::OnIsSelectableOrNavigableInternal(UObject* FirstSelectedItem)
{
	return FirstSelectedItem != nullptr && FirstSelectedItem->IsA<UPlayerServerDetailsViewModel>();
}

void UModerationConsolePlayerList::SetOwner(UPlayerServerDetailsDataObject* InOwner)
{
	Owner = InOwner;
	RefreshList();
}

UUserWidget& UModerationConsolePlayerList::OnGenerateEntryWidgetInternal(UObject* Item,
	TSubclassOf<UUserWidget> DesiredEntryClass, const TSharedRef<STableViewBase>& OwnerTable)
{
	UUserWidget& EntryWidget = Super::OnGenerateEntryWidgetInternal(Item, DesiredEntryClass, OwnerTable);

	if (UServerModerationConsolePlayerListItem* PlayerListItem = Cast<UServerModerationConsolePlayerListItem>(&EntryWidget))
	{
		PlayerListItem->OnNavDelegate.BindWeakLambda(this, [this, Item](const FGeometry& MyGeometry,
		                                                                const FNavigationEvent& InNavigationEvent, const FNavigationReply& InDefaultReply)
		{
			// Are we the second item? Since the first one is a section header
			if (Item == GetItemAt(1))
			{
				if (InNavigationEvent.GetNavigationType() == EUINavigation::Up)
				{
					// don't let us move up
					return FNavigationReply::Stop();
				}
			}
			// check if we are the last item
			else if (Item == GetItemAt(GetNumItems() - 1))
			{
				// don't let us move down
				if (InNavigationEvent.GetNavigationType() == EUINavigation::Down)
				{
					return FNavigationReply::Stop();
				}
			}

			// In all other cases, we want to stop navigation if we're moving left, and we want to force us to move to the
			// panel on the right if we have something selected, otherwise stop.
			if (InNavigationEvent.GetNavigationType() == EUINavigation::Left)
			{
				return FNavigationReply::Stop();
			}

			if (InNavigationEvent.GetNavigationType() == EUINavigation::Right)
			{
				if (AreAnyPlayersSelectedDelegate.Execute())
				{
					// We have selected players, so we want to move to the right
					UWidget* FocusDestination = GetRightNavFocusDestination.Execute();
					// We have a destination that we know is outside our list, so we want to remove all actions at this point
					SelectOrDeselectSelectedItemActionHandle.Unregister();
					return FNavigationReply::Explicit(FocusDestination->GetCachedWidget());
				}
				else
				{
					return FNavigationReply::Stop();
				}
			}
			
			return InDefaultReply;
		});
	}
	
	return EntryWidget;
}

void UModerationConsolePlayerList::SetModerators(const TArray<UPlayerServerDetailsDataObject*>& InModerators)
{
	Moderators = InModerators;
	RefreshList();
}

void UModerationConsolePlayerList::SetOnlinePlayers(const TArray<UPlayerServerDetailsDataObject*>& InOnlinePlayers)
{
	OnlinePlayers = InOnlinePlayers;
	RefreshList();
}

void UModerationConsolePlayerList::SetOfflinePlayers(const TArray<UPlayerServerDetailsDataObject*>& InOfflinePlayers)
{
	OfflinePlayers = InOfflinePlayers;
	RefreshList();
}

void UModerationConsolePlayerList::SetDisplayMode(EPlayerSelectorMode Mode)
{
	DisplayMode = Mode;
	RefreshList();
}

UWidget* UModerationConsolePlayerList::GetFirstFocusableItem() const
{
	// Should always be the case, but defensive
	// Index 1 will be the Owner / first player entry
	if (GetNumItems() > 1)
	{
		return GetEntryWidgetFromItem(GetItemAt(1));
	}
	else
	{
		return nullptr;
	}
}

void UModerationConsolePlayerList::SetBannedPlayers(const TArray<UPlayerServerDetailsDataObject*>& InBannedPlayers)
{
	BannedPlayers = InBannedPlayers;
	RefreshList();
}

void UModerationConsolePlayerList::RefreshList()
{
	// TODO: Preserve checked status somehow
	
	//Go through, section by section, creating the section header item and then adding the items for that section

	ClearListItems();

	if (DisplayMode == EPlayerSelectorMode::All || DisplayMode == EPlayerSelectorMode::Staff)
	{
		AddSectionHeader(LOCTEXT("OwnerHeader", "Owner"), PlayerRoleIcons.FindRef("Owner"));
		if (Owner)
		{
			UPlayerServerDetailsViewModel* PlayerViewModel = UPlayerServerDetailsViewModel::FromDataObject(Owner);
			PlayerViewModel->OnPlayerSelectionChanged = OnPlayerSelectionChanged;
			PlayerViewModel->OnPlayerSelectedExclusive = OnPlayerSelectedExclusive;
			PlayerViewModel->OnPlayerItemReceivedFocus = OnPlayerItemReceivedFocus;
			PlayerViewModel->bIsSelectable = false;
			AddItem(PlayerViewModel);
		}
		else
		{
			UNoPlayersFoundItemData* NoOwner = UNoPlayersFoundItemData::Create(EModerationConsolePlayerListType::Owner);
			AddItem(NoOwner);
		}

		AddSectionHeader(LOCTEXT("ModeratorsHeader", "Moderators"), PlayerRoleIcons.FindRef("Moderator"));
		AddPlayersToList(Moderators, true, TOptional(EModerationConsolePlayerListType::Moderators));
	}

	if (DisplayMode == EPlayerSelectorMode::All || DisplayMode == EPlayerSelectorMode::Players)
	{
		AddSectionHeader(LOCTEXT("OnlinePlayersHeader", "Online Players"), PlayerRoleIcons.FindRef("Online"));
		AddPlayersToList(OnlinePlayers, true, TOptional(EModerationConsolePlayerListType::OnlinePlayers));

		AddSectionHeader(LOCTEXT("OfflinePlayersHeader", "Offline Players"), PlayerRoleIcons.FindRef("Offline"));
		AddPlayersToList(OfflinePlayers, true, TOptional(EModerationConsolePlayerListType::OfflinePlayers));
	}
	
	if (DisplayMode == EPlayerSelectorMode::All || DisplayMode == EPlayerSelectorMode::Banned)
	{
		AddSectionHeader(LOCTEXT("BannedPlayersHeader", "Banned Players"), PlayerRoleIcons.FindRef("Banned"));
		AddPlayersToList(BannedPlayers, true, TOptional(EModerationConsolePlayerListType::BannedPlayers));
	}
}

void UModerationConsolePlayerList::AddSectionHeader(const FText& HeaderText, const FModerationConsoleHeaderIcon& IconData)
{
	UPlayerListSectionHeaderViewModel* HeaderVm = NewObject<UPlayerListSectionHeaderViewModel>();
	HeaderVm->HeaderText = HeaderText;
	HeaderVm->IconData = IconData;
	AddItem(HeaderVm);
}

void UModerationConsolePlayerList::AddPlayersToList(const TArray<UPlayerServerDetailsDataObject*>& Players, const bool bAreChildrenSelectable,
                                                    const TOptional<EModerationConsolePlayerListType>& PlayerListType)
{
	TArray<UPlayerServerDetailsViewModel*> PlayerViewModels = {};

	if (Players.Num() > 0)
	{
		for (UPlayerServerDetailsDataObject* Player : Players)
		{
			UPlayerServerDetailsViewModel* PlayerViewModel = UPlayerServerDetailsViewModel::FromDataObject(Player);
			PlayerViewModel->OnPlayerSelectionChanged = OnPlayerSelectionChanged;
			PlayerViewModel->OnPlayerSelectedExclusive = OnPlayerSelectedExclusive;
			PlayerViewModel->OnPlayerItemReceivedFocus = OnPlayerItemReceivedFocus;
			PlayerViewModel->bIsSelectable = bAreChildrenSelectable;
			PlayerViewModels.Add(PlayerViewModel);
			AddItem(PlayerViewModel);
		}
	}
	else
	{
		UNoPlayersFoundItemData* NoPlayers = UNoPlayersFoundItemData::Create(PlayerListType.Get(EModerationConsolePlayerListType::None));
		AddItem(NoPlayers);
	}
}

#undef LOCTEXT_NAMESPACE
