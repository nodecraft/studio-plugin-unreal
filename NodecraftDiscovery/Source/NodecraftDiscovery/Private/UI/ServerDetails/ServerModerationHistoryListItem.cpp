// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ServerDetails/ServerModerationHistoryListItem.h"
#include "CommonTextBlock.h"
#include "DeveloperSettings/DiscoveryStyleSettings.h"
#include "Models/ModerationLogEntryDataObject.h"
#include "Models/PlayerDataObject.h"
#include "Subsystems/AssetStreamerSubsystem.h"

#define LOCTEXT_NAMESPACE "ServerDetailsModerationListItem"

void UServerModerationHistoryListItem::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);

	const UModerationLogEntryDataObject* ModerationLogEntryDataObject = Cast<UModerationLogEntryDataObject>(ListItemObject);
	if (const UPlayerDataObject* Player = ModerationLogEntryDataObject->GetPlayer())
	{
		Username->SetText(Player->GetUsername().IsEmpty() ? FText::FromString(Player->GetIdent()) : FText::FromString(Player->GetUsername()));
		
		ModerationDate->SetText(FText::FromString(ModerationLogEntryDataObject->GetDateCreated().ToFormattedString(*UDiscoveryStyleSettings::GetDateFormat())));

		switch (ModerationLogEntryDataObject->GetModerationDuration())
		{
		case EModerationDuration::Temporary:
			Expires->SetText(FText::FromString(ModerationLogEntryDataObject->GetDateExpires().ToFormattedString(*UDiscoveryStyleSettings::GetDateFormat())));
			break;
		case EModerationDuration::Permanent:
			Expires->SetText(LOCTEXT("PermanentBanOrKick", "Permanent"));
			break;
		default: ;
		}

		if (const TSoftClassPtr<UCommonTextStyle> Style = ExpirationTextStyle.FindChecked(ModerationLogEntryDataObject->GetModerationDuration()); Style.IsNull() == false)
		{
			UAssetStreamerSubsystem::Get().LoadAssetAsync(Style.ToSoftObjectPath(), FStreamableDelegate::CreateLambda([this, Style]
			{
				Expires->SetStyle(Style.Get());
			}));
		}
	}
}

#undef LOCTEXT_NAMESPACE