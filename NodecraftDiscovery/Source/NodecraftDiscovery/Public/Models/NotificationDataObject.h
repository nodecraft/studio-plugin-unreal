// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BanDataObject.h"
#include "CommunityDataObject.h"
#include "ExpirableDataObject.h"
#include "ServerDataObject.h"
#include "API/NotificationListType.h"
#include "Services/ServiceDelegates.h"
#include "UObject/Object.h"
#include "NotificationDataObject.generated.h"

UENUM()
enum class ENotificationType : uint8
{
	Kick,
	Ban,
	Allow,
	Session,
	Invite,
	Moderation,
};

/**
 * 
 */
UCLASS()
class NODECRAFTDISCOVERY_API UNotificationDataObject : public UObject
{
	GENERATED_BODY()

public:
	FString GetId() const;
	FDateTime GetDateCreated() const;
	ENotificationType GetType() const;
	FString GetSubject() const;
	UPlayerDataObject* GetPlayer() const;
	UServerDataObject* GetServer() const;
	UBanDataObject* GetBan() const;
	UCommunityDataObject* GetCommunity() const;
	FServerAllowDataObject GetAllowData() const;
	FServerInviteDataObject GetInviteData() const;
	ENotificationListType GetListType() const;
	void SetListType(ENotificationListType NewListType);

	void PrimaryAction(FSimpleServiceResponseDelegate& Callback) const;
	void DismissAction(FSimpleServiceResponseDelegate& Callback, const UWorld* World) const;
	ENotificationListType CanAccept();

	static UNotificationDataObject* FromJson(const TSharedRef<FJsonObject>& Json);

private:
	FString Id;
	FString DateCreated;
	ENotificationType Type;
	FString Subject;

	// Player who sent an Allow or Invite, or the player who joined a server in the case of a Session notification
	UPROPERTY()
	UPlayerDataObject* Player;

	// The server this notification is for
	UPROPERTY()
	UServerDataObject* Server;

	// Only present if Type == Ban
	UPROPERTY()
	UBanDataObject* Ban;

	// Only present if related to a Community
	UPROPERTY()
	UCommunityDataObject* Community;

	// Only present if Type == Allow
	FServerAllowDataObject AllowData;

	// Only present if Type == Invite
	FServerInviteDataObject InviteData;

	// Used to determine which list this notification belongs to. This is used to determine various styling
	ENotificationListType ListType;
};
