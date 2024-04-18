// Nodecraft, Inc. © 2012-2024, All Rights Reserved.


#include "Models/PlayerListDataObject.h"
#include "Models/PlayerDataObject.h"

UPlayerListDataObject* UPlayerListDataObject::FromJson(const TSharedRef<FJsonObject> Json, FString PlayersKeyToCheck)
{
	UPlayerListDataObject* PlayerListDataObject = NewObject<UPlayerListDataObject>();
	
	Json->TryGetNumberField("players_count", PlayerListDataObject->PlayersCount);
    Json->TryGetNumberField("players_max", PlayerListDataObject->PlayersMax);
	
	const TArray<TSharedPtr<FJsonValue>> Players = Json->GetArrayField(PlayersKeyToCheck);
	for (const TSharedPtr<FJsonValue> Player : Players)
	{
		UPlayerDataObject* PDO = UPlayerDataObject::FromJson(Player->AsObject().ToSharedRef());
		PlayerListDataObject->PlayerDataObjects.Add(PDO);
	}
	
	return PlayerListDataObject;
}

int32 UPlayerListDataObject::GetPlayersCount() const
{
	return PlayersCount;
}

int32 UPlayerListDataObject::GetMaxPlayersCount() const
{
	return PlayersMax;
}

TArray<UPlayerDataObject*> UPlayerListDataObject::GetPlayerDataObjects()
{
	return PlayerDataObjects;
}

void UPlayerListDataObject::SetPlayerDataObjects(const TArray<UPlayerDataObject*>& InPlayerDataObjects)
{
	PlayerDataObjects = InPlayerDataObjects;
}
