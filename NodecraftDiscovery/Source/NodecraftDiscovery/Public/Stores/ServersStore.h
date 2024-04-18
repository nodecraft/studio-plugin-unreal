// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Models/ServerDataObject.h"
#include "Subsystems/EngineSubsystem.h"
#include "ServersStore.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnFavoriteServersUpdated, const TArray<UServerDataObject*>);

UCLASS()
class NODECRAFTDISCOVERY_API UServersStore : public UEngineSubsystem
{
	GENERATED_BODY()

public:
	static UServersStore& Get() { return *GEngine->GetEngineSubsystem<UServersStore>(); }

	void SetFavoriteServers(const TArray<UServerDataObject*>& Servers);

	void AddFavoriteServer(UServerDataObject* Server);

	void RemoveFavoriteServer(UServerDataObject* Server);

	FOnFavoriteServersUpdated OnFavoriteServersUpdated;

private:
	UPROPERTY()
	TArray<UServerDataObject*> FavoriteServers;

};
