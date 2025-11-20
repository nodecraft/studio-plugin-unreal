// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "CommonTextBlock.h"
#include "Components/Image.h"
#include "UI/Foundation/NodecraftButtonBase.h"
#include "ModerationActionButton.generated.h"

enum class EModerationAction : uint8;

USTRUCT()
struct FModerationActionButtonStyle
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Nodecraft UI|Styles")
	TSoftObjectPtr<UTexture2D> Icon;
	
	UPROPERTY(EditAnywhere, Category = "Nodecraft UI|Styles")
	FText Label;
	
	UPROPERTY(EditAnywhere, Category = "Nodecraft UI|Styles")
	FText Description;
};

/**
 * 
 */
UCLASS()
class NODECRAFTDISCOVERY_API UModerationActionButton : public UNodecraftButtonBase
{
	GENERATED_BODY()

private:
	UPROPERTY(EditInstanceOnly, Category = "Nodecraft UI|Server Details")
	EModerationAction Action;

	void ConfigureForAction(EModerationAction InAction);

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Details", meta = (BindWidget))
	UImage* ActionIcon;
	
	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Details", meta = (BindWidget))
	UCommonTextBlock* ActionLabel;
	
	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Details", meta = (BindWidget))
	UCommonTextBlock* ActionDescription;

	UPROPERTY(EditDefaultsOnly, Category = "Nodecraft UI|Server Details")
	TMap<EModerationAction, FModerationActionButtonStyle> ActionStyles;

	UFUNCTION(BlueprintCallable, Category = "Nodecraft UI|Server Details")
	void Refresh();
};
