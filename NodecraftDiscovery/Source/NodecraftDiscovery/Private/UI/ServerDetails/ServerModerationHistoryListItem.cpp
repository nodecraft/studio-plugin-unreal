// Nodecraft, Inc. © 2012-2024, All Rights Reserved.


#include "UI/ServerDetails/ServerModerationHistoryListItem.h"
#include "CommonTextBlock.h"
#include "PlayerPlatformIcon.h"
#include "Models/ModerationLogEntryDataObject.h"
#include "Models/PlayerDataObject.h"
#include "Subsystems/AssetStreamerSubsystem.h"
#include "DataTypes/ModerationTypes.h"

#define LOCTEXT_NAMESPACE "ServerDetailsModerationListItem"

void UServerModerationHistoryListItem::UpdateExpirationTextStyle(EModerationDuration ModerationDuration)
{
	if (const TSoftClassPtr<UCommonTextStyle> Style = ExpirationTextStyle.FindChecked(ModerationDuration); Style.IsNull() == false)
	{
		UAssetStreamerSubsystem::Get().LoadAssetAsync(Style.ToSoftObjectPath(), FStreamableDelegate::CreateWeakLambda(this, [this, Style]
		{
			Expires->SetStyle(Style.Get());
		}));
	}
}

void UServerModerationHistoryListItem::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);

	const UModerationLogEntryDataObject* ModerationLogEntryDataObject = Cast<UModerationLogEntryDataObject>(ListItemObject);
	if (const UPlayerDataObject* Player = ModerationLogEntryDataObject->GetPlayer())
	{
		Username->SetText(Player->GetUsername().IsEmpty() ? FText::FromString(Player->GetIdent()) : FText::FromString(Player->GetUsername()));
		IdentImage->SetIdentity(Player->GetIdentType());

		const FText ModerationTypeText = ModerationLogEntryDataObject->GetType() == EModerationAction::Ban ? LOCTEXT("ModerationTypeBan", "Ban") : LOCTEXT("ModerationTypeKick", "Kick");
		ModerationType->SetText(ModerationTypeText);

		ModerationReason->SetText(FText::FromString(ModerationLogEntryDataObject->GetReason()));
		
		ModerationDate->SetText(FText::AsDate(ModerationLogEntryDataObject->GetDateCreated()));

		if (ModerationLogEntryDataObject->GetType() == EModerationAction::Kick)
		{
			Expires->SetText(FText::FromString("--"));
			UpdateExpirationTextStyle(EModerationDuration::Undefined);
		}
		else
		{
			switch (ModerationLogEntryDataObject->GetModerationDuration())
			{
			case EModerationDuration::Temporary:
				{
					const FTimespan TimeLeft = ModerationLogEntryDataObject->GetDateExpires() - FDateTime::UtcNow();
					if (TimeLeft < FTimespan::FromMinutes(1))
					{
                        Expires->SetText(LOCTEXT("ExpiresInLessThanMinute", "in less than a minute"));
					}
					else if (TimeLeft < FTimespan::FromHours(1))
					{
						Expires->SetText(FText::FormatOrdered(LOCTEXT("ExpiresInMinutes", "in {0} {0}|plural(one=minute, other=minutes)"), TimeLeft.GetMinutes()));
					}
					else if (TimeLeft < FTimespan::FromDays(1))
					{
						Expires->SetText(FText::FormatOrdered(
							LOCTEXT("ExpiresInHoursAndMinutes",
							        "in {0} {0}|plural(one=hour, other=hours) {1} {1}|plural(one=minute, other=minutes)"),
							TimeLeft.GetHours(), TimeLeft.GetMinutes()));
					}
					else if (TimeLeft <= FTimespan::FromDays(7))
					{
						Expires->SetText(FText::FormatOrdered(
							LOCTEXT("ExpiresInDaysAndHours",
							        "in {0} {0}|plural(one=day, other=days) {1} {1}|plural(one=hour, other=hours)"),
							TimeLeft.GetDays(), TimeLeft.GetHours()));
					}
					else
					{
						Expires->SetText(FText::AsDate(ModerationLogEntryDataObject->GetDateExpires(), EDateTimeStyle::Type::Short));
					}
				}
				break;
			case EModerationDuration::Permanent:
				Expires->SetText(LOCTEXT("PermanentBanOrKick", "Permanent"));
				break;
			case EModerationDuration::Undefined:
				Expires->SetText(LOCTEXT("UndefinedBanOrKick", "Undefined"));
				break;
			case EModerationDuration::Expired:
				Expires->SetText(LOCTEXT("ExpiredBanOrKick", "Expired"));
				break;
			default: ;
			}
			UpdateExpirationTextStyle(ModerationLogEntryDataObject->GetModerationDuration());
		}
	}
}

#undef LOCTEXT_NAMESPACE