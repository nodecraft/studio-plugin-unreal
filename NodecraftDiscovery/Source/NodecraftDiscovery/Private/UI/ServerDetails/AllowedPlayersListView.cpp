// Nodecraft, Inc. © 2012-2024, All Rights Reserved.


#include "UI/ServerDetails/AllowedPlayersListView.h"

#include "UI/ServerDetails/AllowedPlayersListItem.h"

UUserWidget& UAllowedPlayersListView::OnGenerateEntryWidgetInternal(UObject* Item, TSubclassOf<UUserWidget> DesiredEntryClass, const TSharedRef<STableViewBase>& OwnerTable)
{
	UUserWidget& EntryWidget = Super::OnGenerateEntryWidgetInternal(Item, DesiredEntryClass, OwnerTable);

	if (UAllowedPlayersListItem* ListItem = Cast<UAllowedPlayersListItem>(&EntryWidget))
	{
		ListItem->OnNavDelegate.BindWeakLambda(this, [this, &EntryWidget](const FGeometry& MyGeometry, const FNavigationEvent& InNavigationEvent, const FNavigationReply& InDefaultReply)
		{
			if (InNavigationEvent.GetNavigationGenesis() == ENavigationGenesis::Controller)
			{
				const UUserWidget* FirstItem = GetEntryWidgetFromItem(GetItemAt(0));
				const UUserWidget* LastItem = GetEntryWidgetFromItem(GetItemAt(GetNumItems() - 1));
					
				if (InNavigationEvent.GetNavigationType() == EUINavigation::Up && &EntryWidget == FirstItem)
				{
					return FNavigationReply::Stop();
				}
				if (InNavigationEvent.GetNavigationType() == EUINavigation::Down && &EntryWidget == LastItem)
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
	
	return EntryWidget;
}
