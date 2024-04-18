﻿// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/TextBlock.h"
#include "UI/Foundation/NodecraftButtonBase.h"
#include "NodecraftBadgedButton.generated.h"

/**
 * 
 */
UCLASS()
class NODECRAFTDISCOVERY_API UNodecraftBadgedButton : public UNodecraftButtonBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditInstanceOnly, Category = "Nodecraft Badged Button")
	TSoftClassPtr<UCommonButtonStyle> UnbadgedButtonStyle;

	UPROPERTY(EditInstanceOnly, Category = "Nodecraft Badged Button")
	TSoftClassPtr<UCommonButtonStyle> BadgedButtonStyle;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UPanelWidget* BadgeContainer;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UTextBlock* BadgeText;

public:
	void SetBadgeText(const FText& InText);

	void SetIsBadged(bool bIsBadged);

	virtual void NativeConstruct() override;
};
