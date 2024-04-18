﻿// Nodecraft, Inc. © 2012-2024, All Rights Reserved.


#include "UI/ServerInvites/ServerInvitesListItem.h"

#include "CommonTextBlock.h"
#include "Models/FriendDataObject.h"
#include "Models/ServerDataObject.h"
#include "Models/InviteFriendViewModel.h"
#include "Services/InvitesService.h"
#include "UI/Alerts/AlertMessage.h"
#include "UI/Common/AsyncImage.h"
#include "UI/Foundation/NodecraftButtonBase.h"


void UServerInvitesListItem::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);

	const UInviteFriendViewModel* InviteDetails = Cast<UInviteFriendViewModel>(ListItemObject);

	const UServerDataObject* ServerDataObject = InviteDetails->GetServer();
	const UFriendDataObject* Invitee = InviteDetails->GetInvitee();
	
	ServerImage->LoadImageAsync(ServerDataObject->GetImageUrl(), ServerDataObject->GetImageFilenameForCache());
	ServerTitle->SetText(ServerDataObject->GetTitle());
	ServerDescription->SetText(ServerDataObject->GetSummary());
	PlayerCount->SetText(FText::FromString(FString::Printf(TEXT("%d / %d"), ServerDataObject->GetPlayersCount(), ServerDataObject->GetPlayersMax())));
	
	InviteButton->OnClicked().AddWeakLambda(this, [this, ServerDataObject, Invitee, InviteDetails]
	{
		FOnCreateInvite OnCreateInviteComplete;
		OnCreateInviteComplete.BindWeakLambda(this, [this, InviteDetails](const UInviteDataObject* InviteDataObject, bool bSuccess, TOptional<FText> Error)
		{
			if (bSuccess)
			{
				InviteDetails->ClosePopupDelegate.ExecuteIfBound();
			}
			else
			{
				InviteDetails->OnRequestAlertMessage.ExecuteIfBound(EAlertType::Error, Error.GetValue());
			}
		});
		UInvitesService::Get().CreateInvite(ServerDataObject->GetId(), Invitee->GetPlayer()->GetId(), OnCreateInviteComplete);
	});
}
