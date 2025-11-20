// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Dom/JsonObject.h"
#include "PaginationDataObject.generated.h"

/**
 * 
 */
UCLASS()
class NODECRAFTDISCOVERY_API UPaginationDataObject : public UObject
{
	GENERATED_BODY()

public:
	static UPaginationDataObject* FromJson(const TSharedRef<FJsonObject> Json);

	int32 GetCount() const;

	// Based on index 1
	int32 GetPage() const;
	
	int32 GetPagesMax() const;
	int32 GetEntriesPerPage() const;
	bool IsUninitialized() const;

protected:
	int32 Count = 0;
	
	// Based on index 1
	int32 Page = 0;
	
	int32 PagesMax = 0;
	int32 EntriesPerPage = 0;
	bool bInitializedFromBackendData = false;
};
