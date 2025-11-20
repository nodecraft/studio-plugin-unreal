// Nodecraft, Inc. © 2012-2024, All Rights Reserved.


#include "UI/FriendsList/FriendsList.h"

#include "CommonListView.h"
#include "CommonLoadGuard.h"
#include "CommonTextBlock.h"
#include "Models/FriendDataObject.h"
#include "Stores/FriendsStore.h"

#define LOCTEXT_NAMESPACE "FriendsList"

void UFriendsList::NativeConstruct()
{
	Super::NativeConstruct();

	FriendCountText->SetText(FText::GetEmpty());

	if (UWorld* World = GetWorld())
	{
		if (UFriendsStore* FriendsStore = UFriendsStore::Get(World))
		{
			FriendsUpdatedDelegateHandle = FriendsStore->AddFriendsUpdatedListener(
				FOnFriendsUpdated::FDelegate::CreateWeakLambda(this, [this](const TArray<UFriendDataObject*>& Friends)
				{
					FriendListView->SetListItems(Friends);
					
					int32 OnlineFriendsCount = 0;
					for (const UFriendDataObject* Friend : Friends)
					{
						if (Friend->GetStatus().EqualToCaseIgnored(FText::FromString("offline")) == false)
						{
							OnlineFriendsCount++;
						}
					}

					const FText FriendCount = FText::Format(LOCTEXT("OnlinePlayerCount", "{0} / {1} Online"), OnlineFriendsCount, Friends.Num());
					FriendCountText->SetText(FriendCount);

					LoadGuard->SetIsLoading(false);
				}));
		}
	}
	
	FriendListView->OnEntryWidgetGenerated().AddWeakLambda(this, [this](UWidget& EntryWidget)
	{
		EntryWidget.SetFocus();
		FriendListView->OnEntryWidgetGenerated().RemoveAll(this);
	});
}

void UFriendsList::NativeDestruct()
{
	if (UWorld* World = GetWorld())
	{
		if (UFriendsStore* FriendsStore = UFriendsStore::Get(World))
		{
			FriendsStore->RemoveFriendsUpdatedListener(FriendsUpdatedDelegateHandle);
		}
	}
	Super::NativeDestruct();
	
}

UWidget* UFriendsList::NativeGetDesiredFocusTarget() const
{
	if (FriendListView->GetDisplayedEntryWidgets().IsValidIndex(0))
	{
		return FriendListView->GetDisplayedEntryWidgets()[0];
	}
	return Super::NativeGetDesiredFocusTarget();
}

#undef LOCTEXT_NAMESPACE
