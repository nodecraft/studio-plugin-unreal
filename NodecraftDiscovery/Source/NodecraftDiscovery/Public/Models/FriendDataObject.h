// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "FriendDataObject.generated.h"

class UServerDataObject;
class UPlayerDataObject;
/**
 * 
 */
UCLASS()
class NODECRAFTDISCOVERY_API UFriendDataObject : public UObject
{
	GENERATED_BODY()

public:
	static UFriendDataObject* FromJson(const TSharedRef<FJsonObject> Json);

	UPlayerDataObject* GetPlayer() const;
	UServerDataObject* GetServer() const;
	FText GetStatus() const;
	FText GetDateActive() const;

protected:
	UPROPERTY()
	UPlayerDataObject* Player;

	UPROPERTY()
	UServerDataObject* ServerDataObject;
	
	FString Status;
	FString DateActive;
};
