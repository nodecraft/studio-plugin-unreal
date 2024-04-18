// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "AccountReputationEntryDataObject.generated.h"

class UCommunityDataObject;
enum class EModerationScope : uint8;
enum class EModerationDuration : uint8;
class UServerDataObject;
/**
 * 
 */
UCLASS()
class NODECRAFTDISCOVERY_API UAccountReputationEntryDataObject : public UObject
{
	GENERATED_BODY()

public:
	static UAccountReputationEntryDataObject* FromJson(const TSharedRef<FJsonObject>& Json);

	UServerDataObject* GetServer() const;
	UCommunityDataObject* GetCommunity() const;
	FDateTime GetDateCreated() const;
	FDateTime GetDateExpires() const;
	EModerationDuration GetModerationDuration() const;
	EModerationScope GetModerationScope() const;

protected:
	// can be valid or null: when null, then ban is not tied to a specific server
	UPROPERTY()
	UServerDataObject* ServerDataObject;

	// can be valid or null: when null, then ban is not tied to a specific community
	UPROPERTY()
	UCommunityDataObject* CommunityDataObject;
	
	FDateTime DateCreated;

	// if empty, then the ban is permanent.
	FDateTime DateExpires;

	EModerationDuration ModerationDuration;
	EModerationScope ModerationScope;
};
