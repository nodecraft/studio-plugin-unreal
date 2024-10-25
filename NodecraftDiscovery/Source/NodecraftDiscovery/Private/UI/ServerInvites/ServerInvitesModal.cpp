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
#include "UI/ServerInvites/ServerInvitesListItem.h"

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

void UServerInvitesModal::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	ListView->OnEntryWidgetGenerated().AddWeakLambda(this, [this](UWidget& InEntryWidget)
	{
		if (UServerInvitesListItem* ListItem = Cast<UServerInvitesListItem>(&InEntryWidget))
		{
			ListItem->OnNavDelegate.BindWeakLambda(this, [this, &InEntryWidget](const FGeometry& MyGeometry, const FNavigationEvent& InNavigationEvent, const FNavigationReply& InDefaultReply)
			{
				const UUserWidget* FirstItem = ListView->GetEntryWidgetFromItem(ListView->GetItemAt(0));
				const UUserWidget* LastItem = ListView->GetEntryWidgetFromItem(ListView->GetItemAt(ListView->GetNumItems() - 1));
				if (InNavigationEvent.GetNavigationType() == EUINavigation::Up && &InEntryWidget == FirstItem)
				{
					return FNavigationReply::Stop();
				}
				if (InNavigationEvent.GetNavigationType() == EUINavigation::Down && &InEntryWidget == LastItem)
				{
					return FNavigationReply::Stop();
				}
				if (InNavigationEvent.GetNavigationType() == EUINavigation::Left || InNavigationEvent.GetNavigationType() == EUINavigation::Right
					|| InNavigationEvent.GetNavigationType() == EUINavigation::Next || InNavigationEvent.GetNavigationType() == EUINavigation::Previous)
				{
					return FNavigationReply::Stop();
				}
				return InDefaultReply;
			});
		}
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

UWidget* UServerInvitesModal::NativeGetDesiredFocusTarget() const
{
	return ListView->GetDisplayedEntryWidgets().IsValidIndex(0)
		? ListView->GetDisplayedEntryWidgets()[0]
		: Super::NativeGetDesiredFocusTarget();
}

bool UServerInvitesModal::NativeOnHandleBackAction()
{
	CloseButtonTop->OnClicked().Broadcast();
	return Super::NativeOnHandleBackAction();
}

#undef LOCTEXT_NAMESPACE
