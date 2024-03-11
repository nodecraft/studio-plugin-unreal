// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "ServerQueueTokenDataObject.generated.h"

/**
 * 
 */
UCLASS()
class NODECRAFTDISCOVERY_API UServerQueueTokenDataObject : public UObject
{
	GENERATED_BODY()

public:
	static UServerQueueTokenDataObject* FromJson(const TSharedRef<FJsonObject>& Json);

	FString GetToken() const;
	FDateTime GetQueueExpires() const;
	FDateTime GetQueueRenews() const;

protected:
	FString Token;
	FDateTime QueueExpires;
	FDateTime QueueRenews;
};
