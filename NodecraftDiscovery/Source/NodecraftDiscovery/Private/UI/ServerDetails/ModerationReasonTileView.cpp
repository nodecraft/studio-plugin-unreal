// Nodecraft, Inc. © 2012-2024, All Rights Reserved.


#include "UI/ServerDetails/ModerationReasonTileView.h"

#include "UI/ServerDetails/ModerationReasonButton.h"
#include "UI/ServerDetails/ViewModels/ModerationReasonTileViewModel.h"

UUserWidget& UModerationReasonTileView::OnGenerateEntryWidgetInternal(UObject* Item,
                                                                      TSubclassOf<UUserWidget> DesiredEntryClass,
                                                                      const TSharedRef<STableViewBase>& OwnerTable)
{
	UUserWidget& Widget =  Super::OnGenerateEntryWidgetInternal(Item, DesiredEntryClass, OwnerTable);

	UModerationReasonTileViewModel* ViewModel = Cast<UModerationReasonTileViewModel>(Item);
	if (!ViewModel)
	{
		return Widget;
	}
	if (ViewModel->IsBottomRow())
	{
		Widget.SetNavigationRuleCustom(EUINavigation::Down, BottomRowDownNavigationDelegate);
	}

	// If we're an odd index, we don't want to ever be able to navigate to the right.
	if (ViewModel->GetDisplayIndex() % 2 == 1)
	{
		Widget.SetNavigationRuleBase(EUINavigation::Right, EUINavigationRule::Stop);
	}

	// If we're at an even index (left hand column) we want to delegate navigation so that we end up (as of writing) at
	// an item in the player list.
	if (ViewModel->GetDisplayIndex() % 2 == 0)
	{
		Widget.SetNavigationRuleCustom(EUINavigation::Left, LeftColumnNavLeftDelegate);
	}

	if (ViewModel->GetDisplayIndex() < 2)
	{
		Widget.SetNavigationRuleCustom(EUINavigation::Up, TopRowUpNavigationDelegate);
	}
	return Widget;
}
