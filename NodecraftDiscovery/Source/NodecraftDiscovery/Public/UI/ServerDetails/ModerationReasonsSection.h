// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "ModerationReasonTileView.h"
#include "Common/NodecraftDelegates.h"
#include "Components/ComboBoxString.h"
#include "Components/MultiLineEditableText.h"
#include "Models/BanDuration.h"
#include "UI/Common/NodecraftLoadGuard.h"
#include "ViewModels/ModerationConsolePlayerDetailsPanelViewModel.h"
#include "ModerationReasonsSection.generated.h"

class UNodecraftButtonBase;
DECLARE_DELEGATE_OneParam(FOnReasonSelectedDelegate, UModerationReasonDataObject* /*Reason*/);
DECLARE_DELEGATE_OneParam(FOnBanDurationChangedDelegate, TOptional<FTimespan> /*BanDuration*/);
DECLARE_DELEGATE_OneParam(FOnUserEditedPrivateNotesDelegate, const FText& /*Notes*/);

UCLASS()
class NODECRAFTDISCOVERY_API UModerationReasonsSection : public UCommonUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	
	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Details", meta = (BindWidget))
	UMultiLineEditableText* PrivateNotesBlock;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Details", meta = (BindWidget))
	UModerationReasonTileView* ReasonButtonsTileView;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Details", meta = (BindWidget))
	UPanelWidget* BanInfoContainer;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Details", meta = (BindWidget))
	UComboBoxString* BanDurationComboBox;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Details", meta = (BindWidget))
	UNodecraftLoadGuard* LoadGuard;	

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Details", meta = (BindWidget))
	UNodecraftButtonBase* CancelButton;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Details", meta = (BindWidget))
	UNodecraftButtonBase* ConfirmActionButton;

	// These are used to populate the combo box
	UPROPERTY(EditDefaultsOnly, Category = "Nodecraft UI|Server Details|Moderation Console")
	TArray<FBanDuration> BanDurations;

	UPROPERTY(EditDefaultsOnly, Category = "Nodecraft UI|Server Details|Moderation Console")
	int32 DefaultBanDurationIndex;
	
	EModerationAction SelectedReason;
	
	UFUNCTION()
	void OnBanDurationChanged(FString SelectedItem, ESelectInfo::Type SelectionType);

	UFUNCTION()
	void OnPrivateNotesChanged(const FText& Text);

	UFUNCTION()
	UWidget* HandleTileNavDownFromLastRow(EUINavigation Direction);

	FCustomWidgetNavigationDelegate OnTileNavigation;

public:
	FSimpleDelegate OnScrollToTopDelegate;
	FSimpleDelegate OnConfirmActionDelegate;
	FSimpleDelegate OnCancelDelegate;
	FGetFocusDestination NavToPlayerListDelegate;
	FOnReasonSelectedDelegate OnReasonSelectedDelegate;
	FOnBanDurationChangedDelegate OnBanDurationChangedDelegate;
	FOnUserEditedPrivateNotesDelegate OnUserEditedPrivateNotesDelegate;
	
	void SetSelectedAction(EModerationAction InSelectedReason);

	UFUNCTION()
	UWidget* HandleTileNavUpFromFirstRow(EUINavigation Direction);

	UFUNCTION()
	UWidget* HandleTileNavLeftFromLeftColumn(EUINavigation Direction);

	void SetConfirmButtonEnabled(bool bActive);
	void ClearPrivateNotes();
	void SetIsLoading(bool bIsLoading);

	UWidget* GetFirstReasonWidget();

	FOnWidgetReceivedFocus OnWidgetReceivedFocus;

private:
	void GetSelectedBanDuration(FBanDuration& OutBanDuration);

};
