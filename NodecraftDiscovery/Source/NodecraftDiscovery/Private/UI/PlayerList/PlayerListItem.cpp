// Nodecraft, Inc. © 2012-2024, All Rights Reserved.


#include "UI/PlayerList/PlayerListItem.h"

#include "DeveloperSettings/NodecraftStudioIdentitySettings.h"
#include "Models/PlayerDataObject.h"

#define LOCTEXT_NAMESPACE "PlayerListItem"

void UPlayerListItem::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);

	const UPlayerDataObject* PlayerDataObject = Cast<UPlayerDataObject>(ListItemObject);

	PlayerProfileImage->LoadPlayerAvatarAsync(PlayerDataObject);
	
	// show player username or ident (if no username is set)
	const FString PlayerName = PlayerDataObject->GetUsername().IsEmpty() ? PlayerDataObject->GetIdent() : PlayerDataObject->GetUsername();
	PlayerNameTextBlock->SetText(FText::FromString(PlayerName));

	// "Started playing X ago" or "Last seen X ago"
	if (PlayerDataObject->IsOnline())
	{
		UpdateTimestamp(PlayerDataObject->GetDateStart(), true);
	}
	else
	{
		UpdateTimestamp(PlayerDataObject->GetDateEnd(), false);
	}

	UTexture2D* IdentTexture = UNodecraftStudioIdentitySettings::Get().GetIconForType(PlayerDataObject->GetIdentType());
	PlatformIcon->SetBrushFromTexture(IdentTexture);
}

void UPlayerListItem::UpdateTimestamp(const FDateTime& Time, bool bIsStartTime)
{
	const FTimespan TimeSinceStartedPlaying = FDateTime::UtcNow() - Time;
	
	const FText Prefix = bIsStartTime ? LOCTEXT("StartedPlaying", "Started playing") : LOCTEXT("LastSeen", "Last seen");

	if (TimeSinceStartedPlaying.GetMinutes() < 1)
	{
		PlayerStatusTextBlock->SetText(FText::Format(LOCTEXT("ActiveTimeLessThanAMinute", "{0} less than a minute ago"), Prefix));
	}
	else if (TimeSinceStartedPlaying.GetHours() < 1)
	{
		PlayerStatusTextBlock->SetText(FText::Format(LOCTEXT("ActiveTimeMinutes", "{0} {1} minutes ago"), Prefix, TimeSinceStartedPlaying.GetMinutes()));
	}
	else if (TimeSinceStartedPlaying.GetDays() < 1)
	{
		PlayerStatusTextBlock->SetText(FText::Format(LOCTEXT("ActiveTimeHoursAndMinutes", "{0} {1} hours and {2} minutes ago"),
			Prefix, TimeSinceStartedPlaying.GetHours(), TimeSinceStartedPlaying.GetMinutes()));
	}
	else
	{
		PlayerStatusTextBlock->SetText(FText::Format(LOCTEXT("ActiveTimeDays", "{0} {1} days ago"), Prefix, TimeSinceStartedPlaying.GetDays()));
	}
}

#undef LOCTEXT_NAMESPACE
