// Nodecraft, Inc. © 2012-2024, All Rights Reserved.


#include "UI/ServerDetails/ModerationReasonButton.h"

#include "Models/ModerationReasonDataObject.h"

void UModerationReasonButton::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	if (UModerationReasonDataObject* Reason = Cast<UModerationReasonDataObject>(ListItemObject))
	{
		Label->SetText(Reason->GetTitle());
		Description->SetText(Reason->GetDescription());
	}
}
