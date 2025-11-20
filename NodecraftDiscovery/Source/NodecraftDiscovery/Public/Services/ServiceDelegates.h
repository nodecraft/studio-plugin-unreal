// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
// #include "ServiceDelegates.generated.h"

DECLARE_DELEGATE_TwoParams(FSimpleServiceResponseDelegate, bool /*Succeess*/, TOptional<FText> /*Error*/);
