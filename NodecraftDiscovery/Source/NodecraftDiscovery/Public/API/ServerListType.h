// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ServerListType.generated.h"

/**
 * 
 */
UENUM()
enum class EServerListType : uint8
{
	Popular,
	Favorite,
	Recommended,
	Owned
};
