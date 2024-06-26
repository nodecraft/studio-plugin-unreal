﻿// Nodecraft, Inc. © 2012-2024, All Rights Reserved.


#include "UI/Common/NodecraftTabListWidgetBase.h"



#include "Blueprint/UserWidget.h"
#include "CommonAnimatedSwitcher.h"
#include "CommonButtonBase.h"
#include "CommonUserWidget.h"
#include "Components/SlateWrapperTypes.h"
#include "Components/Widget.h"
#include "Containers/UnrealString.h"
#include "Misc/AssertionMacros.h"
#include "Templates/Casts.h"
#include "Templates/SharedPointer.h"
#include "UObject/Class.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(NodecraftTabListWidgetBase)

void UNodecraftTabListWidgetBase::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UNodecraftTabListWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();

	SetupTabs();
}

void UNodecraftTabListWidgetBase::NativeDestruct()
{
	for (FNodecraftTabDescriptor& TabInfo : PreregisteredTabInfoArray)
	{
		if (TabInfo.CreatedTabContentWidget)
		{
			TabInfo.CreatedTabContentWidget->RemoveFromParent();
			TabInfo.CreatedTabContentWidget = nullptr;
		}
	}

	Super::NativeDestruct();
}

bool UNodecraftTabListWidgetBase::GetPreregisteredTabInfo(const FName TabNameId, FNodecraftTabDescriptor& OutTabInfo)
{
	const FNodecraftTabDescriptor* const FoundTabInfo = PreregisteredTabInfoArray.FindByPredicate([this, TabNameId](FNodecraftTabDescriptor& TabInfo) -> bool
	{
		return TabInfo.TabId == TabNameId;
	});

	if (!FoundTabInfo)
	{
		return false;
	}

	OutTabInfo = *FoundTabInfo;
	return true;
}

void UNodecraftTabListWidgetBase::SetTabHiddenState(FName TabNameId, bool bHidden)
{
	for (FNodecraftTabDescriptor& TabInfo : PreregisteredTabInfoArray)
	{
		if (TabInfo.TabId == TabNameId)
		{
			TabInfo.bHidden = bHidden;
			break;
		}
	}
}

bool UNodecraftTabListWidgetBase::RegisterDynamicTab(const FNodecraftTabDescriptor& TabDescriptor)
{
	// If it's hidden we just ignore it.
	if (TabDescriptor.bHidden)
	{
		return true;
	}
	
	PendingTabLabelInfoMap.Add(TabDescriptor.TabId, TabDescriptor);

	return RegisterTab(TabDescriptor.TabId, TabDescriptor.TabButtonType, TabDescriptor.CreatedTabContentWidget);
}

void UNodecraftTabListWidgetBase::HandlePreLinkedSwitcherChanged()
{
	for (const FNodecraftTabDescriptor& TabInfo : PreregisteredTabInfoArray)
	{
		// Remove tab content widget from linked switcher, as it is being disassociated.
		if (TabInfo.CreatedTabContentWidget)
		{
			TabInfo.CreatedTabContentWidget->RemoveFromParent();
		}
	}

	Super::HandlePreLinkedSwitcherChanged();
}

void UNodecraftTabListWidgetBase::HandlePostLinkedSwitcherChanged()
{
	if (!IsDesignTime() && GetCachedWidget().IsValid())
	{
		// Don't bother making tabs if we're in the designer or haven't been constructed yet
		SetupTabs();
	}

	Super::HandlePostLinkedSwitcherChanged();
}

void UNodecraftTabListWidgetBase::HandleTabCreation_Implementation(FName TabId, UCommonButtonBase* TabButton)
{
	FNodecraftTabDescriptor* TabInfoPtr = nullptr;
	
	FNodecraftTabDescriptor TabInfo;
	if (GetPreregisteredTabInfo(TabId, TabInfo))
	{
		TabInfoPtr = &TabInfo;
	}
	else
	{
		TabInfoPtr = PendingTabLabelInfoMap.Find(TabId);
	}
	
	if (TabButton->GetClass()->ImplementsInterface(UNodecraftTabButtonInterface::StaticClass()))
	{
		if (ensureMsgf(TabInfoPtr, TEXT("A tab button was created with id %s but no label info was specified. RegisterDynamicTab should be used over RegisterTab to provide label info."), *TabId.ToString()))
		{
			INodecraftTabButtonInterface::Execute_SetTabLabelInfo(TabButton, *TabInfoPtr);
		}
	}

	PendingTabLabelInfoMap.Remove(TabId);
}

bool UNodecraftTabListWidgetBase::IsFirstTabActive() const
{
	if (PreregisteredTabInfoArray.Num() > 0)
	{
		return GetActiveTab() == PreregisteredTabInfoArray[0].TabId;
	}

	return false;
}

bool UNodecraftTabListWidgetBase::IsLastTabActive() const
{
	if (PreregisteredTabInfoArray.Num() > 0)
	{
		return GetActiveTab() == PreregisteredTabInfoArray.Last().TabId;
	}

	return false;
}

bool UNodecraftTabListWidgetBase::IsTabVisible(FName TabId)
{
	if (const UCommonButtonBase* Button = GetTabButtonBaseByID(TabId))
	{
		const ESlateVisibility TabVisibility = Button->GetVisibility();
		return (TabVisibility == ESlateVisibility::Visible
			|| TabVisibility == ESlateVisibility::HitTestInvisible
			|| TabVisibility == ESlateVisibility::SelfHitTestInvisible);
	}

	return false;
}

int32 UNodecraftTabListWidgetBase::GetVisibleTabCount()
{
	int32 Result = 0;
	const int32 TabCount = GetTabCount();
	for ( int32 Index = 0; Index < TabCount; Index++ )
	{
		if (IsTabVisible(GetTabIdAtIndex( Index )))
		{
			Result++;
		}
	}

	return Result;
}

void UNodecraftTabListWidgetBase::SetupTabs()
{
	for (FNodecraftTabDescriptor& TabInfo : PreregisteredTabInfoArray)
	{
		if (TabInfo.bHidden)
		{
			continue;
		}

		// If we've created our own instance in UMG, link them up now
		if (UCommonAnimatedSwitcher* CurrentLinkedSwitcher = GetLinkedSwitcher())
		{
			for (UWidget* Child : CurrentLinkedSwitcher->GetAllChildren())
			{
				if (Child->IsA(TabInfo.TabContentType))
				{
					TabInfo.CreatedTabContentWidget = Child;
					break;
				}
			}
		}

		// If the tab content hasn't been created already, create it.
		if (!TabInfo.CreatedTabContentWidget && TabInfo.TabContentType)
		{
			TabInfo.CreatedTabContentWidget = CreateWidget<UCommonUserWidget>(GetOwningPlayer(), TabInfo.TabContentType);
			OnTabContentCreatedNative.Broadcast(TabInfo.TabId, Cast<UCommonUserWidget>(TabInfo.CreatedTabContentWidget));
			OnTabContentCreated.Broadcast(TabInfo.TabId, Cast<UCommonUserWidget>(TabInfo.CreatedTabContentWidget));
		}

		if (UCommonAnimatedSwitcher* CurrentLinkedSwitcher = GetLinkedSwitcher())
		{
			// Add the tab content to the newly linked switcher.
			if (!CurrentLinkedSwitcher->HasChild(TabInfo.CreatedTabContentWidget))
			{
				CurrentLinkedSwitcher->AddChild(TabInfo.CreatedTabContentWidget);
			}
		}

		// If the tab is not already registered, register it.
		if (GetTabButtonBaseByID(TabInfo.TabId) == nullptr)
		{
			RegisterTab(TabInfo.TabId, TabInfo.TabButtonType, TabInfo.CreatedTabContentWidget);
		}
	}
}
