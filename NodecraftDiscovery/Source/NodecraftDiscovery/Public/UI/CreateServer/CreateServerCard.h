// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "CreateServerCard.generated.h"

class UCommonBorder;
class UNodecraftButtonBase;
/**
 * 
 */
UCLASS()
class NODECRAFTDISCOVERY_API UCreateServerCard : public UCommonActivatableWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	
	UPROPERTY(meta=(BindWidget))
	UNodecraftButtonBase* CreateServerButton;
	
	UPROPERTY(meta=(BindWidget))
	UCommonBorder* CardContentsContainer;

	UPROPERTY(EditDefaultsOnly)
	FLinearColor ContentTintOnHover;

	UPROPERTY(EditDefaultsOnly)
	FLinearColor ContentTintOnUnhover;

	UPROPERTY(EditDefaultsOnly)
	TSoftClassPtr<UCommonActivatableWidget> CreateServerModalClass;
};
