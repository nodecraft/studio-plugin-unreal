// Nodecraft, Inc. © 2012-2024, All Rights Reserved.


#include "UI/ServerDetails/AllowedPlayersListItem.h"

#include "CommonTextBlock.h"
#include "API/NodecraftStudioSessionManagerSubsystem.h"
#include "Components/ListView.h"
#include "DeveloperSettings/NodecraftStudioIdentitySettings.h"
#include "Models/AllowsDataObject.h"
#include "Models/AllowStatus.h"
#include "Models/AllowsViewModel.h"
#include "Models/ServerDataObject.h"
#include "Services/AllowsService.h"
#include "Stores/ServersStore.h"
#include "UI/Common/AsyncImage.h"
#include "UI/Common/NodecraftLoadGuard.h"
#include "UI/Foundation/NodecraftButtonBase.h"

void UAllowedPlayersListItem::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);

	const UAllowsViewModel* AllowsViewModel = Cast<UAllowsViewModel>(ListItemObject);
	const UAllowsDataObject* AllowsDataObject = AllowsViewModel->GetAllow();
	const UServerDataObject* CurrentServer = AllowsViewModel->GetServer();
	if (AllowsDataObject == nullptr)
	{
		return;
	}
	
	if (const UPlayerDataObject* Player = AllowsDataObject->GetReceiver())
	{
		Avatar->LoadPlayerAvatarAsync(Player);
		Username->SetText(Player->GetUsername().IsEmpty()
			? FText::FromString(Player->GetIdent()) : FText::FromString(Player->GetUsername()));
		UTexture2D* IdentTexture = UNodecraftStudioIdentitySettings::Get().GetIconForType(Player->GetIdentType());
		IdentImage->SetBrushFromTexture(IdentTexture);
		IdentText->SetText(FText::FromString(Player->GetIdent()));

		StatusIcon->SetBrushFromSoftTexture(StatusIcons.FindChecked(AllowsDataObject->GetStatus()));
		StatusText->SetText(StatusMessage.FindChecked(AllowsDataObject->GetStatus()));
		
		PrimaryActionButton->SetVisibility(CurrentServer->GetPlayer()->GetId() == UNodecraftStudioSessionManager::Get().GetPlayerId()
			&& AllowsDataObject->GetStatus() != EAllowStatus::Revoked
			&& AllowsDataObject->GetStatus() != EAllowStatus::Declined
			? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
		if (PrimaryActionButton->IsVisible())
		{
			PrimaryActionButton->OnClicked().Clear();
			PrimaryActionButton->OnClicked().AddWeakLambda(this, [this, CurrentServer, AllowsDataObject]
			{
				LoadGuard->SetIsLoading(true);
				FUpdateAllowResponseDelegate OnComplete;
				OnComplete.BindWeakLambda(this, [this, CurrentServer](UAllowsDataObject* AllowsDataObject, bool bSuccess, TOptional<FText> Error)
				{
					if (bSuccess && UServersStore::Get().GetCurrentServerId() == CurrentServer->GetId())
					{
						UAllowsViewModel* ViewModel = UAllowsViewModel::FromDataObjects(AllowsDataObject, CurrentServer);
						NativeOnListItemObjectSet(ViewModel);
						UpdateActionBar();
					}
					LoadGuard->SetIsLoading(false);
				});
				UAllowsService::Get().UpdateAllow(CurrentServer->GetId(), AllowsDataObject->GetID(), EAllowStatus::Revoked, OnComplete);
			});
		}
		
		switch (AllowsDataObject->GetStatus())
		{
		case EAllowStatus::Pending:
			StatusIcon->SetColorAndOpacity(PendingStyle.TintColor);
			StatusText->SetStyle(PendingStyle.TextStyle);
			break;
		case EAllowStatus::Accepted:
			StatusIcon->SetColorAndOpacity(AcceptedStyle.TintColor);
			StatusText->SetStyle(AcceptedStyle.TextStyle);
			break;
		case EAllowStatus::Declined:
			StatusIcon->SetColorAndOpacity(DeclinedStyle.TintColor);
			StatusText->SetStyle(DeclinedStyle.TextStyle);
			break;
		case EAllowStatus::Revoked:
			StatusIcon->SetColorAndOpacity(RevokedStyle.TintColor);
			StatusText->SetStyle(RevokedStyle.TextStyle);
			break;
		}
	}
}

FReply UAllowedPlayersListItem::NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent)
{
	UpdateActionBar();
	
	if (UListView* OwningListView = Cast<UListView>(GetOwningListView()))
	{
		OwningListView->SetSelectedItem(GetListItem<UAllowsViewModel>());
	}
	return Super::NativeOnFocusReceived(InGeometry, InFocusEvent);
}

void UAllowedPlayersListItem::NativeOnFocusLost(const FFocusEvent& InFocusEvent)
{
	UnregisterUIActionBindings();
	Super::NativeOnFocusLost(InFocusEvent);
}

FNavigationReply UAllowedPlayersListItem::NativeOnNavigation(const FGeometry& MyGeometry,
	const FNavigationEvent& InNavigationEvent, const FNavigationReply& InDefaultReply)
{
	if (OnNavDelegate.IsBound())
	{
		return OnNavDelegate.Execute(MyGeometry, InNavigationEvent, InDefaultReply);
	}
	return Super::NativeOnNavigation(MyGeometry, InNavigationEvent, InDefaultReply);
}

void UAllowedPlayersListItem::UpdateActionBar()
{
	if (PrimaryActionButton && PrimaryActionButton->IsVisible())
	{
		if (PrimaryUIActionHandle.IsValid() == false)
		{
			RegisterActionBinding(EUIActionBindingType::Primary);
		}
	}
	else
	{
		UnregisterUIActionBindings();
	}
}
