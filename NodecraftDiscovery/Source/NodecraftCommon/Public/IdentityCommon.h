// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Models/Consents.h"
#include "UObject/Object.h"
#include "IdentityCommon.generated.h"

UENUM()
enum class ESendIdentTokenResponseStatus : uint8
{
	Success,
	RequiresConsents,
	Error
};

DECLARE_DELEGATE_OneParam(FSendIdentTokenResponseDelegate, const struct FSendIdentTokenResponseDelegateData& /*Data*/);

DECLARE_DELEGATE_OneParam(FAcceptConsentsDelegate, const FGameConsents& /*Consents*/);

DECLARE_DELEGATE_TwoParams(FOnAuthRequiresConsentsDelegate, FAcceptConsentsDelegate /*AcceptConsentsDelegate*/, const FString& /*IdentType*/);

USTRUCT()
struct FSendIdentTokenResponseDelegateData
{
	GENERATED_BODY()

	ESendIdentTokenResponseStatus Status;
	FText Error;
	// Will be set if the response requires additional consents
	FAcceptConsentsDelegate AcceptConsentsDelegate;
};

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnConsentsRequired, FSendIdentTokenResponseDelegateData /*Data*/, const FString& /*IdentType*/);