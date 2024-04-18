// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "Stores/FriendsStore.h"
#include "FriendsList.generated.h"

class UCommonTextBlock;
class UFriendDataObject;
class UCommonListView;
class UCommonLoadGuard;
/**
 * 
 */
UCLASS()
class NODECRAFTDISCOVERY_API UFriendsList : public UCommonActivatableWidget
{
	GENERATED_BODY()

private:
	FDelegateHandle FriendsUpdatedDelegateHandle;

protected:
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UCommonLoadGuard* LoadGuard;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UCommonListView* FriendListView;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UCommonTextBlock* FriendCountText;

public:

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;	
};
