// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/Foundation/NodecraftLoadingButton.h"
#include "IconTextLoadingButton.generated.h"

class UCommonTextBlock;
class UImage;
/**
 * 
 */
UCLASS()
class NODECRAFTDISCOVERY_API UIconTextLoadingButton : public UNodecraftLoadingButton
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
