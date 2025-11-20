// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "CommonTextBlock.h"
#include "CommonTileView.h"
#include "ServerCard.h"
#include "ServerTileView.h"
#include "API/ServerListType.h"

#include "PaginatedServerList.generated.h"

class UServerListViewModel;
class UServerDataObject;
class UPaginatedServerListNavButton;

/**
 * 
 */
UCLASS()
class NODECRAFTDISCOVERY_API UPaginatedServerList : public UCommonActivatableWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	UServerListViewModel* ViewModel;
	
	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Lists", meta=(BindWidget))
	UCommonTextBlock* ServerListTitleTextBlock;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Lists", meta=(BindWidget))
	UPaginatedServerListNavButton* NextButton;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Lists", meta=(BindWidget))
	UPaginatedServerListNavButton* PrevButton;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Lists", meta=(BindWidget))
	UServerCard* NextPreviewCard;
	
	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Lists", meta=(BindWidget))
	UServerCard* PreviousPreviewCard;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Lists", meta=(BindWidget))
	UServerTileView* ServerListView;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Lists", meta=(BindWidget))
	UCommonTileView* LoadingServersListView;

	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "Nodecraft UI|Server Lists")
	EServerListType ServerListType;

	UPROPERTY(EditDefaultsOnly, Category = "Nodecraft UI|Server Lists")
	TSubclassOf<UUserWidget> NoServerWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "Nodecraft UI|Server Lists")
	TSubclassOf<UUserWidget> CreateServerButtonWidgetClass;

	virtual FReply NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent) override;
	void ForwardNavigationDelegate(UWidget& EntryWidget);

public:
	virtual void NativeConstruct() override;

	UUserWidget* GetFirstWidgetInList();
	
	void MoveFocusToLastWidgetInList();

	// We delegate to this when a server card delegates to us for navigation.
	// ServerCard -> PaginatedServerList -> MainMenu
	FOnNavigationDelegate ListNavDelegate;

	// When a server card is focused, we want to notify the main menu so it can handle de/activating
	// the appropriate lists, which results in action routing to the correct list and its prev/next buttons.
	FSimpleDelegate OnServerCardFocused;
};
