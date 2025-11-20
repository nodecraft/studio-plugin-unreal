// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/Foundation/NodecraftLoadingButton.h"
#include "IconTextLoadingButton.generated.h"

class UHorizontalBox;
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

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI", meta = (BindWidget))
	UHorizontalBox* IconTextContainer;
	
	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI", meta = (BindWidget))
	UImage* Icon;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI", meta = (BindWidget))
	UCommonTextBlock* Text;

	UPROPERTY(EditAnywhere, Category = "Nodecraft UI")
	TSoftObjectPtr<UTexture2D> IconImage;

	UPROPERTY(EditAnywhere, Category = "Nodecraft UI")
	bool bShowIcon;

	UPROPERTY(EditAnywhere, Category = "Nodecraft UI")
	bool bIconOnRight;

	// Padding for the icon when it is on the left.
	// If the icon is on the right, the padding is reversed (left becomes right, right becomes left).
	UPROPERTY(EditAnywhere, Category = "Nodecraft UI", meta=(EditCondition="bShowIcon", DisplayName="Icon Padding (Icon on Left)"))
	FMargin IconPadding;

	UPROPERTY(EditAnywhere, Category = "Nodecraft UI")
	TEnumAsByte<EHorizontalAlignment> IconHorizontalAlignment = HAlign_Center;

	UPROPERTY(EditAnywhere, Category = "Nodecraft UI")
	TEnumAsByte<EVerticalAlignment> IconVerticalAlignment = VAlign_Center;

	UPROPERTY(EditAnywhere, Category = "Nodecraft UI")
	TEnumAsByte<EHorizontalAlignment> TextHorizontalAlignment = HAlign_Center;

	UPROPERTY(EditAnywhere, Category = "Nodecraft UI")
	TEnumAsByte<EVerticalAlignment> TextVerticalAlignment = VAlign_Center;

	UPROPERTY(EditAnywhere, Category = "Nodecraft UI")
	TEnumAsByte<EHorizontalAlignment> ContainerHorizontalAlignment = HAlign_Center;

	UPROPERTY(EditAnywhere, Category = "Nodecraft UI")
	TEnumAsByte<EVerticalAlignment> ContainerVerticalAlignment = VAlign_Center;
};
