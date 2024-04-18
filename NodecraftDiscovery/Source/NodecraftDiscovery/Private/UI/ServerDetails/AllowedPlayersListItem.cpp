// Nodecraft, Inc. © 2012-2024, All Rights Reserved.


#include "UI/ServerDetails/AllowedPlayersListItem.h"

#include "CommonTextBlock.h"
#include "API/NodecraftStudioSessionManagerSubsystem.h"
#include "DeveloperSettings/NodecraftStudioIdentitySettings.h"
#include "Models/AllowsDataObject.h"
#include "Models/AllowStatus.h"
#include "Models/AllowsViewModel.h"
#include "Models/ServerDataObject.h"
#include "Services/AllowsService.h"
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
		
		RevokeButton->SetVisibility(CurrentServer->GetPlayer()->GetId() == UNodecraftStudioSessionManager::Get().GetPlayerId()
			&& AllowsDataObject->GetStatus() != EAllowStatus::Revoked
			? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
		if (RevokeButton->IsVisible())
		{
			RevokeButton->OnClicked().Clear();
			RevokeButton->OnClicked().AddWeakLambda(this, [this, CurrentServer, AllowsDataObject]
			{
				LoadGuard->SetIsLoading(true);
				FUpdateAllowResponseDelegate OnComplete;
				OnComplete.BindWeakLambda(this, [this, CurrentServer](UAllowsDataObject* AllowsDataObject, bool bSuccess, TOptional<FText> Error)
				{
					if (bSuccess)
					{
						UAllowsViewModel* ViewModel = UAllowsViewModel::FromDataObjects(AllowsDataObject, CurrentServer);
						NativeOnListItemObjectSet(ViewModel);
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
