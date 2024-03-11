// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/IHttpRequest.h"
#include "UObject/Object.h"
#include "NodecraftUtility.generated.h"

/**
 * 
 */
UCLASS()
class NODECRAFTDISCOVERY_API UNodecraftUtility : public UObject
{
	GENERATED_BODY()

public:
	static FText ParseError(FHttpResponsePtr Response, FString FunctionName);

	static FText TimeLeft(FDateTime Expires);
};
