// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "ModerationActionButton.h"
#include "Common/NodecraftDelegates.h"
#include "Models/PlayerServerDetails.h"
#include "UI/Common/NodecraftLoadGuard.h"
#include "ViewModels/ModerationConsolePlayerDetailsPanelViewModel.h"
#include "ModerationActionsSection.generated.h"

DECLARE_DELEGATE_OneParam(FOnActionSelected, EModerationAction);

UCLASS()
class NODECRAFTDISCOVERY_API UModerationActionsSection : public UCommonUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Details", meta = (BindWidget))
	UModerationActionButton* KickButton;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Details", meta = (BindWidget))
	UModerationActionButton* BanButton;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Details", meta = (BindWidget))
	UModerationActionButton* UnbanButton;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Details", meta = (BindWidget))
	UModerationActionButton* PromoteToModButton;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Details", meta = (BindWidget))
	UModerationActionButton* DemoteModButton;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Details", meta = (BindWidget))
	UNodecraftLoadGuard* LoadGuard;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Nodecraft UI|Input", meta = (RowType = "/Script/CommonUI.CommonInputActionDataBase"))
	FDataTableRowHandle SelectionModActionData;

	FUIActionBindingHandle SelectActionBindingHandle;

	UFUNCTION(BlueprintCallable, Category = "Nodecraft UI|Server Details")
	void ScrollToTop();

public:
	void ConfigureForPlayerServerDetails(TArray<UPlayerServerDetailsDataObject*> PlayerServerDetails, EPlayerRole PlayerRole);
	
	void SetIsLoading(bool bIsLoading);
	void RefreshActions(EModerationAction Action);
	UWidget* GetInitialFocusTarget();

	FOnActionSelected OnActionSelected;

	virtual void NativeOnInitialized() override;

	FOnWidgetReceivedFocus OnWidgetReceivedFocus;

	FGetFocusDestination NavToPlayerListDelegate;

	FSimpleDelegate OnScrollToTopDelegate;

private:
	UFUNCTION()
	UWidget* HandleMoveLeftFromBanButtons(EUINavigation Direction);
	
	UFUNCTION()
	UWidget* HandleMoveLeftFromNonBanButtons(EUINavigation Direction);

	void SetupButtonNavigation();
};
