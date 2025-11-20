// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DataTypes/ModerationTypes.h"
#include "Models/ModerationReasonDataObject.h"
#include "Models/PlayerServerDetails.h"
#include "Models/ServerDataObject.h"
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

	// The role of the player viewing the panel. Some actions are only available to certain roles.
	UPROPERTY()
	EPlayerRole PlayerRole = EPlayerRole::Unknown;
};
