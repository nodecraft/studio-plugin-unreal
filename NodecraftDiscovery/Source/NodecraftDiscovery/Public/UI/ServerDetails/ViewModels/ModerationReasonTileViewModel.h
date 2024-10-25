// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Models/ModerationReasonDataObject.h"
#include "UObject/Object.h"
#include "ModerationReasonTileViewModel.generated.h"

/**
 * 
 */
UCLASS()
class NODECRAFTDISCOVERY_API UModerationReasonTileViewModel : public UObject
{
	GENERATED_BODY()

public:
	UModerationReasonDataObject* GetReason() const;
	void SetReason(UModerationReasonDataObject* const InReason);
	int32 GetDisplayIndex() const;
	void SetDisplayIndex(const int32 InDisplayIndex);

private:
	UPROPERTY()
	UModerationReasonDataObject* Reason;

	int32 DisplayIndex;
	bool bIsBottomRow;

public:
	bool IsBottomRow() const;
	void SetIsBottomRow(const bool InBottomRow);
};
