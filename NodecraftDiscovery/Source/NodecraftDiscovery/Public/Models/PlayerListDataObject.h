// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "PlayerListDataObject.generated.h"

class UPlayerDataObject;
/**
 * 
 */
UCLASS()
class NODECRAFTDISCOVERY_API UPlayerListDataObject : public UObject
{
	GENERATED_BODY()

public:
	// if there's no optional string passed in, then we're checking the root (data) key
	static UPlayerListDataObject* FromJson(const TSharedRef<FJsonObject> Json, FString PlayersKeyToCheck = "data");
	
	int32 GetPlayersCount() const;
	int32 GetMaxPlayersCount() const;
	TArray<UPlayerDataObject*> GetPlayerDataObjects();
	void SetPlayerDataObjects(const TArray<UPlayerDataObject*>& InPlayerDataObjects);

private:
	int32 PlayersCount;
	int32 PlayersMax;

	UPROPERTY()
	TArray<UPlayerDataObject*> PlayerDataObjects;
};
