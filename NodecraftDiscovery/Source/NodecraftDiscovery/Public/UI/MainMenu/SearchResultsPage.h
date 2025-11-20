// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "SearchResultsPage.generated.h"

class UNodecraftLoadingButton;
class UHorizontalBox;
class UVerticalBox;
class UNodecraftButtonBase;
class UCommonTileView;
class UCommonTextBlock;
class UNodecraftLoadGuard;
/**
 * 
 */
UCLASS()
class NODECRAFTDISCOVERY_API USearchResultsPage : public UCommonActivatableWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	
	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Search Results", meta=(BindWidget))
	UNodecraftLoadGuard* SearchResultsLoadGuard;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Search Results", meta=(BindWidget))
	UCommonTextBlock* SearchQueryText;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Search Results", meta=(BindWidget))
	UCommonTileView* SearchResults;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Search Results", meta=(BindWidget))
	UNodecraftLoadingButton* PreviousPageButton;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Search Results", meta=(BindWidget))
	UNodecraftLoadingButton* NextPageButton;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Search Results", meta=(BindWidget))
	UNodecraftButtonBase* ClearSearchButton;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Search Results", meta=(BindWidget))
	UVerticalBox* NoSearchResultsContainer;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Search Results", meta=(BindWidget))
	UHorizontalBox* PaginationButtonsContainer;

private:
	uint32 CurrentPage = 1;
	FDelegateHandle OnSearchQueryUpdated;
	FDelegateHandle OnSearchResultsUpdated;
	FDelegateHandle OnSearchStarted;
};
