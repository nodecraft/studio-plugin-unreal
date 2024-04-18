// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Utility/NodecraftUtility.h"
#include "InviteFriendViewModel.generated.h"

class UFriendDataObject;
class UServerDataObject;
/**
 * 
 */
UCLASS()
class NODECRAFTDISCOVERY_API UInviteFriendViewModel : public UObject
{
	GENERATED_BODY()

public:
	static UInviteFriendViewModel* FromDataObjects(const UServerDataObject* Server, const UFriendDataObject* Invitee, FSimpleDelegate InClosePopupDelegate);

	const UServerDataObject* GetServer() const;
	const UFriendDataObject* GetInvitee() const;

	FSimpleDelegate ClosePopupDelegate;
	FOnRequestAlertMessage OnRequestAlertMessage;
	
protected:
	UPROPERTY()
	const UServerDataObject* ServerDataObject;

	UPROPERTY()
	const UFriendDataObject* InviteeDataObject;

};
