// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/FriendsList/FriendsList.h"

#include "CommonListView.h"
#include "CommonLoadGuard.h"
#include "CommonTextBlock.h"
#include "Models/FriendDataObject.h"
#include "Services/FriendsService.h"

#define LOCTEXT_NAMESPACE "FriendsList"

void UFriendsList::NativeConstruct()
{
	Super::NativeConstruct();

	LoadData();
}

void UFriendsList::LoadData()
{
	UFriendsService& FriendsService = UFriendsService::Get();

	LoadGuard->SetIsLoading(true);
	
	FGetFriendsListDelegate GetFriendsListDelegate;
	GetFriendsListDelegate.BindWeakLambda(this, [this](const TArray<UFriendDataObject*> Friends, bool bSuccess, TOptional<FText> Error)
	{
		if (bSuccess)
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
		}
		else
		{
			FriendCountText->SetText(FText::GetEmpty());
		}
	});
	
	FriendsService.GetFriends(GetFriendsListDelegate);
}

#undef LOCTEXT_NAMESPACE