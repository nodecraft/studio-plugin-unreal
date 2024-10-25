// Nodecraft, Inc. © 2012-2024, All Rights Reserved.


#include "UI/Settings/AccountReputationListItem.h"

#include "CommonTextBlock.h"
#include "DataTypes/ModerationTypes.h"
#include "Models/AccountReputationEntryDataObject.h"
#include "Models/ServerDataObject.h"
#include "Subsystems/AssetStreamerSubsystem.h"

#define LOCTEXT_NAMESPACE "AccountReputationListItem"

void UAccountReputationListItem::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);

	const UAccountReputationEntryDataObject* AccountReputationEntryDataObject = Cast<UAccountReputationEntryDataObject>(ListItemObject);
	
	switch (AccountReputationEntryDataObject->GetModerationScope())
	{
	case EModerationScope::Server:
		BanScope->SetText(LOCTEXT("BanScopeServerBan", "Server Ban"));
		break;
	case EModerationScope::Community:
		BanScope->SetText(LOCTEXT("BanScopeCommunityBan", "Community Ban"));
		break;
	case EModerationScope::Game:
		BanScope->SetText(LOCTEXT("BanScopeGameBan", "Game Ban"));
		break;
	case EModerationScope::Global:
		BanScope->SetText(LOCTEXT("BanScopeGlobalBan", "Global Ban"));
		break;
	default: ;
	}
	
	if (AccountReputationEntryDataObject->GetModerationScope() == EModerationScope::Global)
	{
		ServerOrCommunityName->SetVisibility(ESlateVisibility::Collapsed);
	}
	
	if (const UServerDataObject* Server = AccountReputationEntryDataObject->GetServer())
	{
		ServerOrCommunityName->SetText(Server->GetTitle());
	}
	else if (const UCommunityDataObject* Community = AccountReputationEntryDataObject->GetCommunity())
	{
		ServerOrCommunityName->SetText(Community->GetTitle());
	}
	
	ModerationDate->SetText(FText::AsDate(AccountReputationEntryDataObject->GetDateCreated()));

	switch (AccountReputationEntryDataObject->GetModerationDuration())
	{
	case EModerationDuration::Temporary:
		ExpirationText->SetText(FText::AsDate(AccountReputationEntryDataObject->GetDateExpires()));
		break;
	case EModerationDuration::Permanent:
		ExpirationText->SetText(LOCTEXT("PermanentBan", "Permanent"));
		break;
	case EModerationDuration::Expired:
		ExpirationText->SetText(LOCTEXT("ExpiredBan", "Expired"));
		break;
	default: ;
	}

	if (const TSoftClassPtr<UCommonTextStyle> Style = ExpirationTextStyle.FindChecked(AccountReputationEntryDataObject->GetModerationDuration()); Style.IsNull() == false)
	{
		UAssetStreamerSubsystem::Get().LoadAssetAsync(Style.ToSoftObjectPath(), FStreamableDelegate::CreateWeakLambda(this, [this, Style]
		{
			ExpirationText->SetStyle(Style.Get());
		}));
	}
}

#undef LOCTEXT_NAMESPACE
