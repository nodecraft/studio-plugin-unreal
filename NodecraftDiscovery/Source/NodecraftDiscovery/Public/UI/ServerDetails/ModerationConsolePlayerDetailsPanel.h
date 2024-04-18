// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "ModerationActionsSection.h"
#include "ModerationReasonsSection.h"
#include "PlayerPlatformIcon.h"
#include "SelectedPlayerDetails.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "Models/PlayerDataObject.h"
#include "Models/PlayerServerDetails.h"
#include "UI/Common/NodecraftLoadGuard.h"
#include "ViewModels\ModerationConsolePlayerDetailsPanelViewModel.h"
#include "ModerationConsolePlayerDetailsPanel.generated.h"

/**
 * 
 */
UCLASS()
class NODECRAFTDISCOVERY_API UModerationConsolePlayerDetailsPanel : public UCommonActivatableWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UPanelWidget* NoPlayerSelectedContent;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UPanelWidget* PlayerSelectedContent;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	USelectedPlayerDetails* PlayerDetails;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UScrollBox* ScrollBox;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UModerationActionsSection* ModerationActionsSection;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UModerationReasonsSection* ModerationReasonsSection;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UCommonTextBlock* UsernameLabel;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
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

	FSimpleMulticastDelegate OnCompletedModerationActionDelegate;
};
