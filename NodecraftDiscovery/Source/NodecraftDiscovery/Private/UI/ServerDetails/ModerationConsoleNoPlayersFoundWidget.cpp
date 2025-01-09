// Nodecraft, Inc. © 2012-2024, All Rights Reserved.


#include "UI/ServerDetails/ModerationConsoleNoPlayersFoundWidget.h"

#include "UI/ServerDetails/ModerationConsolePlayerList.h"

#define LOCTEXT_NAMESPACE "ModerationConsoleNoPlayersFoundWidget"

void UModerationConsoleNoPlayersFoundWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	Super::NativeOnListItemObjectSet(ListItemObject);

	if (const UNoPlayersFoundItemData* NoPlayersFoundItemData = Cast<UNoPlayersFoundItemData>(ListItemObject))
	{
		switch (NoPlayersFoundItemData->GetListType())
		{
		case EModerationConsolePlayerListType::None:
			break;
		case EModerationConsolePlayerListType::Owner:
			break;
		case EModerationConsolePlayerListType::Moderators:
			NoPlayersFoundText->SetText(LOCTEXT("NoModeratorsFound", "When a player is promoted to moderator they will show up here."));
			break;
		case EModerationConsolePlayerListType::OnlinePlayers:
			NoPlayersFoundText->SetText(LOCTEXT("NoOnlinePlayersFound", "When other players join the server, they will show up here."));
			break;
		case EModerationConsolePlayerListType::OfflinePlayers:
			NoPlayersFoundText->SetText(LOCTEXT("NoOfflinePlayersFound", "Once a player leaves your server, they will show up here."));
			break;
		case EModerationConsolePlayerListType::BannedPlayers:
			NoPlayersFoundText->SetText(LOCTEXT("NoBannedPlayersFound", "If you ever ban players, they will show up here."));
			break;
		default:
			break;
		}
	}
}

#undef LOCTEXT_NAMESPACE