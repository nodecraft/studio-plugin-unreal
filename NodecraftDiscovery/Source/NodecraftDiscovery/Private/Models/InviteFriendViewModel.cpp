// Nodecraft, Inc. © 2012-2024, All Rights Reserved.


#include "Models/InviteFriendViewModel.h"

UInviteFriendViewModel* UInviteFriendViewModel::FromDataObjects(const UServerDataObject* Server, const UFriendDataObject* Invitee, FSimpleDelegate InClosePopupDelegate)
{
	UInviteFriendViewModel* InviteDetails = NewObject<UInviteFriendViewModel>();
	InviteDetails->ServerDataObject = Server;
	InviteDetails->InviteeDataObject = Invitee;
	InviteDetails->ClosePopupDelegate = InClosePopupDelegate;
	return InviteDetails;
}

const UServerDataObject* UInviteFriendViewModel::GetServer() const
{
	return ServerDataObject;
}

const UFriendDataObject* UInviteFriendViewModel::GetInvitee() const
{
	return InviteeDataObject;
}