// Nodecraft, Inc. © 2012-2024, All Rights Reserved.


#include "UI/ServerDetails/ViewModels/ModerationReasonTileViewModel.h"

UModerationReasonDataObject* UModerationReasonTileViewModel::GetReason() const
{
	return Reason;
}

void UModerationReasonTileViewModel::SetReason(UModerationReasonDataObject* const InReason)
{
	this->Reason = InReason;
}

int32 UModerationReasonTileViewModel::GetDisplayIndex() const
{
	return DisplayIndex;
}

void UModerationReasonTileViewModel::SetDisplayIndex(const int32 InDisplayIndex)
{
	this->DisplayIndex = InDisplayIndex;
}

bool UModerationReasonTileViewModel::IsBottomRow() const
{
	return bIsBottomRow;
}

void UModerationReasonTileViewModel::SetIsBottomRow(const bool InBottomRow)
{
	this->bIsBottomRow = InBottomRow;
}
