// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
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

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual UWidget* NativeGetDesiredFocusTarget() const override;
	
protected:
	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Friends List", meta=(BindWidget))
	UCommonLoadGuard* LoadGuard;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Friends List", meta=(BindWidget))
	UCommonListView* FriendListView;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Friends List", meta=(BindWidget))
	UCommonTextBlock* FriendCountText;

private:
	FDelegateHandle FriendsUpdatedDelegateHandle;
};
