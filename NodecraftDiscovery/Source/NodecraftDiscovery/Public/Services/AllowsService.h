// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ServiceDelegates.h"
#include "UObject/Object.h"
#include "AllowsService.generated.h"

class UAllowsDataObject;
DECLARE_DELEGATE_ThreeParams(FListPlayerAllowsResponseDelegate, TArray<UAllowsDataObject*> /*Allows*/, bool /*bSuccess*/, TOptional<FText> /*Error*/);
/**
 * 
 */
UCLASS()
class NODECRAFTDISCOVERY_API UAllowsService : public UEngineSubsystem
{
	GENERATED_BODY()

public:
	static UAllowsService& Get() { return *GEngine->GetEngineSubsystem<UAllowsService>(); }

	bool AcceptAllow(const FString& AllowId, FSimpleServiceResponseDelegate OnComplete);
	bool DeclineAllow(const FString& AllowId, FSimpleServiceResponseDelegate OnComplete);
	bool ListPlayerAllows(FListPlayerAllowsResponseDelegate OnComplete);
};
