// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystems/MenuManagerSubsystem.h"

#include "DeveloperSettings/DiscoveryWidgetSettings.h"
#include "Models/ServerQueueTokenDataObject.h"
#include "Services/ServerQueueService.h"
#include "Subsystems/AssetStreamerSubsystem.h"
#include "UI/ServerDetails/ServerDetailsModal.h"
#include "UI/ServerDetails/ServerPasswordModal.h"

void UMenuManagerSubsystem::ShowServerDetails(UServerDataObject* ServerDataObject)
{
	OnRequestShowServerDetailsModal.ExecuteIfBound(ServerDataObject);
}

void UMenuManagerSubsystem::ShowPlayerConsents(TSubclassOf<UCommonActivatableWidget> Modal, const FPlayerConsents& Consents)
{
	OnPushConsentsPopupDelegate.ExecuteIfBound(Modal, Consents);
}

void UMenuManagerSubsystem::ShowServerInvites(TSubclassOf<UCommonActivatableWidget> Modal, const TArray<UServerDataObject*>& Servers, const UFriendDataObject* Invitee)
{
	OnPushServerInvitesPopupDelegate.ExecuteIfBound(Modal, Servers, Invitee);
}

void UMenuManagerSubsystem::ShowJoiningServerQueue(const UWorld* World, const UServerDataObject* ServerDataObject, FString Password)
{
	FJoinServerDelegate OnJoinServerComplete;
	OnJoinServerComplete.BindWeakLambda(this, [this, World]
		(UServerQueueTokenDataObject* QueueTokenDataObject, bool bSuccess, TOptional<FText> Error)
	{
		if (bSuccess && QueueTokenDataObject)
		{
			FGetServerQueueDelegate OnGetServerQueueComplete;
			OnGetServerQueueComplete.BindWeakLambda(this, [this, QueueTokenDataObject](UServerQueueDataObject* ServerQueueDataObject, bool bSuccess, TOptional<FText> Error)
			{
				if (bSuccess)
				{
					UAssetStreamerSubsystem::Get().LoadAssetAsync(UDiscoveryWidgetSettings::Get().GetJoiningServerQueueModal().ToSoftObjectPath(),
						FStreamableDelegate::CreateWeakLambda(this, [this, ServerQueueDataObject, QueueTokenDataObject]
					{
						OnPushJoiningServerQueuePopupDelegate.ExecuteIfBound(ServerQueueDataObject, QueueTokenDataObject);
					}));
				}
			});
			UServerQueueService::Get(World)->GetServerQueue(QueueTokenDataObject->GetToken(), OnGetServerQueueComplete);
		}
		else
		{
			OnServerQueueError.ExecuteIfBound(Error.GetValue());
		}
	});
	UServerQueueService::Get(World)->JoinServer(ServerDataObject->GetId(), Password, OnJoinServerComplete);
	
}

void UMenuManagerSubsystem::ShowServerPasswordModal(const UServerDataObject* ServerDataObject)
{
	OnPushServerPasswordPopupDelegate.ExecuteIfBound(ServerDataObject);
}

