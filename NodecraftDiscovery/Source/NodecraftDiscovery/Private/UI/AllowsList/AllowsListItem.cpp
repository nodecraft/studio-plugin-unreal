// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/AllowsList/AllowsListItem.h"

#include "CommonTextBlock.h"
#include "Components/HorizontalBox.h"
#include "DeveloperSettings/IdentitySettings.h"
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
	UTexture2D* IdentTexture = UIdentitySettings::Get().GetIconForType(AllowsDataObject->GetSender()->GetIdentType());
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
			ButtonsBox->SetVisibility(ESlateVisibility::Collapsed);
			RevokedOrExpiredText->SetText(FText::Format(LOCTEXT("ExpiredOnDate", "Expired {0}"),
				FText::AsDate(AllowsDataObject->GetDateExpires())));
			RevokedOrExpiredText->SetVisibility(ESlateVisibility::HitTestInvisible);
		}
		else
		{
			ButtonsBox->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			RevokedOrExpiredText->SetVisibility(ESlateVisibility::Collapsed);
			AcceptButton->SetButtonText(LOCTEXT_ACCEPT);
			DeclineButton->SetButtonText(LOCTEXT_DECLINE);
			AcceptButton->SetIsEnabled(true);
			DeclineButton->SetIsEnabled(true);
		}
		break;
	case EAllowStatus::Accepted:
		ButtonsBox->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		RevokedOrExpiredText->SetVisibility(ESlateVisibility::Collapsed);
		AcceptButton->SetButtonText(LOCTEXT_ACCEPTED);
		DeclineButton->SetButtonText(LOCTEXT_LEAVE_AND_DECLINE);
		AcceptButton->SetIsEnabled(false);
		DeclineButton->SetIsEnabled(true);
		break;
	case EAllowStatus::Declined:
		ButtonsBox->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		RevokedOrExpiredText->SetVisibility(ESlateVisibility::Collapsed);
		AcceptButton->SetButtonText(LOCTEXT_ACCEPT);
		DeclineButton->SetButtonText(LOCTEXT_DECLINED);
		AcceptButton->SetIsEnabled(true);
		DeclineButton->SetIsEnabled(false);
		break;
	case EAllowStatus::Revoked:
		ButtonsBox->SetVisibility(ESlateVisibility::Collapsed);
		RevokedOrExpiredText->SetText(LOCTEXT("InviteWasRevoked", "This invite was revoked by the server owner."));
		RevokedOrExpiredText->SetVisibility(ESlateVisibility::HitTestInvisible);
		break;
	default: ;
	}

	AcceptButton->OnClicked().Clear();
	DeclineButton->OnClicked().Clear();

	AcceptButton->OnClicked().AddWeakLambda(this, [this, AllowsDataObject]
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
				AcceptButton->SetIsEnabled(false);
				AcceptButton->SetButtonText(LOCTEXT_ACCEPTED);
				DeclineButton->SetIsSelected(false);
				DeclineButton->SetIsEnabled(true);
				DeclineButton->SetButtonText(LOCTEXT_LEAVE_AND_DECLINE);
			}
			LoadGuard->SetIsLoading(false);
		});
		UAllowsService::Get().AcceptAllow(AllowsDataObject->GetID(), OnAcceptAllow);
	});

	DeclineButton->OnClicked().AddWeakLambda(this, [this, AllowsDataObject]
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
				AcceptButton->SetIsEnabled(true);
				AcceptButton->SetIsSelected(false);
				AcceptButton->SetButtonText(LOCTEXT_ACCEPT);
				DeclineButton->SetIsEnabled(false);
				DeclineButton->SetButtonText(LOCTEXT_DECLINED);
			}
			LoadGuard->SetIsLoading(false);
		});
		UAllowsService::Get().DeclineAllow(AllowsDataObject->GetID(), OnDeclineALlow);
	});
}

#undef LOCTEXT_NAMESPACE

#undef LOCTEXT_ACCEPT
#undef LOCTEXT_ACCEPTED
#undef LOCTEXT_DECLINE
#undef LOCTEXT_DECLINED
#undef LOCTEXT_LEAVE_AND_DECLINE
