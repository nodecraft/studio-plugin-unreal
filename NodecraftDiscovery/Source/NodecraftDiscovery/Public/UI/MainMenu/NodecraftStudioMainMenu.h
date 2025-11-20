// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "Components/Border.h"
#include "Stores/SearchStore.h"
#include "UI/ServerLists/PaginatedServerList.h"
#include "UI/ServerLists/ServerList.h"
#include "Widgets/CommonActivatableWidgetContainer.h"
#include "NodecraftStudioMainMenu.generated.h"

class UCommonBoundActionBar;
class UFloatingMainMenuHeader;
class UScrollBox;
class USearchResultsPage;
class UNodecraftLoadGuard;
class UCommonActivatableWidgetSwitcher;
class UAsyncImage;
/**
 * 
 */
UCLASS()
class NODECRAFTDISCOVERY_API UNodecraftStudioMainMenu : public UCommonActivatableWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeOnInitialized() override;
	virtual UWidget* NativeGetDesiredFocusTarget() const override;
	
	void SetActiveServerList(UPaginatedServerList* ActiveList);

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Main Menu", meta=(BindWidget))
	UFloatingMainMenuHeader* MainMenuHeader;
	
	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Main Menu", meta=(BindWidget))
	UPaginatedServerList* PrivateServers;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Main Menu", meta=(BindWidget))
	UPaginatedServerList* RecommendedServers;
	
	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Main Menu", meta=(BindWidget))
	UPaginatedServerList* FavoriteServers;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Main Menu", meta=(BindWidget))
	UPaginatedServerList* PopularServers;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Main Menu", meta=(BindWidget))
	UAsyncImage* GameBackground;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Main Menu", meta=(BindWidget))
	UScrollBox* ServerListsScrollBox;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Main Menu", meta=(BindWidget))
	USearchResultsPage* SearchResultsPage;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Main Menu", meta=(BindWidget))
	UCommonActivatableWidgetStack* MenuStack;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Main Menu", meta=(BindWidget))
	UCommonActivatableWidgetStack* PopupMenuStack;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Main Menu", meta=(BindWidget))
	UCommonBoundActionBar* ActionBar;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Nodecraft UI|Main Menu")
	TSubclassOf<UCommonActivatableWidget> ServerDetailsClass;

	UPROPERTY()
	UBorder* PopupMenuStackBackground;

	UPROPERTY()
	UBorder* MenuStackBackground;

	UFUNCTION()
	UWidget* MoveToCreateServerButton(EUINavigation Direction);

private:
	void UpdateActionBindings(ECommonInputType CurrentInputType);
	
	FDelegateHandle OnRequestShowSearchResults;
	FDelegateHandle OnRequestClearSearch;
};
