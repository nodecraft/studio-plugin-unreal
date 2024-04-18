// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Models/ServerDataObject.h"
#include "UObject/Interface.h"
#include "ServerDetailsSection.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UServerDetailsSection : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class NODECRAFTDISCOVERY_API IServerDetailsSection
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void SetServerData(UServerDataObject* ServerDataObject) = 0;
};
