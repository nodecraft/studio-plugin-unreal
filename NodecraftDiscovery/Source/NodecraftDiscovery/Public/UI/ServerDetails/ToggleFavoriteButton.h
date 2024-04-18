// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/Common/IconTextLoadingButton.h"
#include "ToggleFavoriteButton.generated.h"

/**
 * 
 */
UCLASS()
class NODECRAFTDISCOVERY_API UToggleFavoriteButton : public UIconTextLoadingButton
{
	GENERATED_BODY()

public:
	void SetIsFavorited(bool bIsFavorited);

protected:
	UPROPERTY(EditAnywhere, Category="Favorite Button")
	TSoftClassPtr<UCommonButtonStyle> ButtonStyleFavorited;

	UPROPERTY(EditAnywhere, Category="Favorite Button")
	TSoftClassPtr<UCommonButtonStyle> ButtonStyleNotFavorited;
};
