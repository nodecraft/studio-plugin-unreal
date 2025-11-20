// Nodecraft, Inc. © 2012-2024, All Rights Reserved.


#include "UI/Foundation/NodecraftDynamicEntryBox.h"

TSubclassOf<UUserWidget> UNodecraftDynamicEntryBox::GetEntryWidgetClass() const
{
	return EntryWidgetClass;
}

void UNodecraftDynamicEntryBox::SetEntryWidgetClass(TSubclassOf<UUserWidget> InEntryWidgetClass)
{
	EntryWidgetClass = InEntryWidgetClass;
}

void UNodecraftDynamicEntryBox::Reset(bool bDeleteWidgets)
{
	ResetInternal(bDeleteWidgets);
}

UUserWidget* UNodecraftDynamicEntryBox::CreateEntryForDesigner(TSubclassOf<UUserWidget> InEntryClass)
{
	return CreateEntryInternal(InEntryClass);
}

UUserWidget* UNodecraftDynamicEntryBox::GetEntryAtIndex(int32 Index) const
{
	if (GetAllEntries().IsValidIndex(Index))
	{
		return GetAllEntries()[Index];
	}
	return nullptr;
}
