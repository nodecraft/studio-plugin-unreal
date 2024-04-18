﻿// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "BanDuration.generated.h"

USTRUCT()
struct NODECRAFTDISCOVERY_API FBanDuration
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FText DisplayText;

	UPROPERTY(EditAnywhere)
	bool bPermanent = false;

	UPROPERTY(EditAnywhere, meta=(EditConditionHides, EditCondition="bPermanent==false"))
	int32 Days = 0;

	UPROPERTY(EditAnywhere, meta=(EditConditionHides, EditCondition="bPermanent==false"))
	int32 Hours = 0;

	UPROPERTY(EditAnywhere, meta=(EditConditionHides, EditCondition="bPermanent==false"))
	int32 Minutes = 0;
	
	TOptional<FTimespan> GetDuration() const
	{
		if (bPermanent)
		{
			return TOptional<FTimespan>();
		}
		else
		{
			return FTimespan(Days, Hours, Minutes);
		}
	}
};

