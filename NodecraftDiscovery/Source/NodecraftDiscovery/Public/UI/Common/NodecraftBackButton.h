// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "NodecraftBackButton.generated.h"

class UCommonTextStyle;
class UNodecraftButtonBase;
class UCommonTextBlock;
class UImage;

USTRUCT()
struct FBackButtonStyle
{
	GENERATED_BODY()

	FBackButtonStyle() : Material(nullptr), Tint(FLinearColor::White) {  }

	UPROPERTY(EditDefaultsOnly)
	TSoftClassPtr<UCommonTextStyle> TextStyle;

	UPROPERTY(EditDefaultsOnly)
	UMaterialInterface* Material;

	UPROPERTY(EditDefaultsOnly)
	FLinearColor Tint;
};
/**
 * 
 */
UCLASS()
class NODECRAFTDISCOVERY_API UNodecraftBackButton : public UCommonActivatableWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(meta=(BindWidget))
	UNodecraftButtonBase* Button;

	UPROPERTY(meta=(BindWidget))
	UImage* BackgroundCircle;

	UPROPERTY(meta=(BindWidget))
	UImage* BackArrow;

	UPROPERTY(meta=(BindWidget))
	UCommonTextBlock* Text;

	UPROPERTY(EditDefaultsOnly)
	FBackButtonStyle NormalStyle;

	UPROPERTY(EditDefaultsOnly)
	FBackButtonStyle HoveredStyle;
};
