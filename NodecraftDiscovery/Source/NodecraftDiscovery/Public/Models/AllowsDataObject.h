// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "AllowsDataObject.generated.h"

class UPlayerDataObject;
class UServerDataObject;
enum class EAllowStatus : uint8;
/**
 * 
 */
UCLASS()
class NODECRAFTDISCOVERY_API UAllowsDataObject : public UObject
{
	GENERATED_BODY()

public:
	static UAllowsDataObject* FromJson(const TSharedRef<FJsonObject>& Json);
	FString GetID() const;
	EAllowStatus GetStatus() const;
	FDateTime GetDateExpires() const;
	UPlayerDataObject* GetSender() const;
	UServerDataObject* GetServer() const;

	void SetStatus(const EAllowStatus Status);

protected:
	FString ID;
	EAllowStatus AllowStatus;
	FDateTime DateExpires;

	UPROPERTY()
	UServerDataObject* Server;

	UPROPERTY()
	UPlayerDataObject* Sender;
};
