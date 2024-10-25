// Nodecraft, Inc. © 2012-2024, All Rights Reserved.


#include "UI/AllowsList/AllowsListItem.h"

#include "CommonTextBlock.h"
#include "Components/HorizontalBox.h"
#include "DeveloperSettings/NodecraftStudioIdentitySettings.h"
#include "Models/AllowsDataObject.h"
#include "Models/AllowStatus.h"
#include "Models/ServerDataObject.h"
#include "Services/AllowsService.h"
#include "UI/Common/AsyncImage.h"
#include "UI/Common/NodecraftLoadGuard.h"
#include "UI/Foundation/NodecraftButtonBase.h"

#define LOCTEXT_NAMESPACE "AllowsListItem"

#define LOCTEXT_ACCEPT LOCTEXT("Accept", "Accept")
#define LOCTEXT_ACCEPTED LOCTEXT("Accepted", "Accepted")
#define LOCTEXT_DECLINE LOCTEXT("Decline", "Decline")
#define LOCTEXT_DECLINED LOCTEXT("Declined", "Declined")
#define LOCTEXT_LEAVE_AND_DECLINE LOCTEXT("LeaveAndDecline", "Leave & Decline")

void UAllowsListItem::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);

	const UAllowsDataObject* AllowsDataObject = Cast<UAllowsDataObject>(ListItemObject);
	
	PlayerImage->LoadPlayerAvatarAsync(AllowsDataObject->GetSender());
	UTexture2D* IdentTexture = UNodecraftStudioIdentitySettings::Get().GetIconForType(AllowsDataObject->GetSender()->GetIdentType());
	IdentImage->SetBrushFromTexture(IdentTexture);
	Username->SetText(AllowsDataObject->GetSender()->GetUsername().IsEmpty() ?
		FText::FromString(AllowsDataObject->GetSender()->GetIdent()) : FText::FromString(AllowsDataObject->GetSender()->GetUsername()));
	
	ServerName->SetText(AllowsDataObject->GetServer()->GetTitle());

	switch (AllowsDataObject->GetStatus())
	{
	case EAllowStatus::Pending:
		if (AllowsDataObject->GetDateExpires().GetTicks() != 0 && AllowsDataObject->GetDateExpires() < FDateTime::UtcNow())
		{
			// pending but expired
			bShowActionsInActionBar = false;
			ButtonsBox->SetVisibility(ESlateVisibility::Collapsed);
			RevokedOrExpiredText->SetText(FText::Format(LOCTEXT("ExpiredOnDate", "Expired {0}"),
				FText::AsDate(AllowsDataObject->GetDateExpires())));
			RevokedOrExpiredText->SetVisibility(ESlateVisibility::HitTestInvisible);
		}
		else
		{
			bShowActionsInActionBar = true;
			ButtonsBox->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			RevokedOrExpiredText->SetVisibility(ESlateVisibility::Collapsed);
			PrimaryActionButton->SetButtonText(LOCTEXT_ACCEPT);
			SecondaryActionButton->SetButtonText(LOCTEXT_DECLINE);
			PrimaryActionButton->SetIsEnabled(true);
			SecondaryActionButton->SetIsEnabled(true);
		}
		break;
	case EAllowStatus::Accepted:
		bShowActionsInActionBar = true;
		ButtonsBox->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		RevokedOrExpiredText->SetVisibility(ESlateVisibility::Collapsed);
		PrimaryActionButton->SetButtonText(LOCTEXT_ACCEPTED);
		SecondaryActionButton->SetButtonText(LOCTEXT_LEAVE_AND_DECLINE);
		PrimaryActionButton->SetIsEnabled(false);
		SecondaryActionButton->SetIsEnabled(true);
		break;
	case EAllowStatus::Declined:
		bShowActionsInActionBar = true;
		ButtonsBox->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		RevokedOrExpiredText->SetVisibility(ESlateVisibility::Collapsed);
		PrimaryActionButton->SetButtonText(LOCTEXT_ACCEPT);
		SecondaryActionButton->SetButtonText(LOCTEXT_DECLINED);
		PrimaryActionButton->SetIsEnabled(true);
		SecondaryActionButton->SetIsEnabled(false);
		break;
	case EAllowStatus::Revoked:
		bShowActionsInActionBar = false;
		ButtonsBox->SetVisibility(ESlateVisibility::Collapsed);
		RevokedOrExpiredText->SetText(LOCTEXT("InviteWasRevoked", "This invite was revoked by the server owner."));
		RevokedOrExpiredText->SetVisibility(ESlateVisibility::HitTestInvisible);
		break;
	default: ;
	}

	PrimaryActionButton->OnClicked().Clear();
	SecondaryActionButton->OnClicked().Clear();

	PrimaryActionButton->OnClicked().AddWeakLambda(this, [this, AllowsDataObject]
	{
		LoadGuard->SetIsLoading(true);
		
		FSimpleServiceResponseDelegate OnAcceptAllow;
		OnAcceptAllow.BindWeakLambda(this, [this](bool bSuccess, TOptional<FText> Error)
		{
			if (bSuccess)
			{
				if (UAllowsDataObject* ListItem = GetListItem<UAllowsDataObject>())
				{
					ListItem->SetStatus(EAllowStatus::Accepted);
				}
				PrimaryActionButton->SetIsEnabled(false);
				PrimaryActionButton->SetButtonText(LOCTEXT_ACCEPTED);
				SecondaryActionButton->SetIsSelected(false);
				SecondaryActionButton->SetIsEnabled(true);
				SecondaryActionButton->SetButtonText(LOCTEXT_LEAVE_AND_DECLINE);
			}
			LoadGuard->SetIsLoading(false);
			UpdateActionBar();
		});
		UAllowsService::Get().AcceptAllow(AllowsDataObject->GetID(), OnAcceptAllow);
	});

	SecondaryActionButton->OnClicked().AddWeakLambda(this, [this, AllowsDataObject]
	{
		LoadGuard->SetIsLoading(true);
		
		FSimpleServiceResponseDelegate OnDeclineALlow;
		OnDeclineALlow.BindWeakLambda(this, [this](bool bSuccess, TOptional<FText> Error)
		{
			if (bSuccess)
			{
				if (UAllowsDataObject* ListItem = GetListItem<UAllowsDataObject>())
				{
					ListItem->SetStatus(EAllowStatus::Declined);
				}
				PrimaryActionButton->SetIsEnabled(true);
				PrimaryActionButton->SetIsSelected(false);
				PrimaryActionButton->SetButtonText(LOCTEXT_ACCEPT);
				SecondaryActionButton->SetIsEnabled(false);
				SecondaryActionButton->SetButtonText(LOCTEXT_DECLINED);
			}
			LoadGuard->SetIsLoading(false);
			UpdateActionBar();
		});
		UAllowsService::Get().DeclineAllow(AllowsDataObject->GetID(), OnDeclineALlow);
	});
}

void UAllowsListItem::UpdateActionBar()
{
	if (bShowActionsInActionBar)
	{
		if (PrimaryActionButton->GetIsEnabled())
		{
			if (PrimaryUIActionHandle.IsValid() == false)
			{
				RegisterActionBinding(EUIActionBindingType::Primary);
			}
		}
		else
		{
			PrimaryUIActionHandle.Unregister();
		}
		
		if (SecondaryActionButton->GetIsEnabled())
		{
			if (SecondaryUIActionHandle.IsValid() == false)
			{
				RegisterActionBinding(EUIActionBindingType::Secondary);
			}
		}
		else
		{
			SecondaryUIActionHandle.Unregister();
		}
	}
}

FReply UAllowsListItem::NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent)
{
	UpdateActionBar();
	
	return Super::NativeOnFocusReceived(InGeometry, InFocusEvent);
}

void UAllowsListItem::NativeOnFocusLost(const FFocusEvent& InFocusEvent)
{
	UnregisterUIActionBindings();
	Super::NativeOnFocusLost(InFocusEvent);
}

#undef LOCTEXT_NAMESPACE

#undef LOCTEXT_ACCEPT
#undef LOCTEXT_ACCEPTED
#undef LOCTEXT_DECLINE
#undef LOCTEXT_DECLINED
#undef LOCTEXT_LEAVE_AND_DECLINE
