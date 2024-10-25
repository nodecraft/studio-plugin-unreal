// Nodecraft, Inc. © 2012-2024, All Rights Reserved.


#include "UI/SocialLinks/SocialLinksList.h"

#include "CommonListView.h"
#include "CommonTextBlock.h"
#include "Components/Image.h"
#include "Models/SocialLinkDataObject.h"
#include "UI/SocialLinks/SocialLinkItem.h"


void USocialLinksList::ShowLinksVisuals(bool bShowLinks)
{
	NoLinksImage->SetVisibility(bShowLinks ? ESlateVisibility::Collapsed : ESlateVisibility::HitTestInvisible);
	NoLinksText->SetVisibility(bShowLinks ? ESlateVisibility::Collapsed : ESlateVisibility::HitTestInvisible);
}

void USocialLinksList::SetListItems(const TArray<USocialLinkDataObject*>& ListItems)
{
	ListView->ClearListItems();
	ListView->SetListItems(ListItems);
	ShowLinksVisuals(ListItems.Num() > 0);
}

UListViewBase::FOnListEntryGenerated& USocialLinksList::OnEntryWidgetGenerated()
{
	return ListView->OnEntryWidgetGenerated();
}

void USocialLinksList::NativeConstruct()
{
	Super::NativeConstruct();

	ListView->OnEntryWidgetGenerated().AddWeakLambda(this, [this](UWidget& EntryWidget)
	{
		// If we're using the controller and we're either moving up from the first item or down from the last item, we want to do something different than usual
		// Cast entry widget to social link item
		if (USocialLinkItem* SocialLinkItem = Cast<USocialLinkItem>(&EntryWidget))
		{
			SocialLinkItem->OnNavDelegate.BindWeakLambda(this, [this, &EntryWidget](const FGeometry& MyGeometry, const FNavigationEvent& InNavigationEvent, const FNavigationReply& InDefaultReply)
			{
				if (InNavigationEvent.GetNavigationGenesis() == ENavigationGenesis::Controller)
				{
					const auto FirstItem = ListView->GetEntryWidgetFromItem(ListView->GetItemAt(0));
					const auto LastItem = ListView->GetEntryWidgetFromItem(ListView->GetItemAt(ListView->GetNumItems() - 1));
					if (InNavigationEvent.GetNavigationType() == EUINavigation::Up && &EntryWidget == FirstItem)
					{
						// Log that we navigated up from top of list
						OnControllerMovedUpFromFirstItem.ExecuteIfBound();
						return FNavigationReply::Stop();
					}
					else if (InNavigationEvent.GetNavigationType() == EUINavigation::Down && &EntryWidget == LastItem)
					{
						OnControllerMovedDownFromLastItem.ExecuteIfBound();
						return FNavigationReply::Stop();
					}
					else if (InNavigationEvent.GetNavigationType() == EUINavigation::Left || InNavigationEvent.GetNavigationType() == EUINavigation::Right)
					{
						return FNavigationReply::Stop();
					}
				}

				return InDefaultReply;
			});
		}
	});
}

UWidget* USocialLinksList::GetFirstWidget() const
{
	if (ListView->GetNumItems() > 0 && ListView->GetEntryWidgetFromItem(ListView->GetItemAt(0)))
	{
		return ListView->GetEntryWidgetFromItem(ListView->GetItemAt(0));
	}
	return nullptr;
}
