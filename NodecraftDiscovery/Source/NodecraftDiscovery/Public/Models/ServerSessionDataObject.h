// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "ServerSessionDataObject.generated.h"

/**
 * 
 */
UCLASS()
class NODECRAFTDISCOVERY_API UServerSessionDataObject : public UObject
{
	GENERATED_BODY()

public:
	static UServerSessionDataObject* FromJson(const TSharedPtr<FJsonObject>& Json);

	UFUNCTION(BlueprintCallable)
	FString GetToken() const;

	UFUNCTION(BlueprintCallable)
	FDateTime GetDateExpires() const;

	UFUNCTION(BlueprintCallable)
	FString GetPlayerAccessTokenID() const;

	UFUNCTION(BlueprintCallable)
	FString GetServerConnection() const;

protected:
	FString Token;
	FDateTime DateExpires;
	FString ServerConnection;
	FString PlayerAccessTokenID;
};
