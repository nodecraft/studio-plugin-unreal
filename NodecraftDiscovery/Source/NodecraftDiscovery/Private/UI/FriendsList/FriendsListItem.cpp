// Nodecraft, Inc. © 2012-2024, All Rights Reserved.


#include "UI/FriendsList/FriendsListItem.h"

#include "CommonBorder.h"
#include "CommonTextBlock.h"
#include "Models/FriendDataObject.h"
#include "Models/PlayerDataObject.h"
#include "Models/ServerDataObject.h"
#include "Services/InvitesService.h"
#include "Services/ServersService.h"
#include "Subsystems/AssetStreamerSubsystem.h"
#include "Subsystems/MenuManagerSubsystem.h"
#include "UI/Common/AsyncImage.h"
#include "UI/Foundation/NodecraftButtonBase.h"
#include "UI/ServerInvites/ServerInvitesModal.h"

#define LOCTEXT_NAMESPACE "FriendsListItem"

void UFriendsListItem::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);

	const UFriendDataObject* FriendDataObject = Cast<UFriendDataObject>(ListItemObject);
	
	if (const UPlayerDataObject* Player = FriendDataObject->GetPlayer())
	{
		FriendProfileImage->LoadPlayerAvatarAsync(Player);
		
        FriendNameTextBlock->SetText(Player->GetUsername().IsEmpty() ?
        	FText::FromString(Player->GetIdent()) : FText::FromString(Player->GetUsername()));
		FriendStatusTextBlock->SetText(FriendDataObject->GetStatus());

		// load and set the style for status text
		if (const TSoftClassPtr<UCommonTextStyle> Style = StatusStyle.FindChecked(FriendDataObject->GetStatus().ToString()); Style.IsNull() == false)
		{
			UAssetStreamerSubsystem::Get().LoadAssetAsync(Style.ToSoftObjectPath(), FStreamableDelegate::CreateWeakLambda(this, [this, Style]
			{
				FriendStatusTextBlock->SetStyle(Style.Get());
			}));
		}
	}
	
	JoinServerCardContainer->SetVisibility(FriendDataObject->GetServer() ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	if (UServerDataObject* Server = FriendDataObject->GetServer())
	{
		const FText PlayersOnline = FText::Format(LOCTEXT("OnlinePlayersCount", "{0} / {1} Players Online"),
												  Server->GetPlayersCount(), Server->GetPlayersMax());
		PlayersOnlineText->SetText(PlayersOnline);
		ServerName->SetText(Server->GetTitle());

		JoinButton->OnClicked().Clear();
		JoinButton->OnClicked().AddWeakLambda(this, [this, Server]
		{
			UServersService::Get().JoinServer(Server, GetWorld());
		});
	}
	
	InviteToPlayButton->OnClicked().Clear();
	InviteToPlayButton->OnClicked().AddWeakLambda(this, [this, FriendDataObject]
	{
		if (ServerInvitesModalClass.IsNull() == false)
		{
			UAssetStreamerSubsystem::Get().LoadAssetAsync(ServerInvitesModalClass.ToSoftObjectPath(), FStreamableDelegate::CreateWeakLambda(this, [this, FriendDataObject]
			{
				FOnListInvitableServers OnListInvitableServers;
				OnListInvitableServers.BindWeakLambda(this, [this, FriendDataObject](TArray<UServerDataObject*> Servers, bool bSuccess, TOptional<FText> Error)
				{
					if (bSuccess && Servers.Num() > 0)
					{
						UMenuManagerSubsystem::Get().ShowServerInvites(ServerInvitesModalClass.Get(), Servers, FriendDataObject);
					}
				});
				UInvitesService::Get().ListInvitableServers(FriendDataObject->GetPlayer()->GetId(), OnListInvitableServers);
			}));
		}
	});
}

#undef LOCTEXT_NAMESPACE