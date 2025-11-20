// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "FloatingMainMenuHeader.generated.h"

class USearchField;
class UNodecraftButtonBase;
class UHorizontalBox;
class UAsyncImage;

/**
 * 
 */
UCLASS()
class NODECRAFTDISCOVERY_API UFloatingMainMenuHeader : public UCommonUserWidget
{
	GENERATED_BODY()

public:
	UUserWidget* GetBackButton();

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UPROPERTY(meta=(BindWidget))
	UUserWidget* BackButton;
	
	UPROPERTY(meta=(BindWidget))
	UAsyncImage* GameLogoImage;

	UPROPERTY(meta=(BindWidget))
	UNodecraftButtonBase* SearchButton;

	UPROPERTY(meta=(BindWidget))
	USearchField* SearchField;

	UPROPERTY(meta=(BindWidget))
	UHorizontalBox* ButtonsContainer;

	UPROPERTY(EditAnywhere, Category = "Nodecraft UI|Input", meta=(RowType="/Script/CommonUI.CommonInputActionDataBase"))
	FDataTableRowHandle BackInputAction;

	UPROPERTY(EditAnywhere, Category = "Nodecraft UI|Input", meta=(RowType="/Script/CommonUI.CommonInputActionDataBase"))
	FDataTableRowHandle SearchInputAction;

private:
	void RegisterCloseUIActionBinding();
	void RegisterClearSearchActionBinding();
	
	FDelegateHandle OnRequestClearSearch;
	FUIActionBindingHandle ClearSearchActionHandle;
	FUIActionBindingHandle CloseUIActionHandle;
};
