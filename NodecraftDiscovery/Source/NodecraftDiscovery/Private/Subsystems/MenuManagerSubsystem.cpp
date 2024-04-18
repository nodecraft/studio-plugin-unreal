// Nodecraft, Inc. © 2012-2024, All Rights Reserved.


#include "Subsystems/MenuManagerSubsystem.h"

#include "DeveloperSettings/NodecraftStudioWidgetSettings.h"
#include "Models/ServerQueueTokenDataObject.h"
#include "Services/ServerQueueService.h"
#include "Subsystems/AssetStreamerSubsystem.h"
#include "UI/ServerDetails/ServerDetailsModal.h"
#include "UI/ServerDetails/ServerPasswordModal.h"

void UMenuManagerSubsystem::ShowServerDetails(UServerDataObject* ServerDataObject, TOptional<FText> RulesError)
{
	OnRequestShowServerDetailsModal.ExecuteIfBound(ServerDataObject, RulesError);
}

void UMenuManagerSubsystem::ShowPlayerConsents(TSubclassOf<UCommonActivatableWidget> Modal, const FPlayerConsents& Consents)
{
	OnPushConsentsPopupDelegate.ExecuteIfBound(Modal, Consents);
}

void UMenuManagerSubsystem::ShowServerInvites(TSubclassOf<UCommonActivatableWidget> Modal, const TArray<UServerDataObject*>& Servers, const UFriendDataObject* Invitee)
{
	OnPushServerInvitesPopupDelegate.ExecuteIfBound(Modal, Servers, Invitee);
}

void UMenuManagerSubsystem::ShowJoiningServerQueue()
{
	OnPushJoiningServerQueuePopupDelegate.ExecuteIfBound();
}

void UMenuManagerSubsystem::ShowServerPasswordModal(UServerDataObject* ServerDataObject)
{
	OnPushServerPasswordPopupDelegate.ExecuteIfBound(ServerDataObject);
}

void UMenuManagerSubsystem::ShowCreateServerModal(TSoftClassPtr<UCommonActivatableWidget> ModalClass)
{
	if (ModalClass.IsNull() == false)
	{
		FStreamableDelegate OnLoaded;
		OnLoaded.BindWeakLambda(this, [this, ModalClass]
		{
			OnPushCreateServerPopupDelegate.ExecuteIfBound(ModalClass.Get());
		});
		UAssetStreamerSubsystem::Get().LoadAssetAsync(ModalClass.ToSoftObjectPath(), OnLoaded);
	}
}

void UMenuManagerSubsystem::ShowRedirectModal(FString QrCodeUrl, ELinkType LinkType)
{
	if (UNodecraftStudioWidgetSettings::Get().GetExternalRedirectModal().IsNull() == false)
	{
		FStreamableDelegate OnLoaded;
		OnLoaded.BindWeakLambda(this, [this, QrCodeUrl, LinkType]
		{
			OnPushExternalRedirectModalDelegate.ExecuteIfBound(UNodecraftStudioWidgetSettings::Get().GetExternalRedirectModal().Get(), QrCodeUrl, LinkType);
		});
		UAssetStreamerSubsystem::Get().LoadAssetAsync(UNodecraftStudioWidgetSettings::Get().GetExternalRedirectModal().ToSoftObjectPath(), OnLoaded);
	}
}


void UMenuManagerSubsystem::ShowInternalRedirectModal(EPlayerConnectionSubject Subject)
{
	if (UNodecraftStudioWidgetSettings::Get().GetInternalRedirectModal().IsNull() == false)
	{
		FStreamableDelegate OnLoaded;
		OnLoaded.BindWeakLambda(this, [this, Subject]
		{
			OnPushInternalRedirectPopupDelegate.ExecuteIfBound(UNodecraftStudioWidgetSettings::Get().GetInternalRedirectModal().Get(), Subject, TOptional<FString>());
		});
		UAssetStreamerSubsystem::Get().LoadAssetAsync(UNodecraftStudioWidgetSettings::Get().GetInternalRedirectModal().ToSoftObjectPath(), OnLoaded);
	}
}

void UMenuManagerSubsystem::ShowServerSettingsRedirectModal(const FString& ServerId)
{
	if (UNodecraftStudioWidgetSettings::Get().GetInternalRedirectModal().IsNull() == false)
	{
		FStreamableDelegate OnLoaded;
		OnLoaded.BindWeakLambda(this, [this, ServerId]
		{
			OnPushInternalRedirectPopupDelegate.ExecuteIfBound(UNodecraftStudioWidgetSettings::Get().GetInternalRedirectModal().Get(), EPlayerConnectionSubject::ServerSettings, ServerId);
		});
		UAssetStreamerSubsystem::Get().LoadAssetAsync(UNodecraftStudioWidgetSettings::Get().GetInternalRedirectModal().ToSoftObjectPath(), OnLoaded);
	}
}



