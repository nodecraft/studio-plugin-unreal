﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "PlayerListType.generated.h"

/**
 * 
 */
UENUM()
enum class EPlayerListType : uint8
{
	Online,
	Recent
};
