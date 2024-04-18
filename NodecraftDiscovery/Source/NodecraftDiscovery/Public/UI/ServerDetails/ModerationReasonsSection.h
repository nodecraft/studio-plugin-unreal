// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "Components/ComboBox.h"
#include "Components/ComboBoxString.h"
#include "Components/MultiLineEditableText.h"
#include "Components/TileView.h"
#include "Models/BanDuration.h"
#include "UI/Common/NodecraftLoadGuard.h"
#include "ViewModels/ModerationConsolePlayerDetailsPanelViewModel.h"
#include "ModerationReasonsSection.generated.h"

DECLARE_DELEGATE_OneParam(FOnReasonSelectedDelegate, UModerationReasonDataObject* /*Reason*/);
DECLARE_DELEGATE_OneParam(FOnBanDurationChangedDelegate, TOptional<FTimespan> /*BanDuration*/);
DECLARE_DELEGATE_OneParam(FOnUserEditedPrivateNotesDelegate, const FText& /*Notes*/);


UCLASS()
class NODECRAFTDISCOVERY_API UModerationReasonsSection : public UCommonUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UMultiLineEditableText* PrivateNotesBlock;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTileView* ReasonButtonsTileView;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UPanelWidget* BanInfoContainer;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UComboBoxString* BanDurationComboBox;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UNodecraftLoadGuard* LoadGuard;	

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UNodecraftButtonBase* CancelButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UNodecraftButtonBase* ConfirmActionButton;

	// These are used to populate the combo box
	UPROPERTY(EditDefaultsOnly)
	TArray<FBanDuration> BanDurations;

	UPROPERTY(EditDefaultsOnly)
	int32 DefaultBanDurationIndex;

	UFUNCTION()
	void OnBanDurationChanged(FString SelectedItem, ESelectInfo::Type SelectionType);

	UFUNCTION()
	void OnPrivateNotesChanged(const FText& Text);

public:
	FSimpleDelegate OnConfirmActionDelegate;
	FSimpleDelegate OnCancelDelegate;
	FOnReasonSelectedDelegate OnReasonSelectedDelegate;
	FOnBanDurationChangedDelegate OnBanDurationChangedDelegate;
	FOnUserEditedPrivateNotesDelegate OnUserEditedPrivateNotesDelegate;
	
	void SetSelectedAction(EModerationAction SelectedReason);
	
	virtual void NativeOnInitialized() override;

	void SetConfirmButtonEnabled(bool bActive);
	void ClearPrivateNotes();
	void SetIsLoading(bool bIsLoading);

private:
	void GetSelectedBanDuration(FBanDuration& OutBanDuration);

};
