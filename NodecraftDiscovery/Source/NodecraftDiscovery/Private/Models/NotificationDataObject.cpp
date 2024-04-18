// Nodecraft, Inc. © 2012-2024, All Rights Reserved.


#include "Models/NotificationDataObject.h"

#include "Services/AllowsService.h"
#include "Services/InvitesService.h"
#include "Services/NotificationsService.h"

FString UNotificationDataObject::GetId() const
{
	return Id;
}

FDateTime UNotificationDataObject::GetDateCreated() const
{
	FDateTime Time;
	FDateTime::ParseIso8601(*DateCreated, Time);
	return Time;
}

ENotificationType UNotificationDataObject::GetType() const
{
	return Type;
}

FString UNotificationDataObject::GetSubject() const
{
	return Subject;
}

UPlayerDataObject* UNotificationDataObject::GetPlayer() const
{
	return Player;
}

UServerDataObject* UNotificationDataObject::GetServer() const
{
	return Server;
}

UBanDataObject* UNotificationDataObject::GetBan() const
{
	return Ban;
}

UCommunityDataObject* UNotificationDataObject::GetCommunity() const
{
	return Community;
}

FServerAllowDataObject UNotificationDataObject::GetAllowData() const
{
	return AllowData;
}

FServerInviteDataObject UNotificationDataObject::GetInviteData() const
{
	return InviteData;
}

ENotificationListType UNotificationDataObject::GetListType() const
{
	return ListType;
}

void UNotificationDataObject::SetListType(const ENotificationListType NewListType)
{
	ListType = NewListType;
}

void UNotificationDataObject::PrimaryAction(FSimpleServiceResponseDelegate& Callback) const
{
	switch (Type)
	{
	case ENotificationType::Allow:
			UAllowsService::Get().AcceptAllow(AllowData.GetId(), Callback);
		break;
	case ENotificationType::Invite:
			UInvitesService::Get().AcceptInvite(InviteData.GetId(), Callback);
		break;
	default:
		// By default there is no primary action to perform
		break;
	}
}

void UNotificationDataObject::DismissAction(FSimpleServiceResponseDelegate& Callback, const UWorld* World) const
{
	switch (Type)
	{
	case ENotificationType::Allow:
		UAllowsService::Get().DeclineAllow(AllowData.GetId(), Callback);	
		break;
	case ENotificationType::Invite:
		UInvitesService::Get().DeclineInvite(InviteData.GetId(), Callback);
		break;
	default:
		if (World)
		{
			UNotificationsService::Get().DismissNotification(World, Id, Callback);
		}
		else
		{
			Callback.ExecuteIfBound(false, FText::FromString("No world provided to dismiss notification"));
		}
		break;
	}
}

UNotificationDataObject* UNotificationDataObject::FromJson(const TSharedRef<FJsonObject>& Json)
{
	UNotificationDataObject* NotificationDataObject = NewObject<UNotificationDataObject>();

	NotificationDataObject->Id = Json->GetStringField("id");
	
	Json->TryGetStringField("date_created", NotificationDataObject->DateCreated);
	
	Json->TryGetStringField("subject", NotificationDataObject->Subject);

	if (const FString TypeString = Json->GetStringField("type"); TypeString == "server_kicks")
	{
		NotificationDataObject->Type = ENotificationType::Kick;
	}
	else if (TypeString == "server_bans")
	{
		NotificationDataObject->Type = ENotificationType::Ban;
	}
	else if (TypeString == "server_allows")
	{
		NotificationDataObject->Type = ENotificationType::Allow;
	}
	else if (TypeString == "server_sessions")
	{
		NotificationDataObject->Type = ENotificationType::Session;
	}
	else if (TypeString == "server_invites")
	{
		NotificationDataObject->Type = ENotificationType::Invite;
	}
	else if (TypeString == "server_moderators")
	{
		NotificationDataObject->Type = ENotificationType::Moderation;
	}

	const TSharedPtr<FJsonObject>* PlayerJson;
	if (Json->TryGetObjectField("player", PlayerJson))
	{
		NotificationDataObject->Player = UPlayerDataObject::FromJson(PlayerJson->ToSharedRef());
	}

	const TSharedPtr<FJsonObject>* ServerJson;
	if (Json->TryGetObjectField("server", ServerJson))
	{
		NotificationDataObject->Server = UServerDataObject::FromJson(ServerJson->ToSharedRef());
	}

	const TSharedPtr<FJsonObject>* BanJson;
	if (Json->TryGetObjectField("ban", BanJson))
	{
		NotificationDataObject->Ban = UBanDataObject::FromJson(BanJson->ToSharedRef());
	}

	const TSharedPtr<FJsonObject>* CommunityJson;
	if (Json->TryGetObjectField("community", CommunityJson))
	{
		NotificationDataObject->Community = UCommunityDataObject::FromJson(CommunityJson->ToSharedRef());
	}

	const TSharedPtr<FJsonObject>* AllowDataJson;
	if (Json->TryGetObjectField("server_allow", AllowDataJson))
	{
		NotificationDataObject->AllowData = FServerAllowDataObject::FromJson(AllowDataJson->ToSharedRef());
	}

	const TSharedPtr<FJsonObject>* InviteDataJson;
	if (Json->TryGetObjectField("server_invite", InviteDataJson))
	{
		NotificationDataObject->InviteData = FServerInviteDataObject::FromJson(InviteDataJson->ToSharedRef());
	}

	return NotificationDataObject;
}
