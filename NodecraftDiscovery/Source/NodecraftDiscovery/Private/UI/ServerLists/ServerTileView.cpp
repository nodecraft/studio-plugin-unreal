// Nodecraft, Inc. © 2012-2024, All Rights Reserved.


#include "UI/ServerLists/ServerTileView.h"

#include "UI/ServerLists/NoServerPlaceholder.h"

UUserWidget& UServerTileView::OnGenerateEntryWidgetInternal(UObject* Item, TSubclassOf<UUserWidget> DesiredEntryClass,
	const TSharedRef<STableViewBase>& OwnerTable)
{
	// If we are generating a widget for a placeholder, we want to stop navigation to the right from the previous item
	// Otherwise, we need to set the allow rule to Escape, otherwise if this is a recycled widget it will keep the old rule
	if (Item)
	{
		if (const int32 IndexForThisItem = GetIndexForItem(Item); IndexForThisItem > 0)
		{
			const int32 IndexForPreviousItem = IndexForThisItem - 1;
			if (UUserWidget* PrevWidget = GetEntryWidgetFromItem(GetItemAt(IndexForPreviousItem)))
			{
				PrevWidget->SetNavigationRuleBase(EUINavigation::Right, Item->IsA<UNoServerPlaceholder>() ? EUINavigationRule::Stop : EUINavigationRule::Escape);
			}
		}
	}
	return Super::OnGenerateEntryWidgetInternal(Item, DesiredEntryClass, OwnerTable);
}
