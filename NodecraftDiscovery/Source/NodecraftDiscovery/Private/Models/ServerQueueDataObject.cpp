// Nodecraft, Inc. © 2012-2024, All Rights Reserved.


#include "Models/ServerQueueDataObject.h"

#include "Models/ServerSessionDataObject.h"

UServerQueueDataObject* UServerQueueDataObject::FromJson(const TSharedRef<FJsonObject>& Json)
{
	UServerQueueDataObject* ServerQueueDataObject = NewObject<UServerQueueDataObject>();

	const TSharedPtr<FJsonObject>* Session;
	if (Json->TryGetObjectField("session", Session))
	{
		ServerQueueDataObject->ServerSession = UServerSessionDataObject::FromJson(Session->ToSharedRef());
	}
	ServerQueueDataObject->PlayersCount = Json->GetNumberField("players_count");
	ServerQueueDataObject->PlayersMax = Json->GetNumberField("players_max");
	ServerQueueDataObject->QueuePosition = Json->GetNumberField("queue_position");
	ServerQueueDataObject->QueueSize = Json->GetNumberField("queue_size");
	ServerQueueDataObject->EstimatedWait = Json->GetNumberField("estimated_wait");

	return ServerQueueDataObject;	
}

UServerSessionDataObject* UServerQueueDataObject::GetServerSession() const
{
	return ServerSession;
}

int32 UServerQueueDataObject::GetPlayersCount() const
{
	return PlayersCount;
}

int32 UServerQueueDataObject::GetPlayersMax() const
{
	return PlayersMax;
}

int32 UServerQueueDataObject::GetQueuePosition() const
{
	return QueuePosition;
}

int32 UServerQueueDataObject::GetQueueSize() const
{
	return QueueSize;
}

int32 UServerQueueDataObject::GetEstimatedWait() const
{
	return EstimatedWait;
}
