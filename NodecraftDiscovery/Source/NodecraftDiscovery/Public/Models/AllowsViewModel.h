// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "AllowsViewModel.generated.h"

class UServerDataObject;
class UAllowsDataObject;
/**
 * 
 */
UCLASS()
class NODECRAFTDISCOVERY_API UAllowsViewModel : public UObject
{
	GENERATED_BODY()

public:
	static UAllowsViewModel* FromDataObjects(const UAllowsDataObject* Allow, const UServerDataObject* Server);

	const UAllowsDataObject* GetAllow() const;
	const UServerDataObject* GetServer() const;

protected:
	UPROPERTY()
	const UAllowsDataObject* Allow;

	UPROPERTY()
	const UServerDataObject* Server;
};
