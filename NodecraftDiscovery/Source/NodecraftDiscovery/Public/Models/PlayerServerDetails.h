// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BanDataObject.h"
#include "PlayerDataObject.h"
#include "Components/Widget.h"
#include "UObject/Object.h"
#include "Dom/JsonObject.h"
#include "PlayerServerDetails.generated.h"

class UPlayerServerDetailsDataObject;

DECLARE_DELEGATE_TwoParams(FOnPlayerSelectionChanged, UPlayerServerDetailsDataObject* /*Player*/, bool /*Selected*/);

// Called when we tap the little arrow on the right hand side, which will select the player and deselect all others
DECLARE_DELEGATE_OneParam(FOnPlayerSelectedExclusive, UPlayerServerDetailsDataObject* /*Player*/);

DECLARE_DELEGATE_OneParam(FOnPlayerItemReceivedFocus, UWidget* /*PlayerWidget*/);

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

	FOnPlayerItemReceivedFocus OnPlayerItemReceivedFocus;

	bool bIsSelectable;

	static UPlayerServerDetailsViewModel* FromDataObject(UPlayerServerDetailsDataObject* DataObject);


private:
	UPROPERTY()
	UPlayerServerDetailsDataObject* PlayerServerDetailsDataObject;

	// Whether the player is selected i.e. we've checked the checkbox next to their name
	bool bIsSelected;
};

