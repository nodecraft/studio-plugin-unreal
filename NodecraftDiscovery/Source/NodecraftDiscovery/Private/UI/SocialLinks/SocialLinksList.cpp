// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SocialLinks/SocialLinksList.h"

#include "CommonListView.h"
#include "CommonTextBlock.h"
#include "Components/Image.h"
#include "Models/SocialLinkDataObject.h"


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
