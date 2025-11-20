// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonInputTypeEnum.h"
#include "Engine/DataTable.h"
#include "CommonUserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "NodecraftListItemBase.generated.h"

class UCommonBorderStyle;
class UCommonBorder;
class UNodecraftButtonBase;

UENUM()
enum class EUIActionBindingType : uint8
{
	Primary,
	Secondary
};

/**
 * 
 */
UCLASS(Abstract)
class NODECRAFTDISCOVERY_API UNodecraftListItemBase : public UCommonUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

public:
	void RegisterActionBinding(const EUIActionBindingType InBindingType, TOptional<FSimpleDelegate> InOnExecuteAction = TOptional<FSimpleDelegate>());
	void UnregisterUIActionBindings();

protected:
	virtual void NativePreConstruct() override;
	virtual void NativeOnInitialized() override;
	virtual FReply NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent) override;
	virtual void NativeOnFocusLost(const FFocusEvent& InFocusEvent) override;
	
	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI", meta=(BindWidgetOptional))
	UNodecraftButtonBase* PrimaryActionButton;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI", meta=(BindWidgetOptional))
	UNodecraftButtonBase* SecondaryActionButton;
	
	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI", meta=(BindWidgetOptional))
	UCommonBorder* Border;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Nodecraft UI|Appearance", meta=(InlineEditConditionToggle))
	bool bUseKeyboardAndMouseStyles = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Nodecraft UI|Appearance")
	TSubclassOf<UCommonBorderStyle> BorderStyleDefault_Gamepad;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Nodecraft UI|Appearance")
	TSubclassOf<UCommonBorderStyle> BorderStyleFocused_Gamepad;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Nodecraft UI|Appearance", meta=(EditCondition="bUseKeyboardAndMouseStyles"))
	TSubclassOf<UCommonBorderStyle> BorderStyleDefault_MouseAndKeyboard;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Nodecraft UI|Appearance", meta=(EditCondition="bUseKeyboardAndMouseStyles"))
	TSubclassOf<UCommonBorderStyle> BorderStyleFocused_MouseAndKeyboard;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Nodecraft UI|Appearance")
	FMargin BorderPaddingDefault_Gamepad = FMargin(0.f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Nodecraft UI|Appearance")
	FMargin BorderPaddingFocused_Gamepad = FMargin(0.f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Nodecraft UI|Appearance", meta=(EditCondition="bUseKeyboardAndMouseStyles"))
	FMargin BorderPaddingDefault_MouseAndKeyboard = FMargin(0.f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Nodecraft UI|Appearance", meta=(EditCondition="bUseKeyboardAndMouseStyles"))
	FMargin BorderPaddingFocused_MouseAndKeyboard = FMargin(0.f);
	
	UPROPERTY(EditAnywhere, Category = "Nodecraft UI|Input", meta=(RowType="/Script/CommonUI.CommonInputActionDataBase"))
	FDataTableRowHandle PrimaryInputActionData;

	UPROPERTY(EditAnywhere, Category = "Nodecraft UI|Input", meta=(RowType="/Script/CommonUI.CommonInputActionDataBase"))
	FDataTableRowHandle SecondaryInputActionData;

	UPROPERTY(EditAnywhere, Category = "Nodecraft UI|Input", AdvancedDisplay)
	FText PrimaryActionNameOverride;

	UPROPERTY(EditAnywhere, Category = "Nodecraft UI|Input", AdvancedDisplay)
	FText SecondaryActionNameOverride;
	
	FUIActionBindingHandle PrimaryUIActionHandle;
	FUIActionBindingHandle SecondaryUIActionHandle;

	void ResetStyles(ECommonInputType InputType);

#if WITH_EDITORONLY_DATA
	UPROPERTY(EditAnywhere, Category = "Nodecraft UI|Debug", meta=(InlineEditConditionToggle))
	bool bPreviewStyles = true;
	
	UPROPERTY(EditAnywhere, Category = "Nodecraft UI|Debug", meta=(EditCondition="bPreviewStyles"))
	ECommonInputType PreviewInputType = ECommonInputType::Gamepad;

	UPROPERTY(EditAnywhere, Category = "Nodecraft UI|Debug", meta=(EditCondition="PreviewInputType == ECommonInputType::MouseAndKeyboard"))
	bool bPreviewDefaultMouseAndKeyboardStyles = false;

	UPROPERTY(EditAnywhere, Category = "Nodecraft UI|Debug", meta=(EditCondition="PreviewInputType == ECommonInputType::MouseAndKeyboard"))
	bool bPreviewFocusedMouseAndKeyboardStyles = false;
	
	UPROPERTY(EditAnywhere, Category = "Nodecraft UI|Debug", meta=(EditCondition="PreviewInputType == ECommonInputType::Gamepad"))
	bool bPreviewDefaultControllerStyles = false;

	UPROPERTY(EditAnywhere, Category = "Nodecraft UI|Debug", meta=(EditCondition="PreviewInputType == ECommonInputType::Gamepad"))
	bool bPreviewFocusedControllerStyles = false;
#endif
};
