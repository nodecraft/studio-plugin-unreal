// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

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

	UPROPERTY(EditDefaultsOnly, Category = "Nodecraft UI|Alerts")
	TSoftObjectPtr<UTexture2D> Icon;

	UPROPERTY(EditDefaultsOnly, Category = "Nodecraft UI|Alerts")
	TSoftClassPtr<UCommonBorderStyle> OuterBorderStyle;

	UPROPERTY(EditDefaultsOnly, Category = "Nodecraft UI|Alerts")
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
	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Alerts", meta = (BindWidget))
	UImage* Icon;
	
	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Alerts", meta = (BindWidget))
	UCommonTextBlock* MessageText;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Alerts", meta = (BindWidget))
	UCommonBorder* OuterBorder;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Alerts", meta = (BindWidget))
	UCommonBorder* InnerBorder;

	UPROPERTY(EditDefaultsOnly, Category = "Nodecraft UI|Alerts")
	TMap<EAlertType, FAlertStyle> AlertStyles;
};
