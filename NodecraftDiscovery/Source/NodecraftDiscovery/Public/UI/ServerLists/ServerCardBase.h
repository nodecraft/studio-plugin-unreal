// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "Common/NodecraftDelegates.h"
#include "UObject/Object.h"
#include "ServerCardBase.generated.h"

class UPaginatedServerList;

DECLARE_DELEGATE_OneParam(FOnServerCardFocusedInList, UPaginatedServerList* /*ServerList*/);

/**
 * Servers as a base class for the regular server cards and also for the Create Server button
 * which appears in the Private servers list. 
 */
UCLASS()
class NODECRAFTDISCOVERY_API UServerCardBase : public UCommonActivatableWidget
{
	GENERATED_BODY()

public:
	FOnNavigationDelegate OnNavDelegate;

	// This delegate is used to notify the server list that this card has been focused.
	FSimpleDelegate OnFocusedDelegate;
	
	virtual FNavigationReply NativeOnNavigation(const FGeometry& MyGeometry, const FNavigationEvent& InNavigationEvent, const FNavigationReply& InDefaultReply) override;

	virtual FReply NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent) override;
};
