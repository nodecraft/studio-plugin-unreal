// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/MainMenu/DiscoveryMainMenu.h"

#include "DeveloperSettings/DiscoveryWidgetSettings.h"
#include "Subsystems/AssetStreamerSubsystem.h"
#include "Subsystems/MenuManagerSubsystem.h"
#include "UI/ServerDetails/JoiningServerQueueModal.h"
#include "UI/ServerDetails/ServerDetailsModal.h"
#include "UI/ServerDetails/ServerPasswordModal.h"
#include "UI/ServerInvites/ServerInvitesModal.h"
#include "UI/Settings/TermsOfServiceModal.h"

void UDiscoveryMainMenu::NativeConstruct()
{
	Super::NativeConstruct();

	UMenuManagerSubsystem::Get().OnRequestShowServerDetailsModal.BindWeakLambda(this, [this](UServerDataObject* ServerDataObject)
	{
		auto *That = PopupMenuStack->AddWidget(ServerDetailsClass);
		UServerDetailsModal* ServerDetailsModal = Cast<UServerDetailsModal>(That);
		if (ServerDetailsModal)
		{
			ServerDetailsModal->LoadServerDetails(ServerDataObject);
		}
	});

	UMenuManagerSubsystem::Get().OnPushConsentsPopupDelegate.BindWeakLambda(this, [this](TSubclassOf<UCommonActivatableWidget> ModalClass, FPlayerConsents Consents)
	{
		UCommonActivatableWidget* Modal = PopupMenuStack->AddWidget(ModalClass);
		if (UTermsOfServiceModal* TermsOfServiceModal = Cast<UTermsOfServiceModal>(Modal))
		{
			FSimpleDelegate OnPopupClosed;
			OnPopupClosed.BindWeakLambda(this, [this, TermsOfServiceModal]
			{
				if (TermsOfServiceModal)
				{
					PopupMenuStack->RemoveWidget(*TermsOfServiceModal);
				}
			});
			TermsOfServiceModal->Configure(Consents, OnPopupClosed);
		}
	});

	UMenuManagerSubsystem::Get().OnPushServerInvitesPopupDelegate.BindWeakLambda(this, [this]
		(TSubclassOf<UCommonActivatableWidget> ModalClass, TArray<UServerDataObject*> Servers, const UFriendDataObject* Invitee)
	{
		UCommonActivatableWidget* Modal = PopupMenuStack->AddWidget(ModalClass);
		if (UServerInvitesModal* ServerInvitesModal = Cast<UServerInvitesModal>(Modal))
		{
			// TODO: menu creates modal's OnClose callback and passes it in instead of passing in the menu stack
			ServerInvitesModal->Configure(Servers, Invitee, PopupMenuStack);
		}
	});

	UMenuManagerSubsystem::Get().OnPushJoiningServerQueuePopupDelegate.BindWeakLambda(this, [this]
		(const UServerQueueDataObject* ServerQueueDataObject, const UServerQueueTokenDataObject* ServerQueueTokenDataObject)
	{
		
		FStreamableDelegate OnLoaded;
		OnLoaded.BindWeakLambda(this, [this, ServerQueueDataObject, ServerQueueTokenDataObject]
		{
			UCommonActivatableWidget* Modal = PopupMenuStack->AddWidget(UDiscoveryWidgetSettings::Get().GetJoiningServerQueueModal().Get());
	        if (UJoiningServerQueueModal* JoiningServerQueueModal = Cast<UJoiningServerQueueModal>(Modal))
	        {
	            FSimpleDelegate OnPopupClosed;
	            OnPopupClosed.BindWeakLambda(this, [this, JoiningServerQueueModal]
	            {
            		PopupMenuStack->RemoveWidget(*JoiningServerQueueModal);
	            });
	            JoiningServerQueueModal->Configure(ServerQueueDataObject, ServerQueueTokenDataObject, OnPopupClosed);
	        }
		});
		UAssetStreamerSubsystem::Get().LoadAssetAsync(UDiscoveryWidgetSettings::Get().GetJoiningServerQueueModal().ToSoftObjectPath(), OnLoaded);
	});

	UMenuManagerSubsystem::Get().OnPushServerPasswordPopupDelegate.BindWeakLambda(this, [this](const UServerDataObject* Server)
	{
		FStreamableDelegate OnLoaded;
		OnLoaded.BindWeakLambda(this, [this, Server]
		{
			UCommonActivatableWidget* Modal = PopupMenuStack->AddWidget(UDiscoveryWidgetSettings::Get().GetServerPasswordModal().Get());
            if (UServerPasswordModal* ServerPasswordModal = Cast<UServerPasswordModal>(Modal))
            {
            	FSimpleDelegate OnPopupClosed;
            	OnPopupClosed.BindWeakLambda(this, [this, Server, ServerPasswordModal]()
            	{
            		PopupMenuStack->RemoveWidget(*ServerPasswordModal);
            	});
            	ServerPasswordModal->Configure(Server, OnPopupClosed);
            }
		});
		UAssetStreamerSubsystem::Get().LoadAssetAsync(UDiscoveryWidgetSettings::Get().GetServerPasswordModal().ToSoftObjectPath(), OnLoaded);
	});
}

UServerList* UDiscoveryMainMenu::GetFavoriteServersList() const
{
	return FavoriteServers;
}
