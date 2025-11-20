// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "CommonTextBlock.h"
#include "Components/Image.h"
#include "UI/Foundation/NodecraftButtonBase.h"
#include "ConsentLabelButton.generated.h"

/**
 * 
 */
UCLASS()
class NODECRAFTDISCOVERY_API UConsentLabelButton : public UNodecraftButtonBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI", meta = (BindWidget))
	UWidget* ConsentStatusContainer;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI", meta = (BindWidget))
	UCommonTextBlock* ConsentStatusTextBlock;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI", meta = (BindWidget))
	UWidget* LoadingSpinnerContainer;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI", meta = (BindWidget))
	UImage* TextIconDecorator;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI", EditAnywhere)
	TSoftObjectPtr<UTexture2D> EnabledTextDecorator;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI", EditAnywhere)
	TSoftObjectPtr<UTexture2D> DisasbledTextDecorator;

public:
	void ShowTextDecoration(bool bShow);

	virtual void NativeConstruct() override;

	virtual void SetIsEnabled(bool bInIsEnabled) override;

	void SetIsLoading(bool bIsLoading);
};
