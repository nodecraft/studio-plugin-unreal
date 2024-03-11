// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "InviteStatus.generated.h"

UENUM()
enum class ENodecraftInviteStatus : uint8
{
	Pending,
	Accepted,
	Declined,
	Revoked
};
