// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "ModerationActionsSection.h"
#include "ModerationReasonsSection.h"
#include "PlayerPlatformIcon.h"
#include "SelectedPlayerDetails.h"
#include "Components/ScrollBox.h"
#include "Models/PlayerServerDetails.h"
#include "ViewModels/ModerationConsolePlayerDetailsPanelViewModel.h"
#include "ModerationConsolePlayerDetailsPanel.generated.h"

/**
 * 
 */
UCLASS()
class NODECRAFTDISCOVERY_API UModerationConsolePlayerDetailsPanel : public UCommonActivatableWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Details", meta = (BindWidget))
	UPanelWidget* NoPlayerSelectedContent;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Details", meta = (BindWidget))
	UPanelWidget* PlayerSelectedContent;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Details", meta = (BindWidget))
	USelectedPlayerDetails* PlayerDetails;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Details", meta = (BindWidget))
	UScrollBox* ScrollBox;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Details", meta = (BindWidget))
	UModerationActionsSection* ModerationActionsSection;
	
	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Details", meta = (BindWidget))
	UModerationReasonsSection* ModerationReasonsSection;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Details", meta = (BindWidget))
	UCommonTextBlock* UsernameLabel;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Details", meta = (BindWidget))
	UPlayerPlatformIcon* PlatformIcon;

	UPROPERTY()
	UModerationConsolePlayerDetailsPanelViewModel* ViewModel;

	void RefreshUI();

	FString ServerId;

private:
	void PerformKickAction();
	void PerformBanAction();
	void PerformUnbanAction();
	void PerformUpdateModStatusAction(EModerationAction Action);

public:
	void SetSelectedPlayers(const TArray<UPlayerServerDetailsDataObject*> SelectedPlayers);

	void SetServerId(const FString& InServerId);

	virtual void NativeOnInitialized() override;
	void SetPlayerRole(EPlayerRole Role);

	FSimpleMulticastDelegate OnCompletedModerationActionDelegate;

	FGetFocusDestination GetPlayerListDelegate;

	virtual UWidget* NativeGetDesiredFocusTarget() const override;
};
