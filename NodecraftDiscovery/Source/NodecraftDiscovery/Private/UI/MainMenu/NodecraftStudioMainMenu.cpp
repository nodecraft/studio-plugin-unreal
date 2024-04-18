// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#include "UI/MainMenu/NodecraftStudioMainMenu.h"

#include "DataTypes/LinkTypes.h"
#include "DeveloperSettings/NodecraftStudioWidgetSettings.h"
#include "Services/GameService.h"
#include "Stores/AllowsStore.h"
#include "Subsystems/AssetStreamerSubsystem.h"
#include "Subsystems/MenuManagerSubsystem.h"
#include "Subsystems/RemoteImageSubsystem.h"
#include "UI/Common/AsyncImage.h"
#include "UI/CreateServer/CreateServerModal.h"
#include "UI/Redirects/InternalRedirectModal.h"
#include "UI/Redirects/RedirectModal.h"
#include "UI/ServerDetails/JoiningServerQueueModal.h"
#include "UI/ServerDetails/ServerDetailsModal.h"
#include "UI/ServerDetails/ServerPasswordModal.h"
#include "UI/ServerInvites/ServerInvitesModal.h"
#include "UI/Settings/TermsOfServiceModal.h"

void UNodecraftStudioMainMenu::NativeConstruct()
{
	Super::NativeConstruct();

	GameBackground->LoadImageAsync(UGameService::Get().GetCachedGameBackgroundURL(), NC_CACHED_GAME_BACKGROUND_FILENAME);

	PopupMenuStackBackground->SetVisibility(ESlateVisibility::Collapsed);
	MenuStackBackground->SetVisibility(ESlateVisibility::Collapsed);
	
	PopupMenuStack->OnDisplayedWidgetChanged().AddWeakLambda(this, [this](UCommonActivatableWidget* NewWidget)
	{
		PopupMenuStackBackground->SetVisibility(PopupMenuStack->GetNumWidgets() > 0 ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	});

	MenuStack->OnDisplayedWidgetChanged().AddWeakLambda(this, [this](UCommonActivatableWidget* NewWidget)
	{
		MenuStackBackground->SetVisibility(MenuStack->GetNumWidgets() > 0 ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	});
	
	UMenuManagerSubsystem::Get().OnRequestShowServerDetailsModal.BindWeakLambda(this, [this](UServerDataObject* ServerDataObject, TOptional<FText> RulesError)
	{
		const bool bIsShowingServerDetailsModal = PopupMenuStack->GetActiveWidget() && PopupMenuStack->GetActiveWidget()->IsA(ServerDetailsClass);
		// If the widget being shown is the server details modal, tell it to move to the rules tab if there is an error
		if (bIsShowingServerDetailsModal)
		{
			if (RulesError.IsSet())
			{
				UServerDetailsModal* ServerDetailsModal = Cast<UServerDetailsModal>(PopupMenuStack->GetActiveWidget());
				ServerDetailsModal->NavigateToRulesTabWithError(RulesError.GetValue());
			}
			return;
		}

		// Otherwise, if we are not showing the server details modal, one of two things will happen
		// If the popup menu stack has any widgets, we want to clear them and show the server details modal
		// However, the stack will not clear for one frame. See bRemoveDisplayedWidgetPostTransition in UCommonActivatableWidgetStack,
		// so we need to wait for the next frame and then try to show the server details modal again, which will result in this same code path
		// If the popup menu stack is empty, we can show the server details modal immediately
		if (PopupMenuStack->GetNumWidgets() > 0)
		{
			PopupMenuStack->ClearWidgets();
			GetWorld()->GetTimerManager().SetTimerForNextTick(FTimerDelegate::CreateWeakLambda(this, [this, ServerDataObject, RulesError]
			{
				UMenuManagerSubsystem::Get().ShowServerDetails(ServerDataObject, RulesError);
			}));
		}
		else
		{
			UServerDetailsModal* ServerDetailsModal = Cast<UServerDetailsModal>(PopupMenuStack->AddWidget(ServerDetailsClass));
			if (ServerDetailsModal)
			{
				ServerDetailsModal->LoadServerDetails(ServerDataObject);
				ServerDetailsModal->SetCloseDelegate(FSimpleDelegate::CreateWeakLambda(this, [this, ServerDetailsModal]
				{
					if (ServerDetailsModal)
					{
						PopupMenuStack->RemoveWidget(*ServerDetailsModal);
						UAllowsStore::Get().ClearAllowsForCurrentServer();
					}
				}));
				if (RulesError.IsSet())
				{
					ServerDetailsModal->NavigateToRulesTabWithError(RulesError.GetValue());
				}
			}
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
			FSimpleDelegate OnPopupClosed;
			OnPopupClosed.BindWeakLambda(this, [this, ServerInvitesModal]
			{
				PopupMenuStack->RemoveWidget(*ServerInvitesModal);
			});
			ServerInvitesModal->Configure(Servers, Invitee, OnPopupClosed);
		}
	});

	UMenuManagerSubsystem::Get().OnPushJoiningServerQueuePopupDelegate.BindWeakLambda(this, [this]()
	{
		FStreamableDelegate OnLoaded;
		OnLoaded.BindWeakLambda(this, [this]
		{
			UCommonActivatableWidget* Modal = PopupMenuStack->AddWidget(UNodecraftStudioWidgetSettings::Get().GetJoiningServerQueueModal().Get());
	        if (UJoiningServerQueueModal* JoiningServerQueueModal = Cast<UJoiningServerQueueModal>(Modal))
	        {
	            FSimpleDelegate OnPopupClosed;
	            OnPopupClosed.BindWeakLambda(this, [this, JoiningServerQueueModal]
	            {
            		PopupMenuStack->RemoveWidget(*JoiningServerQueueModal);
	            });
	            JoiningServerQueueModal->Configure(OnPopupClosed);
	        }
		});
		UAssetStreamerSubsystem::Get().LoadAssetAsync(UNodecraftStudioWidgetSettings::Get().GetJoiningServerQueueModal().ToSoftObjectPath(), OnLoaded);
	});

	UMenuManagerSubsystem::Get().OnPushServerPasswordPopupDelegate.BindWeakLambda(this, [this](UServerDataObject* Server)
	{
		FStreamableDelegate OnLoaded;
		OnLoaded.BindWeakLambda(this, [this, Server]
		{
			UCommonActivatableWidget* Modal = PopupMenuStack->AddWidget(UNodecraftStudioWidgetSettings::Get().GetServerPasswordModal().Get());
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
		UAssetStreamerSubsystem::Get().LoadAssetAsync(UNodecraftStudioWidgetSettings::Get().GetServerPasswordModal().ToSoftObjectPath(), OnLoaded);
	});

	UMenuManagerSubsystem::Get().OnPushCreateServerPopupDelegate.BindWeakLambda(this, [this](TSubclassOf<UCommonActivatableWidget> ModalClass)
	{
		UCommonActivatableWidget* Modal = PopupMenuStack->AddWidget(ModalClass);
		if (UCreateServerModal* CreateServerModal = Cast<UCreateServerModal>(Modal))
		{
			FSimpleDelegate OnPopupClosed;
			OnPopupClosed.BindWeakLambda(this, [this, CreateServerModal]
			{
				PopupMenuStack->RemoveWidget(*CreateServerModal);
			});
			CreateServerModal->Configure(OnPopupClosed);
		}
	});

	UMenuManagerSubsystem::Get().OnPushExternalRedirectModalDelegate.BindWeakLambda(this, [this]
	(TSubclassOf<UCommonActivatableWidget> ModalClass, FString QrCodeUrl, ELinkType LinkType)
	{
		UCommonActivatableWidget* Modal = PopupMenuStack->AddWidget(ModalClass);
		if (URedirectModal* RedirectModal = Cast<URedirectModal>(Modal))
		{
			FSimpleDelegate OnPopupClosed;
			OnPopupClosed.BindWeakLambda(this, [this, RedirectModal]
			{
				PopupMenuStack->RemoveWidget(*RedirectModal);
			});
			RedirectModal->Configure(OnPopupClosed, QrCodeUrl, LinkType);
		}
	});

	UMenuManagerSubsystem::Get().OnPushInternalRedirectPopupDelegate.BindWeakLambda(this, [this]
		(TSubclassOf<UCommonActivatableWidget> ModalClass, EPlayerConnectionSubject Subject, TOptional<FString> ServerId)
	{
		UCommonActivatableWidget* Modal = PopupMenuStack->AddWidget(ModalClass);
		if (UInternalRedirectModal* InternalRedirectModal = Cast<UInternalRedirectModal>(Modal))
		{
			FSimpleDelegate OnPopupClosed;
			OnPopupClosed.BindWeakLambda(this, [this, InternalRedirectModal]
			{
				PopupMenuStack->RemoveWidget(*InternalRedirectModal);
			});
			InternalRedirectModal->Configure(OnPopupClosed, Subject, ServerId);
		}
	});

	UServerQueueService::Get(GetWorld())->OnUserMustAcceptServerConsents.AddWeakLambda(this, [this](UServerDataObject* Server, FText ErrorMsg)
	{
		UMenuManagerSubsystem::Get().ShowServerDetails(Server, ErrorMsg);
	});
}

UServerList* UNodecraftStudioMainMenu::GetFavoriteServersList() const
{
	return FavoriteServers;
}
