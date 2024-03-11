// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "Components/TextBlock.h"
#include "Models/PlayerDataObject.h"
#include "Models/PlayerServerDetails.h"
#include "UI/Common/NodecraftLoadGuard.h"
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
	UTextBlock* NumPlayersSelectedTextLabel;

public:
	void SetSelectedPlayers(const TArray<UPlayerServerDetailsDataObject*> SelectedPlayers);

	virtual void NativeOnInitialized() override;
};
