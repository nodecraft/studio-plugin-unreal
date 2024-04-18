// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Models/AllowsDataObject.h"
#include "AllowsStore.generated.h"

class UAllowsDataObject;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnAllowsUpdated, const TArray<UAllowsDataObject*>&);

/**
 * 
 */
UCLASS()
class NODECRAFTDISCOVERY_API UAllowsStore : public UEngineSubsystem
{
	GENERATED_BODY()

public:
	static UAllowsStore& Get()
	{
		return *GEngine->GetEngineSubsystem<UAllowsStore>();
	}

	FDelegateHandle AddAllowsListener(const FOnAllowsUpdated::FDelegate& Delegate);
	bool RemoveAllowsListener(const FDelegateHandle& Handle);
	
	// Prepends the new Allow to the AllowsForCurrentServer.
	void AddAllowToCurrentServer(UAllowsDataObject* Allow);
	void SetAllowsForCurrentServer(const TArray<UAllowsDataObject*> Allows);
	void UpdateAllow(UAllowsDataObject* Allow);
	void ClearAllowsForCurrentServer();

private:
	FOnAllowsUpdated OnAllowsUpdated;

	// List of server allows for the server currently in context
	// (i.e. currently opened server details page).
	// Empty when no server in context. Contains info on one server at a time.
	UPROPERTY()
	TArray<UAllowsDataObject*> AllowsForCurrentServer;
};
