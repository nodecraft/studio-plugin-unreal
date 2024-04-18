// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonTextBlock.h"
#include "Stores/FriendsStore.h"
#include "UI/Foundation/NodecraftButtonBase.h"
#include "FriendsButton.generated.h"

/**
 * 
 */
UCLASS()
class NODECRAFTDISCOVERY_API UFriendsButton : public UNodecraftButtonBase
{
	GENERATED_BODY()

private:
	FDelegateHandle FriendsUpdatedDelegateHandle;

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UCommonTextBlock* FriendsCountTextBlock;

public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
};
