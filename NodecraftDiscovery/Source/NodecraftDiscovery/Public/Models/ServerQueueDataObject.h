// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerSession.h"
#include "UObject/Object.h"
#include "ServerQueueDataObject.generated.h"

class UServerSessionDataObject;
/**
 * 
 */
UCLASS()
class NODECRAFTDISCOVERY_API UServerQueueDataObject : public UObject
{
	GENERATED_BODY()

public:
	static UServerQueueDataObject* FromJson(const TSharedRef<FJsonObject>& Json);
	
	UServerSessionDataObject* GetServerSession() const;
	int32 GetPlayersCount() const;
	int32 GetPlayersMax() const;
	int32 GetQueuePosition() const;
	int32 GetQueueSize() const;
	int32 GetEstimatedWait() const;
	
protected:
	UPROPERTY()
	UServerSessionDataObject* ServerSession;
	
	int32 PlayersCount;
	int32 PlayersMax;
	int32 QueuePosition;
	int32 QueueSize;
	int32 EstimatedWait;
};
