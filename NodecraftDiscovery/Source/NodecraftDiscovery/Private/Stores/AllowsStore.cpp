// Nodecraft, Inc. © 2012-2024, All Rights Reserved.


#include "Stores/AllowsStore.h"

FDelegateHandle UAllowsStore::AddAllowsListener(const FOnAllowsUpdated::FDelegate& Delegate)
{
	Delegate.ExecuteIfBound(AllowsForCurrentServer);
	return OnAllowsUpdated.Add(Delegate);
}

bool UAllowsStore::RemoveAllowsListener(const FDelegateHandle& Handle)
{
	return OnAllowsUpdated.Remove(Handle);
}

void UAllowsStore::SetAllowsForCurrentServer(const TArray<UAllowsDataObject*> Allows)
{
	AllowsForCurrentServer.Empty();
	AllowsForCurrentServer = Allows;
	OnAllowsUpdated.Broadcast(AllowsForCurrentServer);
}

void UAllowsStore::AddAllowToCurrentServer(UAllowsDataObject* Allow)
{
	AllowsForCurrentServer.Insert(Allow, 0);
	OnAllowsUpdated.Broadcast(AllowsForCurrentServer);
}

void UAllowsStore::UpdateAllow(UAllowsDataObject* Allow)
{
	for (int i = 0; i < AllowsForCurrentServer.Num(); ++i)
	{
		if (AllowsForCurrentServer[i]->GetID() == Allow->GetID())
		{
			AllowsForCurrentServer[i] = Allow;
			OnAllowsUpdated.Broadcast(AllowsForCurrentServer);
			return;
		}
	}
}

void UAllowsStore::ClearAllowsForCurrentServer()
{
	AllowsForCurrentServer.Empty();
}
