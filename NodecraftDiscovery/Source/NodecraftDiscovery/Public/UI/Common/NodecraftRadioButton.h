// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "Common/NodecraftDelegates.h"
#include "NodecraftRadioButton.generated.h"

class UCommonBorder;
class UCommonBorderStyle;
class UOverlay;
class UCommonTextStyle;
class UCommonTextBlock;
class UNodecraftRadioButton;
class UNodecraftButtonBase;
class UImage;

USTRUCT()
struct FNodecraftRadioButtonConfiguration
{
	GENERATED_BODY()

	FNodecraftRadioButtonConfiguration()
		: Id(NAME_None)
		, PrimaryLabel(FText::GetEmpty())
		, SecondaryLabel(FText::GetEmpty())
		, TertiaryLabel(FText::GetEmpty())
		, CreatedRadioButton(nullptr)
	{}

	UPROPERTY(EditAnywhere, Category = "Nodecraft UI|Radio Button")
	FName Id;

	UPROPERTY(EditAnywhere, Category = "Nodecraft UI|Radio Button")
	FText PrimaryLabel;

	UPROPERTY(EditAnywhere, Category = "Nodecraft UI|Radio Button")
	FText SecondaryLabel;

	UPROPERTY(EditAnywhere, Category = "Nodecraft UI|Radio Button")
	FText TertiaryLabel;

	UPROPERTY(Transient)
	TObjectPtr<UNodecraftRadioButton> CreatedRadioButton;
};

USTRUCT()
struct FRadioButtonStyle
{
	GENERATED_BODY()

	FRadioButtonStyle()
		: UncheckedTint(FColor::White)
		, CheckedTint(FColor::White)
	{}

	UPROPERTY(EditAnywhere, Category = "Nodecraft UI|Radio Button|Appearance")
	FSlateBrush BackgroundImage;
	
	UPROPERTY(EditAnywhere, Category = "Nodecraft UI|Radio Button|Appearance")
	FSlateBrush UncheckedImage;

	UPROPERTY(EditAnywhere, Category = "Nodecraft UI|Radio Button|Appearance")
	FSlateBrush CheckedImage;
	
	UPROPERTY(EditAnywhere, Category = "Nodecraft UI|Radio Button|Appearance")
	FColor UncheckedTint;

	UPROPERTY(EditAnywhere, Category = "Nodecraft UI|Radio Button|Appearance")
	FColor CheckedTint;

	UPROPERTY(EditAnywhere, Category = "Nodecraft UI|Radio Button|Appearance")
	FMargin RadioPadding;

	UPROPERTY(EditAnywhere, Category = "Nodecraft UI|Radio Button|Appearance")
	TSubclassOf<UCommonBorderStyle> ButtonBorderStyleDefault;

	UPROPERTY(EditAnywhere, Category = "Nodecraft UI|Radio Button|Appearance")
	TSubclassOf<UCommonBorderStyle> ButtonBorderStyleFocused;

	UPROPERTY(EditAnywhere, Category = "Nodecraft UI|Radio Button|Appearance")
	FMargin BorderPaddingDefault = FMargin(0.f);

	UPROPERTY(EditAnywhere, Category = "Nodecraft UI|Radio Button|Appearance")
	FMargin BorderPaddingFocused = FMargin(0.f);
};

USTRUCT()
struct FRadioButtonTextStyle
{
	GENERATED_BODY()

	FRadioButtonTextStyle() {}

	bool IsValid() const { return TextStyle.IsNull() == false && SelectedTextStyle.IsNull() == false; }

	UPROPERTY(EditAnywhere, Category = "Nodecraft UI|Radio Button|Text Style")
	TSoftClassPtr<UCommonTextStyle> TextStyle;

	UPROPERTY(EditAnywhere, Category = "Nodecraft UI|Radio Button|Text Style")
	TSoftClassPtr<UCommonTextStyle> SelectedTextStyle;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRadioButtonSelected, int32, ButtonIndex);
/**
 * 
 */
UCLASS()
class NODECRAFTDISCOVERY_API UNodecraftRadioButton : public UCommonUserWidget
{
	GENERATED_BODY()

public:
	virtual void SynchronizeProperties() override;
	
	void Configure(const FNodecraftRadioButtonConfiguration& RadioButtonDescriptor, int32 InButtonIndex);
	bool GetIsChecked() const;
	int32 GetButtonIndex() const { return ButtonIndex; }
	FName GetButtonId() const { return Descriptor.Id; }

	void SetIsChecked(bool bIsChecked);
	void SetPrimaryLabelText(const FText& InText);
	void SetSecondaryLabelText(const FText& InText);
	void SetTertiaryLabelText(const FText& InText);
	void SetTextStyles(const FRadioButtonTextStyle& InPrimaryTextStyle, const FRadioButtonTextStyle& InSecondaryTextStyle, const FRadioButtonTextStyle& InTertiaryTextStyle);
	void SetBorderStyles(TSubclassOf<UCommonBorderStyle> InBorderStyleDefault, TSubclassOf<UCommonBorderStyle> InBorderStyleFocused);
	void SetBorderPadding(const FMargin& InBorderPaddingDefault, const FMargin& InBorderPaddingFocused);
	void SetTriggeringInputAction(const FDataTableRowHandle& InInputActionData, TOptional<FText> InInputActionNameOverride = TOptional<FText>());

	FOnRadioButtonSelected OnRadioButtonSelected;
	FOnNavigationDelegate OnNavigation;
	
protected:
	virtual void NativeOnInitialized() override;
	virtual FNavigationReply NativeOnNavigation(const FGeometry& MyGeometry, const FNavigationEvent& InNavigationEvent, const FNavigationReply& InDefaultReply) override;
	virtual FReply NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent) override;
	virtual void NativeOnFocusLost(const FFocusEvent& InFocusEvent) override;

	UPROPERTY(EditAnywhere, Category = "Nodecraft UI|Radio Button|Appearance")
	FRadioButtonStyle Style;
	
	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Radio Button", meta = (BindWidget))
	UOverlay* RadioContainer;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Radio Button", meta = (BindWidgetOptional))
	UCommonBorder* Border;
	
	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Radio Button", meta = (BindWidget))
	UNodecraftButtonBase* Button;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Radio Button", meta = (BindWidget))
	UCommonTextBlock* PrimaryLabel;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Radio Button", meta = (BindWidgetOptional))
	UCommonTextBlock* SecondaryLabel;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Radio Button", meta = (BindWidgetOptional))
	UCommonTextBlock* TertiaryLabel;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Radio Button", meta = (BindWidget))
	UImage* ButtonStateImage;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Radio Button", meta = (BindWidget))
	UImage* ButtonStateBackground;

	UPROPERTY(EditAnywhere, Category = "Nodecraft UI|Radio Button|Appearance", meta=(InlineEditConditionToggle))
	bool bOverrideLabels = false;

private:	
	void SetupVisuals();
	void UpdateTextStyles(bool bIsChecked);
	void UpdateBorderStyles(bool bIsFocused);
	
	UPROPERTY(EditAnywhere, Category = "Nodecraft UI|Radio Button|Appearance", meta=(EditCondition="bOverrideLabels"))
	FNodecraftRadioButtonConfiguration Descriptor;

	FDataTableRowHandle ButtonTriggeringInputAction;
	FUIActionBindingHandle InputActionHandle;
	TOptional<FText> TriggeringActionNameOverride;
	
	int32 ButtonIndex = INDEX_NONE;
	FRadioButtonTextStyle PrimaryTextStyle;
	FRadioButtonTextStyle SecondaryTextStyle;
	FRadioButtonTextStyle TertiaryTextStyle;
};
