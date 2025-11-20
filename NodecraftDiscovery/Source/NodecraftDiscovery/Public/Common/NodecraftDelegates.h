// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once
#include "CoreMinimal.h"
#include "Components/Widget.h"

DECLARE_DELEGATE_RetVal_ThreeParams(FNavigationReply, FOnNavigationDelegate, const FGeometry& /*MyGeometry*/, const FNavigationEvent& /*InNavigationEvent*/, const FNavigationReply& /*InDefaultReply*/);
DECLARE_DELEGATE_RetVal(UWidget*, FGetFocusDestination)
DECLARE_DELEGATE_OneParam(FOnWidgetReceivedFocus, UWidget*);
DECLARE_DELEGATE_RetVal(bool, FSimpleBooleanDelegate);
