// Nodecraft, Inc. © 2012-2024, All Rights Reserved.


#include "Subsystems/NodecraftUIManagerSubsystem.h"

#include "NodecraftLogCategories.h"
#include "Api/NodecraftStudioSessionManagerSubsystem.h"
#include "DeveloperSettings/NodecraftStudioWidgetSettings.h"

void UNodecraftUIManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	ShowNodecraftUI.AddUniqueDynamic(this, &UNodecraftUIManagerSubsystem::OnNodecraftUIShown_Internal);
	
	UIdentService::Get().OnPlayerLoggedIn.BindWeakLambda(this, [this]()
	{
		UE_LOG(LogNodecraftAuth, Log, TEXT("Player logged in."))
		if (bIsShowingUI)
		{
			CloseNodecraftUI(false);
			OpenNodecraftUI(false);
		}
	});

	UIdentService::Get().OnPlayerLoggedOut.BindWeakLambda(this, [this]()
	{
		UE_LOG(LogNodecraftAuth, Log, TEXT("Player logged out."))
		if (bIsShowingUI)
		{
			UE_LOG(LogNodecraftUI, Log, TEXT("Closing Nodecraft UI."))
			HideNodecraftUI.Broadcast(false);
			OpenNodecraftUI(false);
		}
	});
}

void UNodecraftUIManagerSubsystem::Deinitialize()
{
	Super::Deinitialize();
	ShowNodecraftUI.RemoveAll(this);
}

void UNodecraftUIManagerSubsystem::OpenNodecraftUI(bool bAnimate = true)
{
	const bool bIsPlayerLoggedIn = UNodecraftStudioSessionManager::Get().IsPlayerSessionValid();
	if (bIsPlayerLoggedIn)
	{
		if (MainMenu == nullptr)
		{
			MainMenu = CreateWidget<UUserWidget>(GetWorld(), UNodecraftStudioWidgetSettings::Get().GetMainMenuWidget().LoadSynchronous());
		}
		UE_LOG(LogNodecraftUI, Log, TEXT("Opening main Nodecraft menu."))
		ShowNodecraftUI.Broadcast(MainMenu, bAnimate);
	}
	else
	{
		if (LoginScreen == nullptr)
		{
			LoginScreen = CreateWidget<UUserWidget>(GetWorld(), UNodecraftStudioWidgetSettings::Get().GetLoginScreenWidget().LoadSynchronous());
		}
		UE_LOG(LogNodecraftUI, Log, TEXT("Opening Nodecraft authentication UI."))

		ShowNodecraftUI.Broadcast(LoginScreen, bAnimate);
	}
}

void UNodecraftUIManagerSubsystem::CloseNodecraftUI(bool bAnimate)
{
	UE_LOG(LogNodecraftUI, Log, TEXT("Closing Nodecraft UI."))
	HideNodecraftUI.Broadcast(bAnimate);
}

void UNodecraftUIManagerSubsystem::OnNodecraftUIHidden_Internal(UUserWidget* Widget, bool bAnimate)
{
	bIsShowingUI = false;
}

void UNodecraftUIManagerSubsystem::OnNodecraftUIShown_Internal(UUserWidget* Widget, bool bAnimate)
{
	bIsShowingUI = true;
}
