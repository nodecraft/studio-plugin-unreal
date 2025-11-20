// Nodecraft, Inc. © 2012-2024, All Rights Reserved.


#include "UI/MainMenu/FloatingMainMenuHeader.h"

#include "CommonInputSettings.h"
#include "Components/HorizontalBox.h"
#include "Input/CommonUIInputTypes.h"
#include "Services/GameService.h"
#include "Stores/SearchStore.h"
#include "Subsystems/NodecraftUIManagerSubsystem.h"
#include "Subsystems/RemoteImageSubsystem.h"
#include "UI/Common/AsyncImage.h"
#include "UI/Foundation/NodecraftButtonBase.h"
#include "UI/MainMenu/SearchField.h"

#define LOCTEXT_NAMESPACE "MenuHeader"

void UFloatingMainMenuHeader::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	// When we add a search listener, the lambda will be called immediately to update the UI.
	// This allows for the default state of the search field to be collapsed and sets the back action to close the UI.
	OnRequestClearSearch = USearchStore::Get().AddRequestClearSearchListener(
	FSimpleMulticastDelegate::FDelegate::CreateWeakLambda(this, [this]
	{
		ButtonsContainer->SetVisibility(ESlateVisibility::Visible);
		SearchField->SetVisibility(ESlateVisibility::Collapsed);
		SearchButton->SetFocus();

		if (ClearSearchActionHandle.IsValid())
		{
			ClearSearchActionHandle.Unregister();
		}
		// If the search field is not visible, we want the back button to close the UI,
		// and the search button to open the search field.
		RegisterCloseUIActionBinding();
		if (SearchInputAction.IsNull() == false)
		{
			SearchButton->SetTriggeringInputAction(SearchInputAction);
		}
	}));
	
	SearchButton->OnClicked().AddWeakLambda(this, [this]
	{
		if (SearchField->IsVisible())
		{
			USearchStore::Get().ClearSearch();
		}
		else
		{
			ButtonsContainer->SetVisibility(ESlateVisibility::Hidden);
			SearchField->SetVisibility(ESlateVisibility::Visible);
			SearchField->SetFocus();
			
			if (CloseUIActionHandle.IsValid())
			{
				CloseUIActionHandle.Unregister();
			}
			// If the search field is visible, we want the back button to clear the search,
			// and disable the triggering input action for the search button.
			RegisterClearSearchActionBinding();
			SearchButton->SetTriggeringInputAction(FDataTableRowHandle());
		}
	});
}

void UFloatingMainMenuHeader::NativeConstruct()
{
	Super::NativeConstruct();
	
	GameLogoImage->LoadImageAsync(UGameService::Get().GetCachedGameLogoURL(), NC_CACHED_GAME_LOGO_FILENAME);
}

void UFloatingMainMenuHeader::NativeDestruct()
{
	Super::NativeDestruct();
	USearchStore::Get().RemoveRequestClearSearchListener(OnRequestClearSearch);
}

void UFloatingMainMenuHeader::RegisterCloseUIActionBinding()
{
	if (BackInputAction.IsNull() || CloseUIActionHandle.IsValid())
	{
		return;
	}
	FBindUIActionArgs CloseUIActionArgs = FBindUIActionArgs(BackInputAction, bDisplayInActionBar, 
		FSimpleDelegate::CreateWeakLambda(this, [this]
		{
			UNodecraftUIManagerSubsystem::Get(GetGameInstance())->CloseNodecraftUI(true);
		}));
	CloseUIActionHandle = RegisterUIActionBinding(CloseUIActionArgs);
}

void UFloatingMainMenuHeader::RegisterClearSearchActionBinding()
{
	if (BackInputAction.IsNull() || ClearSearchActionHandle.IsValid())
	{
		return;
	}
	FBindUIActionArgs ClearSearchActionArgs = FBindUIActionArgs(BackInputAction, bDisplayInActionBar,
		FSimpleDelegate::CreateWeakLambda(this, [this]
		{
			USearchStore::Get().ClearSearch();
		}));
	ClearSearchActionArgs.OverrideDisplayName = LOCTEXT("ClearSearch", "Clear Search");
	ClearSearchActionHandle = RegisterUIActionBinding(ClearSearchActionArgs);
}

UUserWidget* UFloatingMainMenuHeader::GetBackButton()
{
	return BackButton;
}

#undef LOCTEXT_NAMESPACE
