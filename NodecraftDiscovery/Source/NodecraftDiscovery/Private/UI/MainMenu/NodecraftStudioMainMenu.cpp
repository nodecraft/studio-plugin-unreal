// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#include "UI/MainMenu/NodecraftStudioMainMenu.h"

#include "CommonInputSubsystem.h"
#include "DataTypes/LinkTypes.h"
#include "DeveloperSettings/NodecraftStudioWidgetSettings.h"
#include "Input/CommonBoundActionBar.h"
#include "Services/GameService.h"
#include "Services/ServersService.h"
#include "Stores/AllowsStore.h"
#include "Stores/SearchStore.h"
#include "Stores/ServersStore.h"
#include "Subsystems/AssetStreamerSubsystem.h"
#include "Subsystems/MenuManagerSubsystem.h"
#include "Subsystems/RemoteImageSubsystem.h"
#include "UI/Common/AsyncImage.h"
#include "UI/CreateServer/CreateServerModal.h"
#include "UI/MainMenu/FloatingMainMenuHeader.h"
#include "UI/MainMenu/SearchResultsPage.h"
#include "UI/Redirects/InternalRedirectModal.h"
#include "UI/Redirects/RedirectModal.h"
#include "UI/ServerDetails/JoiningServerQueueModal.h"
#include "UI/ServerDetails/ServerDetailsModal.h"
#include "UI/ServerDetails/ServerPasswordModal.h"
#include "UI/ServerDetails/ServerQueueIdleCheckModal.h"
#include "UI/ServerInvites/ServerInvitesModal.h"
#include "UI/Settings/TermsOfServiceModal.h"
#include "Utility/NodecraftMacros.h"
#include "TimerManager.h"

void UNodecraftStudioMainMenu::NativeConstruct()
{
	Super::NativeConstruct();

	ServerListsScrollBox->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	SearchResultsPage->SetVisibility(ESlateVisibility::Collapsed);

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
	
	UMenuManagerSubsystem::Get().OnRequestShowServerDetailsModal.AddWeakLambda(this, [this](UServerDataObject* ServerDataObject, TOptional<FServerDetailsArgument> Args)
	{
		const bool bIsShowingServerDetailsModal = PopupMenuStack->GetActiveWidget() && PopupMenuStack->GetActiveWidget()->IsA(ServerDetailsClass);
		// If the widget being shown is the server details modal, tell it to move to the correct tab if requested via Args
		if (bIsShowingServerDetailsModal)
		{
			if (Args.IsSet())
			{
				if (Args.GetValue().Contains("RulesError"))
				{
					UServerDetailsModal* ServerDetailsModal = Cast<UServerDetailsModal>(PopupMenuStack->GetActiveWidget());
                    ServerDetailsModal->NavigateToRulesTabWithError(Args.GetValue().FindChecked("RulesError"));
				}
				else if (Args.GetValue().Contains("Tags"))
				{
					UServerDetailsModal* ServerDetailsModal = Cast<UServerDetailsModal>(PopupMenuStack->GetActiveWidget());
					ServerDetailsModal->NavigateToAboutTab();
				}
				return;
			}
		}

		// Otherwise, if we are not showing the server details modal, one of two things will happen
		// If the popup menu stack has any widgets, we want to clear them and show the server details modal
		// However, the stack will not clear for one frame. See bRemoveDisplayedWidgetPostTransition in UCommonActivatableWidgetStack,
		// so we need to wait for the next frame and then try to show the server details modal again, which will result in this same code path
		// If the popup menu stack is empty, we can show the server details modal immediately
		if (PopupMenuStack->GetNumWidgets() > 0)
		{
			PopupMenuStack->ClearWidgets();
			GetWorld()->GetTimerManager().SetTimerForNextTick(FTimerDelegate::CreateWeakLambda(this, [this, ServerDataObject, Args]
			{
				UMenuManagerSubsystem::Get().ShowServerDetails(ServerDataObject, Args);
			}));
		}
		else
		{
			UServerDetailsModal* ServerDetailsModal = Cast<UServerDetailsModal>(PopupMenuStack->AddWidget(ServerDetailsClass));
			if (ServerDetailsModal)
			{
				ServerDetailsModal->LoadServerDetails(ServerDataObject, Args);
				ServerDetailsModal->SetCloseDelegate(FSimpleDelegate::CreateWeakLambda(this, [this, ServerDetailsModal]
				{
					if (ServerDetailsModal)
					{
						PopupMenuStack->RemoveWidget(*ServerDetailsModal);
						UAllowsStore::Get().ClearAllowsForCurrentServer();
						UServersStore::Get().ClearCurrentServerId();
					}
				}));
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

	UMenuManagerSubsystem::Get().OnPushJoiningServerQueuePopupDelegate.AddWeakLambda(this, [this]()
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

	UMenuManagerSubsystem::Get().OnPushIdleCheckPopupDelegate.BindWeakLambda(
		this, [this](TSubclassOf<UCommonActivatableWidget> ModalClass)
	{
		if (Cast<UServerQueueIdleCheckModal>(PopupMenuStack->GetActiveWidget()) == nullptr)
		{
			UCommonActivatableWidget* Modal = PopupMenuStack->AddWidget(ModalClass);
			if (UServerQueueIdleCheckModal* IdleCheckModal = Cast<UServerQueueIdleCheckModal>(Modal))
			{
				FSimpleDelegate OnClose;
				OnClose.BindWeakLambda(this, [this, IdleCheckModal]
				{
					PopupMenuStack->RemoveWidget(*IdleCheckModal);
				});
				IdleCheckModal->Configure(OnClose);
			}
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

	UMenuManagerSubsystem::Get().OnClearPopupStack.BindWeakLambda(this, [this]
	{
		PopupMenuStack->ClearWidgets();
	});

	UServerQueueService::Get(GetWorld())->OnUserMustAcceptServerConsents.AddWeakLambda(this, [this](UServerDataObject* Server, FText ErrorMsg)
	{
		UMenuManagerSubsystem::Get().ShowServerDetails(Server);
	});

	OnRequestShowSearchResults = USearchStore::Get().AddRequestShowSearchResultsListener(
		FSimpleMulticastDelegate::FDelegate::CreateWeakLambda(this, [this]
	{
		SearchResultsPage->SetVisibility(ESlateVisibility::Visible);
		ServerListsScrollBox->SetVisibility(ESlateVisibility::Collapsed);
	}));

	OnRequestClearSearch = USearchStore::Get().AddRequestClearSearchListener(FSimpleMulticastDelegate::FDelegate::CreateWeakLambda(this, [this]
	{
		SearchResultsPage->SetVisibility(ESlateVisibility::Collapsed);
		ServerListsScrollBox->SetVisibility(ESlateVisibility::Visible);
	}));

	ON_INPUT_METHOD_CHANGED(UpdateActionBindings)
}

void UNodecraftStudioMainMenu::NativeDestruct()
{
	Super::NativeDestruct();
	USearchStore& SearchStore = USearchStore::Get();
	SearchStore.RemoveRequestShowSearchResultsListener(OnRequestShowSearchResults);
	SearchStore.RemoveRequestClearSearchListener(OnRequestClearSearch);
}

void UNodecraftStudioMainMenu::SetActiveServerList(UPaginatedServerList* ActiveList)
{
	FavoriteServers->DeactivateWidget();
	PrivateServers->DeactivateWidget();
	RecommendedServers->DeactivateWidget();
	PopularServers->DeactivateWidget();
	if (ActiveList)
	{
		ActiveList->ActivateWidget();
	}
}

void UNodecraftStudioMainMenu::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	// When we move down from the back button, we need to explicitly move to the create server button
	FCustomWidgetNavigationDelegate BackButtonDownNavDelegate = FCustomWidgetNavigationDelegate();
	BackButtonDownNavDelegate.BindUFunction(this, "MoveToCreateServerButton");
	MainMenuHeader->GetBackButton()->SetNavigationRuleCustom(EUINavigation::Down, BackButtonDownNavDelegate);

	FavoriteServers->OnServerCardFocused.BindWeakLambda(this, [this]()
	{
		SetActiveServerList(FavoriteServers);
	});

	PrivateServers->OnServerCardFocused.BindWeakLambda(this, [this]()
	{
		SetActiveServerList(PrivateServers);
	});

	RecommendedServers->OnServerCardFocused.BindWeakLambda(this, [this]()
	{
		SetActiveServerList(RecommendedServers);
	});

	PopularServers->OnServerCardFocused.BindWeakLambda(this, [this]()
	{
		SetActiveServerList(PopularServers);
	});

	// For all of our lists, we need to handle navigation between them
	PrivateServers->ListNavDelegate.BindWeakLambda(this, [this](const FGeometry& MyGeometry, const FNavigationEvent& InNavigationEvent, const FNavigationReply& InDefaultReply)
	{
		if (InNavigationEvent.GetNavigationType() == EUINavigation::Up)
		{
			// If we're navving up from the private servers list, we want to deactivate it before moving
			// up to the back button
			SetActiveServerList(nullptr);
			return FNavigationReply::Explicit(MainMenuHeader->GetBackButton()->TakeWidget());
		}
		else if (InNavigationEvent.GetNavigationType() == EUINavigation::Down)
		{
			// We may not have any favorite servers, so we need to check if we do and otherwise defer to popular servers
			if (UUserWidget* FirstFavorite = FavoriteServers->GetFirstWidgetInList())
			{
				return FNavigationReply::Explicit(FirstFavorite->TakeWidget());
			}
			else if (UUserWidget* FirstPopular = PopularServers->GetFirstWidgetInList())
			{
				return FNavigationReply::Explicit(FirstPopular->TakeWidget());
			}
		}
		return InDefaultReply;
	});

	FavoriteServers->ListNavDelegate.BindWeakLambda(this, [this](const FGeometry& MyGeometry, const FNavigationEvent& InNavigationEvent, const FNavigationReply& InDefaultReply)
	{
		if (InNavigationEvent.GetNavigationType() == EUINavigation::Up)
		{
			if (UUserWidget* FirstPrivate = PrivateServers->GetFirstWidgetInList())
			{
				return FNavigationReply::Explicit(FirstPrivate->TakeWidget());
			}
		}
		else if (InNavigationEvent.GetNavigationType() == EUINavigation::Down)
		{
			if (UUserWidget* FirstPopular = PopularServers->GetFirstWidgetInList())
			{
				return FNavigationReply::Explicit(FirstPopular->TakeWidget());
			}
		}
		return InDefaultReply;
	});

	PopularServers->ListNavDelegate.BindWeakLambda(this, [this](const FGeometry& MyGeometry, const FNavigationEvent& InNavigationEvent, const FNavigationReply& InDefaultReply)
	{
		if (InNavigationEvent.GetNavigationType() == EUINavigation::Up)
		{
			// We may not have any favorite servers, so we need to check if we do and otherwise defer to private servers
			if (UUserWidget* FirstFavorite = FavoriteServers->GetFirstWidgetInList())
			{
				return FNavigationReply::Explicit(FirstFavorite->TakeWidget());
			}
			else if (UUserWidget* FirstPrivate = PrivateServers->GetFirstWidgetInList())
			{
				return FNavigationReply::Explicit(FirstPrivate->TakeWidget());
			}
		}
		else if (InNavigationEvent.GetNavigationType() == EUINavigation::Down)
		{
			if (UUserWidget* FirstRecommended = RecommendedServers->GetFirstWidgetInList())
			{
				return FNavigationReply::Explicit(FirstRecommended->TakeWidget());
			}
		}
		return InDefaultReply;
	});

	RecommendedServers->ListNavDelegate.BindWeakLambda(this, [this](const FGeometry& MyGeometry, const FNavigationEvent& InNavigationEvent, const FNavigationReply& InDefaultReply)
	{
		if (InNavigationEvent.GetNavigationType() == EUINavigation::Up)
		{
			if (UUserWidget* FirstPopular = PopularServers->GetFirstWidgetInList())
			{
				return FNavigationReply::Explicit(FirstPopular->TakeWidget());
			}
		}
		else if (InNavigationEvent.GetNavigationType() == EUINavigation::Down)
		{
			return FNavigationReply::Stop();
		}
		return InDefaultReply;
	});
}

UWidget* UNodecraftStudioMainMenu::NativeGetDesiredFocusTarget() const
{
	return PrivateServers->GetFirstWidgetInList() ? PrivateServers->GetFirstWidgetInList() : MainMenuHeader->GetBackButton();
}

UWidget* UNodecraftStudioMainMenu::MoveToCreateServerButton(EUINavigation Direction)
{
	return PrivateServers->GetFirstWidgetInList() ? PrivateServers->GetFirstWidgetInList() : MainMenuHeader->GetBackButton();
}

void UNodecraftStudioMainMenu::UpdateActionBindings(ECommonInputType CurrentInputType)
{
	ActionBar->SetVisibility(CurrentInputType == ECommonInputType::MouseAndKeyboard
		? ESlateVisibility::Collapsed : ESlateVisibility::SelfHitTestInvisible);
}
