// Nodecraft, Inc. © 2012-2024, All Rights Reserved.


#include "Subsystems/NodecraftUIManagerSubsystem.h"

#include "NodecraftLogCategories.h"
#include "API/NodecraftStudioSessionManagerSubsystem.h"
#include "DeveloperSettings/NodecraftStudioWidgetSettings.h"
#include "UI/NodecraftMenuNavInputProcessor.h"
#include "Framework/Application/SlateApplication.h"

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

#if !UE_SERVER 
	if (ensure(FSlateApplication::IsInitialized()))
	{
		MenuNavInputProcessor = FNodecraftMenuNavInputProcessor::CreateMenuNavInputProcessor();
		FSlateApplication::Get().RegisterInputPreProcessor(MenuNavInputProcessor, 0);	
	}
#endif
	
}

int32 UNodecraftUIManagerSubsystem::GetNumTilesInServerList()
{
	return 4;
	// TODO: In future we will implement this logic to determine the number of tiles to show based on the viewport size. It's not quite ready
	// if (ViewportSize.X > 1920)
	// {
	// 	return 6;
	// }
	// else if (ViewportSize.X > 1280)
	// {
	// 	return 4;
	// }
	// else if (ViewportSize.X > 640)
	// {
	// 	return 3;
	// }
	// else
	// {
	// 	return 2;
	// }
}

void UNodecraftUIManagerSubsystem::Deinitialize()
{
	ShowNodecraftUI.RemoveAll(this);
	FViewport::ViewportResizedEvent.RemoveAll(this);
	Super::Deinitialize();

}

void UNodecraftUIManagerSubsystem::Tick(float DeltaTime)
{
	if (MenuNavInputProcessor)
	{
		float PrimaryVal = MenuNavInputProcessor->GetAnalogValues(EAnalogStick::Left).Y;
		float SecondaryVal = MenuNavInputProcessor->GetAnalogValues(EAnalogStick::Right).Y;
		OnRequestScrollPrimaryMenu.Broadcast(PrimaryVal);
		OnRequestScrollSecondaryMenu.Broadcast(SecondaryVal);
	}
}

TStatId UNodecraftUIManagerSubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UNodecraftUIManagerSubsystem, STATGROUP_Tickables);
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
