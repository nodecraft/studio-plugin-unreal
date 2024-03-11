// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BanDataObject.h"
#include "PlayerDataObject.h"
#include "UObject/Object.h"
#include "PlayerServerDetails.generated.h"

class UPlayerServerDetailsDataObject;

DECLARE_DELEGATE_TwoParams(FOnPlayerSelectionChanged, UPlayerServerDetailsDataObject* /*Player*/, bool /*Selected*/);

// Called when we tap the little arrow on the right hand side, which will select the player and deselect all others
DECLARE_DELEGATE_OneParam(FOnPlayerSelectedExclusive, UPlayerServerDetailsDataObject* /*Player*/);

UCLASS()
class NODECRAFTDISCOVERY_API UPlayerServerDetailsDataObject : public UObject
{
	GENERATED_BODY()

public:
	UPlayerDataObject* GetPlayer() const;
	UBanDataObject* GetBan() const;

	static UPlayerServerDetailsDataObject* FromJson(const TSharedRef<FJsonObject> Json);


private:
	UPROPERTY()
	UPlayerDataObject* Player;
	
	UPROPERTY()
	UBanDataObject* Ban;
};

UCLASS()
class NODECRAFTDISCOVERY_API UPlayerServerDetailsViewModel : public UObject
{
	GENERATED_BODY()

public:
	UPlayerDataObject* GetPlayer() const;
	UBanDataObject* GetBan() const;
	UPlayerServerDetailsDataObject* GetPlayerServerDetails() const;

	// Will be set manually before sending this to our list view
	FOnPlayerSelectionChanged OnPlayerSelectionChanged;

	// Will be set manually before sending this to our list view
	FOnPlayerSelectedExclusive OnPlayerSelectedExclusive;

	bool bIsSelectable;

	static UPlayerServerDetailsViewModel* FromDataObject(UPlayerServerDetailsDataObject* DataObject);


private:
	UPROPERTY()
	UPlayerServerDetailsDataObject* PlayerServerDetailsDataObject;
};

