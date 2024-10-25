// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Nodecraft UI|Appearance")
	TSubclassOf<UCommonBorderStyle> BorderStyleDefault;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Nodecraft UI|Appearance")
	TSubclassOf<UCommonBorderStyle> BorderStyleFocused;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Nodecraft UI|Appearance")
	FMargin BorderPaddingDefault = FMargin(0.f);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Nodecraft UI|Appearance")
	FMargin BorderPaddingFocused = FMargin(0.f);
	
	UPROPERTY(EditAnywhere, Category = Input, meta=(RowType="/Script/CommonUI.CommonInputActionDataBase"))
	FDataTableRowHandle PrimaryInputActionData;

	UPROPERTY(EditAnywhere, Category = "Nodecraft UI|Input", meta=(RowType="/Script/CommonUI.CommonInputActionDataBase"))
	FDataTableRowHandle SecondaryInputActionData;

	UPROPERTY(EditAnywhere, Category = "Nodecraft UI|Input", AdvancedDisplay)
	FText PrimaryActionNameOverride;

	UPROPERTY(EditAnywhere, Category = "Nodecraft UI|Input", AdvancedDisplay)
	FText SecondaryActionNameOverride;
	
	FUIActionBindingHandle PrimaryUIActionHandle;
	FUIActionBindingHandle SecondaryUIActionHandle;

	UFUNCTION(BlueprintCallable, Category = "Nodecraft UI")
	void ResetBorderStyle();
};
