// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#include "UI/MainMenu/FriendsButton.h"
#include "Stores/FriendsStore.h"

#define LOCTEXT_NAMESPACE "FriendsButton"

void UFriendsButton::NativeConstruct()
{
	Super::NativeConstruct();
	FriendsUpdatedDelegateHandle = UFriendsStore::Get(GetWorld())->AddFriendsUpdatedListener(FOnFriendsUpdated::FDelegate::CreateWeakLambda(this, [this](const TArray<UFriendDataObject*>& Friends)
	{
		FriendsCountTextBlock->SetText(FText::Format(LOCTEXT("FriendsCountText", "{0} Friends Online"), FText::AsNumber(Friends.Num())));
	}));
}

void UFriendsButton::NativeDestruct()
{
	Super::NativeDestruct();
	UFriendsStore::Get(GetWorld())->RemoveFriendsUpdatedListener(FriendsUpdatedDelegateHandle);
	FriendsUpdatedDelegateHandle.Reset();
}

#undef LOCTEXT_NAMESPACE