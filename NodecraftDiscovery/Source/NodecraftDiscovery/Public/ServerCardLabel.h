// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CommonTextBlock.h"
#include "ServerCardLabelType.h"
#include "Components/Image.h"
#include "Components/Border.h"
#include "ServerCardLabel.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FServerCardLabelStyleConfig
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category = "Nodecraft UI|Styles")
	UTexture2D* LabelIconImage = nullptr;
	
	UPROPERTY(EditAnywhere, Category = "Nodecraft UI|Styles")
	FText LabelTitle;
	
	UPROPERTY(EditAnywhere, Category = "Nodecraft UI|Styles")
	FLinearColor BackgroundColor = FLinearColor::Transparent;
};
UCLASS()

class NODECRAFTDISCOVERY_API UServerCardLabel : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeOnInitialized() override;
	
	UFUNCTION(BlueprintCallable, Category="Nodecraft UI|Server Card Label")
	void SetLabelType(EServerCardLabelType LabelType);
	
protected:
	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Card Label", meta=(BindWidget))
	UBorder* Background;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Card Label", meta=(BindWidget))
	UImage* Icon;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Card Label", meta=(BindWidget))
	UCommonTextBlock* LabelText;
	
	UPROPERTY(EditDefaultsOnly, Category = "Nodecraft UI|Server Card Label")
	TMap<EServerCardLabelType, FServerCardLabelStyleConfig> StyleMap;

	UPROPERTY(EditInstanceOnly, Category = "Nodecraft UI|Server Card Label")
	EServerCardLabelType LabelType;
	
};
