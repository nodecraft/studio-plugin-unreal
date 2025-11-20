// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "Engine/DataTable.h"
#include "Components/ScrollBox.h"
#include "NodecraftScrollBox.generated.h"

UENUM()
enum class EScrollboxThumbstick : uint8
{
	Left,
	Right
};

UCLASS(Blueprintable)
class NODECRAFTDISCOVERY_API UNodecraftScrollBox : public UScrollBox
{
	GENERATED_BODY()

public:
	void SetListeningForInput(bool bShouldListen, TOptional<FText> InDisplayName = TOptional<FText>());

	bool GetListeningForInput();

	virtual void OnWidgetRebuilt() override;

	FSimpleMulticastDelegate OnScrolledToBottom;

protected:
	/** Is the scroll list currently listening for scroll input actions? */
	bool bIsListeningForInput = false;

	/** True to generally display this widget's actions in the action bar, assuming it has actions. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Nodecraft UI|Scroll Box|Input", meta = (AllowPrivateAccess = true))
	bool bDisplayInActionBar = true;

	/** The display name for this widget's actions in the action bar. */
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Nodecraft UI|Scroll Box|Input", meta = (AllowPrivateAccess = true))
	FText DisplayName;
	
	/** The input action we'll register (even though the action is a null op) to show that we can scroll.
	 *  By default, we will use the action from NodecraftStudioInputSettings, but you can override it here. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, AdvancedDisplay, Category = "Nodecraft UI|Scroll Box|Input", meta = (RowType = "/Script/CommonUI.CommonInputActionDataBase"))
	FDataTableRowHandle ScrollInputActionDataOverride;

	/** The thumbstick we want to use to scroll the menu. By default, we will select the corresponding action
	 * from NodecraftStudioInputSettings to display  in the action bar based on this value, but you can override
	 * what action to display in ScrollInputActionDataOverride. That action has no bearing on what input
	 * is used to scroll the menu though; it is entirely based on this value.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Nodecraft UI|Scroll Box|Input")
	EScrollboxThumbstick ScrollThumbstick = EScrollboxThumbstick::Left;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Nodecraft UI|Scroll Box|Input")
	float ScrollSpeed = 5.0f;

	float DeadZone = 0.1f;

	virtual void UpdateBindings();

	UFUNCTION()
	void ScrollMenu(float ScrollAmount);

private:
	FUIActionBindingHandle ScrollActionHandle;
	
	FUIActionBindingHandle RegisterUIActionBinding(const FBindUIActionArgs& BindActionArgs);
	void UnregisterUIActionBinding(FUIActionBindingHandle& BindingHandle);

	UFUNCTION()
	void CheckIfScrolledToBottom(float CurrentOffset);
};
