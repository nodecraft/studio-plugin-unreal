// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "AllowStatus.generated.h"

UENUM()
enum class EAllowStatus : uint8
{
	Pending,
	Accepted,
	Declined,
	Revoked
};
