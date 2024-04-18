// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonTextBlock.h"
#include "CommonUserWidget.h"
#include "Components/Image.h"
#include "UI/Foundation/NodecraftButtonBase.h"
#include "ModerationActionButton.generated.h"

UENUM()
enum class EModerationAction : uint8
{
	Undefined,
	Kick,
	Ban,
	Unban,
	PromoteToModerator,
	DemoteModerator
};

USTRUCT()
struct FModerationActionButtonStyle
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UTexture2D> Icon;
	
	UPROPERTY(EditAnywhere)
	FText Label;
	
	UPROPERTY(EditAnywhere)
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
	UPROPERTY(EditInstanceOnly)
	EModerationAction Action;

	void ConfigureForAction(EModerationAction InAction);

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UImage* ActionIcon;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UCommonTextBlock* ActionLabel;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UCommonTextBlock* ActionDescription;

	UPROPERTY(EditDefaultsOnly)
	TMap<EModerationAction, FModerationActionButtonStyle> ActionStyles;

	UFUNCTION(BlueprintCallable)
	void Refresh();
};
