// Nodecraft, Inc. © 2012-2024, All Rights Reserved.


#include "UI/ServerDetails/ServerModerationHistoryListItem.h"
#include "CommonTextBlock.h"
#include "DeveloperSettings/NodecraftStudioStyleSettings.h"
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
		
		ModerationDate->SetText(FText::FromString(ModerationLogEntryDataObject->GetDateCreated().ToFormattedString(*UNodecraftStudioStyleSettings::GetDateFormat())));

		switch (ModerationLogEntryDataObject->GetModerationDuration())
		{
		case EModerationDuration::Temporary:
			Expires->SetText(FText::FromString(ModerationLogEntryDataObject->GetDateExpires().ToFormattedString(*UNodecraftStudioStyleSettings::GetDateFormat())));
			break;
		case EModerationDuration::Permanent:
			Expires->SetText(LOCTEXT("PermanentBanOrKick", "Permanent"));
			break;
		default: ;
		}

		if (const TSoftClassPtr<UCommonTextStyle> Style = ExpirationTextStyle.FindChecked(ModerationLogEntryDataObject->GetModerationDuration()); Style.IsNull() == false)
		{
			UAssetStreamerSubsystem::Get().LoadAssetAsync(Style.ToSoftObjectPath(), FStreamableDelegate::CreateWeakLambda(this, [this, Style]
			{
				Expires->SetStyle(Style.Get());
			}));
		}
	}
}

#undef LOCTEXT_NAMESPACE