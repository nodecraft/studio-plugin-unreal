// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Models/ModerationReasonDataObject.h"
#include "Models/PlayerServerDetails.h"
#include "UI/ServerDetails/ModerationActionButton.h"
#include "UObject/Object.h"
#include "ModerationConsolePlayerDetailsPanelViewModel.generated.h"

/**
 * 
 */
UCLASS()
class NODECRAFTDISCOVERY_API UModerationConsolePlayerDetailsPanelViewModel : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TArray<UPlayerServerDetailsDataObject*> SelectedPlayers = TArray<UPlayerServerDetailsDataObject*>();

	UPROPERTY()
	EModerationAction SelectedAction = EModerationAction::Undefined;

	UPROPERTY()
	UModerationReasonDataObject* SelectedReason = nullptr;

	UPROPERTY()
	FText PrivateNotes;

	UPROPERTY()
	bool bIsLoading;

	TOptional<FTimespan> BanDuration;
};
