// Nodecraft, Inc. © 2012-2024, All Rights Reserved.


#include "UI/ServerDetails/AddAllowedFriendListItem.h"

#include "CommonListView.h"
#include "CommonTextBlock.h"
#include "DeveloperSettings/NodecraftStudioIdentitySettings.h"
#include "Models/AllowsDataObject.h"
#include "Models/FriendDataObject.h"
#include "Models/InviteFriendViewModel.h"
#include "Models/ServerDataObject.h"
#include "Services/AllowsService.h"
#include "Subsystems/AssetStreamerSubsystem.h"
#include "UI/Alerts/AlertMessage.h"
#include "UI/Common/AsyncImage.h"
#include "UI/Foundation/NodecraftButtonBase.h"

void UAddAllowedFriendListItem::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);

	const UInviteFriendViewModel* FriendViewModel = Cast<UInviteFriendViewModel>(ListItemObject);
	const UFriendDataObject* FriendDataObject = FriendViewModel->GetInvitee();
	
	Avatar->LoadPlayerAvatarAsync(FriendDataObject->GetPlayer());
	PlayerIdent->SetText(FText::FromString(FriendDataObject->GetPlayer()->GetIdent()));
	UTexture2D* IdentTexture = UNodecraftStudioIdentitySettings::Get().GetIconForType(FriendDataObject->GetPlayer()->GetIdentType());
	IdentImage->SetBrushFromTexture(IdentTexture);
	Status->SetText(FriendDataObject->GetStatus());

	if (const TSoftClassPtr<UCommonTextStyle> Style = StatusStyle.FindChecked(FriendDataObject->GetStatus().ToString()); Style.IsNull() == false)
	{
		UAssetStreamerSubsystem::Get().LoadAssetAsync(Style.ToSoftObjectPath(), FStreamableDelegate::CreateWeakLambda(this, [this, Style]
		{
			Status->SetStyle(Style.Get());
		}));
	}

	Alert->Hide();

	PrimaryActionButton->OnClicked().Clear();
	PrimaryActionButton->OnClicked().AddWeakLambda(this, [this]
	{
		if (UInviteFriendViewModel* FriendVM = GetListItem<UInviteFriendViewModel>())
		{
			FUpdateAllowResponseDelegate OnComplete;
			OnComplete.BindWeakLambda(this, [this, FriendVM](UAllowsDataObject* AllowsDataObject, bool bSuccess, TOptional<FText> Error)
			{
				if (bSuccess)
				{
					if (UCommonListView* ListView = Cast<UCommonListView>(GetOwningListView()))
					{
						ListView->RemoveItem(FriendVM);
						ListView->RequestRefresh();
						FriendVM->ClosePopupDelegate.ExecuteIfBound();
					}
				}
				else
				{
					Alert->Show(Error.GetValue(), EAlertType::Error);
				}
			});
			UAllowsService::Get().CreateAllow(FriendVM->GetServer()->GetId(),
			                                  FriendVM->GetInvitee()->GetPlayer()->GetId(), OnComplete);
		}
	});
	
}

FNavigationReply UAddAllowedFriendListItem::NativeOnNavigation(const FGeometry& MyGeometry,
	const FNavigationEvent& InNavigationEvent, const FNavigationReply& InDefaultReply)
{
	if (OnNavDelegate.IsBound())
	{
		return OnNavDelegate.Execute(MyGeometry, InNavigationEvent, InDefaultReply);
	}
	return Super::NativeOnNavigation(MyGeometry, InNavigationEvent, InDefaultReply);
}

FReply UAddAllowedFriendListItem::NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent)
{
	RegisterActionBinding(EUIActionBindingType::Primary);
	return Super::NativeOnFocusReceived(InGeometry, InFocusEvent);
}

void UAddAllowedFriendListItem::NativeOnFocusLost(const FFocusEvent& InFocusEvent)
{
	UnregisterUIActionBindings();
	Super::NativeOnFocusLost(InFocusEvent);
}
