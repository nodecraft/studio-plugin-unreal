// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ServerInvites/ServerInvitesModal.h"

#include "Models/ServerDataObject.h"
#include "CommonListView.h"
#include "CommonTextBlock.h"
#include "Models/FriendDataObject.h"
#include "UI/Common/NodecraftLoadGuard.h"
#include "UI/Foundation/NodecraftButtonBase.h"
#include "Widgets/CommonActivatableWidgetContainer.h"

#define LOCTEXT_NAMESPACE "ServerInvitesModal"

void UServerInvitesModal::Configure(const TArray<UServerDataObject*>& Servers, const UFriendDataObject* Invitee, UCommonActivatableWidgetStack* PopupStack)
{
	ListView->ClearListItems();
	ListView->SetListItems(Servers);

	LoadGuard->SetIsLoading(false);

	Title->SetText(FText::Format(LOCTEXT("InviteToServer", "Invite {0} to a server"),
		Invitee->GetPlayer()->GetUsername().IsEmpty() ? FText::FromString(Invitee->GetPlayer()->GetIdent())
		: FText::FromString(Invitee->GetPlayer()->GetUsername())));

	CloseButtonTop->OnClicked().AddWeakLambda(this, [this, PopupStack]()
	{
		PopupStack->RemoveWidget(*this);
	});
	CloseButtonBottom->OnClicked().AddWeakLambda(this, [this, PopupStack]()
	{
		PopupStack->RemoveWidget(*this);
	});
}

void UServerInvitesModal::NativeConstruct()
{
	Super::NativeConstruct();

	LoadGuard->SetIsLoading(true);
}

#undef LOCTEXT_NAMESPACE
