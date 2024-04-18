// Nodecraft, Inc. © 2012-2024, All Rights Reserved.


#include "UI/ServerInvites/ServerInvitesModal.h"

#include "Models/ServerDataObject.h"
#include "CommonListView.h"
#include "CommonTextBlock.h"
#include "Models/FriendDataObject.h"
#include "Models/InviteFriendViewModel.h"
#include "UI/Alerts/AlertMessage.h"
#include "UI/Common/NodecraftLoadGuard.h"
#include "UI/Foundation/NodecraftButtonBase.h"
#include "UI/FriendsList/FriendsListItem.h"

#define LOCTEXT_NAMESPACE "ServerInvitesModal"

void UServerInvitesModal::Configure(const TArray<UServerDataObject*>& Servers, const UFriendDataObject* InInvitee, FSimpleDelegate OnPopupClosed)
{
	// aggregate servers and invitees together
	TArray<UInviteFriendViewModel*> ViewModels;
	for (const UServerDataObject* Server : Servers)
	{
		UInviteFriendViewModel* InviteDetails = UInviteFriendViewModel::FromDataObjects(Server, InInvitee, OnPopupClosed);
		InviteDetails->OnRequestAlertMessage.BindWeakLambda(this, [this](EAlertType AlertType, FText Message)
		{
			Alert->Show(Message, AlertType);
		});
		ViewModels.Add(InviteDetails);
	}
	
	ListView->ClearListItems();
	ListView->SetListItems(ViewModels);

	LoadGuard->SetIsLoading(false);

	Title->SetText(FText::Format(LOCTEXT("InviteToServer", "Invite {0} to a server"),
		InInvitee->GetPlayer()->GetUsername().IsEmpty() ? FText::FromString(InInvitee->GetPlayer()->GetIdent())
		: FText::FromString(InInvitee->GetPlayer()->GetUsername())));

	CloseButtonTop->OnClicked().AddWeakLambda(this, [this, OnPopupClosed]()
	{
		OnPopupClosed.ExecuteIfBound();
	});
	CloseButtonBottom->OnClicked().AddWeakLambda(this, [this, OnPopupClosed]()
	{
		OnPopupClosed.ExecuteIfBound();
	});
}

void UServerInvitesModal::NativeConstruct()
{
	Super::NativeConstruct();

	LoadGuard->SetIsLoading(true);
}

void UServerInvitesModal::NativeOnActivated()
{
	Super::NativeOnActivated();
	Alert->Hide();
}

#undef LOCTEXT_NAMESPACE
