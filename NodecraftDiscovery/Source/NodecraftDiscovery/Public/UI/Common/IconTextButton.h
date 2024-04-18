// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/Foundation/NodecraftButtonBase.h"
#include "IconTextButton.generated.h"

class UCommonTextBlock;
class UImage;
/**
 * 
 */
UCLASS()
class NODECRAFTDISCOVERY_API UIconTextButton : public UNodecraftButtonBase
{
	GENERATED_BODY()

protected:
	virtual void NativePreConstruct() override;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UImage* Icon;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UCommonTextBlock* Text;

	UPROPERTY(EditAnywhere, Category="Button")
	TSoftObjectPtr<UTexture2D> IconImage;

	UPROPERTY(EditAnywhere, Category="Button")
	bool bShowIcon;
};
