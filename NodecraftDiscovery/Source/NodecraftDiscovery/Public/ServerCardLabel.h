// Fill out your copyright notice in the Description page of Project Settings.

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
	
	UPROPERTY(EditAnywhere)
	UTexture2D* LabelIconImage = nullptr;
	
	UPROPERTY(EditAnywhere)
	FText LabelTitle;
	
	UPROPERTY(EditAnywhere)
	FLinearColor BackgroundColor = FLinearColor::Transparent;
};
UCLASS()

class NODECRAFTDISCOVERY_API UServerCardLabel : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeOnInitialized() override;
	
	UFUNCTION(BlueprintCallable)
	void SetLabelType(EServerCardLabelType LabelType);
	
protected:
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UBorder* Background;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UImage* Icon;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UCommonTextBlock* LabelText;
	
	UPROPERTY(EditDefaultsOnly)
	TMap<EServerCardLabelType, FServerCardLabelStyleConfig> StyleMap;

	UPROPERTY(EditInstanceOnly)
	EServerCardLabelType LabelType;
	
};
