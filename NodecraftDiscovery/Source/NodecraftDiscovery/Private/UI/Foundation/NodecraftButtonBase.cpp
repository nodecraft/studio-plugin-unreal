// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#include "UI/Foundation/NodecraftButtonBase.h"
#include "CommonActionWidget.h"
#include "CommonInputSubsystem.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(NodecraftButtonBase)

void UNodecraftButtonBase::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (UCommonInputSubsystem* CommonInputSubsystem = GetInputSubsystem())
	{
		if (CommonInputSubsystem->GetCurrentInputType() == ECommonInputType::Gamepad && bHideWhenUsingGamepad)
        {
            SetVisibility(ESlateVisibility::Collapsed);
        }
	}

	UpdateButtonStyle();
	RefreshButtonText();
}

void UNodecraftButtonBase::SetVisibility(ESlateVisibility InVisibility)
{
	if (UCommonInputSubsystem* CommonInputSubsystem = GetInputSubsystem())
	{
		if (CommonInputSubsystem->GetCurrentInputType() == ECommonInputType::Gamepad && bHideWhenUsingGamepad)
		{
			Super::SetVisibility(ESlateVisibility::Collapsed);
			return;
		}
	}
	Super::SetVisibility(InVisibility);
}

void UNodecraftButtonBase::BindTriggeringInputActionToClick()
{
	Super::BindTriggeringInputActionToClick();
	
	if (InputActionNameOverride.IsEmpty() == false && TriggeringBindingHandle.IsValid())
	{
		TriggeringBindingHandle.SetDisplayName(InputActionNameOverride);
	}
}

void UNodecraftButtonBase::UpdateInputActionWidget()
{
	Super::UpdateInputActionWidget();

	UpdateButtonStyle();
	RefreshButtonText();
}

void UNodecraftButtonBase::SetButtonText(const FText& InText)
{
	bOverride_ButtonText = InText.IsEmpty();
	ButtonText = InText;
	RefreshButtonText();
}

FText UNodecraftButtonBase::GetButtonText() const
{
	return ButtonText;
}

void UNodecraftButtonBase::NativeOnSelected(bool bBroadcast)
{
	Super::NativeOnSelected(bBroadcast);
	UpdateButtonStyle();
}

void UNodecraftButtonBase::NativeOnDeselected(bool bBroadcast)
{
	Super::NativeOnDeselected(bBroadcast);
	UpdateButtonStyle();
}

void UNodecraftButtonBase::RefreshButtonText()
{
	if (bOverride_ButtonText || ButtonText.IsEmpty())
	{
		if (InputActionWidget && bOverrideButtonTextWithInputActionDisplayName)
		{
			const FText ActionDisplayText = InputActionWidget->GetDisplayText();	
			if (!ActionDisplayText.IsEmpty())
			{
				UpdateButtonText(ActionDisplayText);
				return;
			}
		}
	}
	
	UpdateButtonText(ButtonText);	
}

FText UNodecraftButtonBase::GetInputActionNameOverride() const
{
	return InputActionNameOverride;
}

void UNodecraftButtonBase::OnInputMethodChanged(ECommonInputType CurrentInputType)
{
	Super::OnInputMethodChanged(CurrentInputType);
	
	if (bHideWhenUsingGamepad)
	{
		SetVisibility(CurrentInputType == ECommonInputType::Gamepad ? ESlateVisibility::Collapsed : ESlateVisibility::Visible);
	}
	UpdateButtonStyle();
}

void UNodecraftButtonBase::SetIsEnabled(bool bInIsEnabled)
{
	Super::SetIsEnabled(bInIsEnabled);
	UpdateButtonStyle();
}

FReply UNodecraftButtonBase::NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent)
{
	UpdateButtonStyle();
	return Super::NativeOnFocusReceived(InGeometry, InFocusEvent);
}
