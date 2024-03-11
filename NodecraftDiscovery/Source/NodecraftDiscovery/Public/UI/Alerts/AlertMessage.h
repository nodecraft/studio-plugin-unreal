// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AlertMessage.generated.h"

class UCommonBorder;
class UCommonBorderStyle;
class UImage;
class UCommonTextBlock;

UENUM()
enum class EAlertType
{
	Message,
	Success,
	Error
};

USTRUCT()
struct FAlertStyle
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UTexture2D> Icon;

	UPROPERTY(EditDefaultsOnly)
	TSoftClassPtr<UCommonBorderStyle> OuterBorderStyle;

	UPROPERTY(EditDefaultsOnly)
	TSoftClassPtr<UCommonBorderStyle> InnerBorderStyle;
};

/**
 * 
 */
UCLASS()
class NODECRAFTDISCOVERY_API UAlertMessage : public UUserWidget
{
	GENERATED_BODY()

public:
	void Show(const FText& Message, EAlertType AlertType);
	void Hide();

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UImage* Icon;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UCommonTextBlock* MessageText;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UCommonBorder* OuterBorder;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UCommonBorder* InnerBorder;

	UPROPERTY(EditDefaultsOnly)
	TMap<EAlertType, FAlertStyle> AlertStyles;
};
