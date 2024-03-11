// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "NotificationListType.generated.h"

/**
 * 
 */
UENUM()
enum class ENotificationListType : uint8
{
	Active,
	Historic
};
