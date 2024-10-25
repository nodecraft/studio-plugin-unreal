// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "Common/NodecraftDelegates.h"
#include "UI/ServerLists/ServerCardBase.h"
#include "CreateServerCard.generated.h"

class UCommonBorder;
class UNodecraftButtonBase;
/**
 * 
 */
UCLASS()
class NODECRAFTDISCOVERY_API UCreateServerCard : public UServerCardBase
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	
	UPROPERTY(meta=(BindWidget))
	UNodecraftButtonBase* CreateServerButton;
	
	UPROPERTY(meta=(BindWidget))
	UCommonBorder* CardContentsContainer;

	UPROPERTY(EditDefaultsOnly, Category="Nodecraft UI|Style")
	FLinearColor ContentTintOnHover;

	UPROPERTY(EditDefaultsOnly, Category="Nodecraft UI|Style")
	FLinearColor ContentTintOnUnhover;

	UPROPERTY(EditDefaultsOnly, Category="Nodecraft UI")
	TSoftClassPtr<UCommonActivatableWidget> CreateServerModalClass;
};
