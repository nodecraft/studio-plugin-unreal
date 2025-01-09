// Nodecraft, Inc. © 2012-2024, All Rights Reserved.


#include "UI/Common/NodecraftListItemBase.h"

#include "CommonBorder.h"
#include "CommonInputSubsystem.h"
#include "Input/CommonUIInputTypes.h"
#include "UI/Foundation/NodecraftButtonBase.h"
#include "Utility/NodecraftMacros.h"

void UNodecraftListItemBase::RegisterActionBinding(const EUIActionBindingType InBindingType, TOptional<FSimpleDelegate> InOnExecuteAction)
{
	switch (InBindingType)
	{
	case EUIActionBindingType::Primary:
		if (PrimaryActionButton && PrimaryInputActionData.IsNull() == false)
		{
			FSimpleDelegate OnExecuteAction = InOnExecuteAction.IsSet()
			? InOnExecuteAction.GetValue()
			: FSimpleDelegate::CreateWeakLambda(this, [this]()
			{
				PrimaryActionButton->OnClicked().Broadcast();
			});
			FBindUIActionArgs UIActionArgs = FBindUIActionArgs(PrimaryInputActionData, bDisplayInActionBar, OnExecuteAction);
			if (PrimaryActionNameOverride.IsEmpty() == false)
			{
				UIActionArgs.OverrideDisplayName = PrimaryActionNameOverride;
			}
			PrimaryUIActionHandle = RegisterUIActionBinding(UIActionArgs);
		}
		break;
	case EUIActionBindingType::Secondary:
		if (SecondaryActionButton && SecondaryInputActionData.IsNull() == false)
		{
			FSimpleDelegate OnExecuteAction = InOnExecuteAction.IsSet()
			? InOnExecuteAction.GetValue()
			: FSimpleDelegate::CreateWeakLambda(this, [this]()
			{
				SecondaryActionButton->OnClicked().Broadcast();
			});
			FBindUIActionArgs UIActionArgs = FBindUIActionArgs(SecondaryInputActionData, bDisplayInActionBar, OnExecuteAction);
			if (SecondaryActionNameOverride.IsEmpty() == false)
			{
				UIActionArgs.OverrideDisplayName = SecondaryActionNameOverride;
			}
			SecondaryUIActionHandle = RegisterUIActionBinding(UIActionArgs);
		}
		break;
	}
}

void UNodecraftListItemBase::UnregisterUIActionBindings()
{
	if (PrimaryUIActionHandle.IsValid())
	{
		PrimaryUIActionHandle.Unregister();
	}

	if (SecondaryUIActionHandle.IsValid())
	{
		SecondaryUIActionHandle.Unregister();
	}
}

void UNodecraftListItemBase::NativePreConstruct()
{
	Super::NativePreConstruct();
	
#if WITH_EDITORONLY_DATA
	// This block is for visualization at design time only. Helps with setting up the styles for different input types.
	if (bPreviewStyles && IsDesignTime())
	{
		if (Border)
		{
			if (PreviewInputType == ECommonInputType::MouseAndKeyboard)
			{
				if (bUseKeyboardAndMouseStyles)
				{
					if (bPreviewDefaultMouseAndKeyboardStyles)
					{
						bPreviewFocusedMouseAndKeyboardStyles = false;
						Border->SetStyle(BorderStyleDefault_MouseAndKeyboard);
						Border->SetPadding(BorderPaddingDefault_MouseAndKeyboard);
					}
					else if (bPreviewFocusedMouseAndKeyboardStyles)
					{
						bPreviewDefaultMouseAndKeyboardStyles = false;
						Border->SetStyle(BorderStyleFocused_MouseAndKeyboard);
						Border->SetPadding(BorderPaddingFocused_MouseAndKeyboard);
					}
					else
					{
						bPreviewDefaultMouseAndKeyboardStyles = false;
						bPreviewFocusedMouseAndKeyboardStyles = false;
						// default to previewing default styles
						Border->SetStyle(BorderStyleDefault_MouseAndKeyboard);
						Border->SetPadding(BorderPaddingDefault_MouseAndKeyboard);
					}
				}
			}
			else
			{
				if (bPreviewDefaultControllerStyles)
				{
					bPreviewFocusedControllerStyles = false;
					Border->SetStyle(BorderStyleDefault_Gamepad);
					Border->SetPadding(BorderPaddingDefault_Gamepad);
				}
				else if (bPreviewFocusedControllerStyles)
				{
					bPreviewDefaultControllerStyles = false;
					Border->SetStyle(BorderStyleFocused_Gamepad);
					Border->SetPadding(BorderPaddingFocused_Gamepad);
				}
				else
				{
					bPreviewDefaultControllerStyles = false;
					bPreviewFocusedControllerStyles = false;
					// default to previewing default styles
					Border->SetStyle(BorderStyleDefault_Gamepad);
					Border->SetPadding(BorderPaddingDefault_Gamepad);
				}
			}
		}
	}
#endif
}

void UNodecraftListItemBase::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	ON_INPUT_METHOD_CHANGED([this](ECommonInputType InputType)
	{
		ResetStyles(InputType);
	});

	// initialize styles based on current input type
	if (const ULocalPlayer* LocalPlayer = GetOwningLocalPlayer())
	{
		if (const UCommonInputSubsystem* InputSubsystem = UCommonInputSubsystem::Get(LocalPlayer))
		{
			ResetStyles(InputSubsystem->GetCurrentInputType());
		}
	}
}

FReply UNodecraftListItemBase::NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent)
{
	if (Border)
	{
		if (InFocusEvent.GetCause() == EFocusCause::Navigation || InFocusEvent.GetCause() == EFocusCause::SetDirectly)
		{
			if (const ULocalPlayer* LocalPlayer = GetOwningLocalPlayer())
			{
				if (const UCommonInputSubsystem* InputSubsystem = UCommonInputSubsystem::Get(LocalPlayer))
				{
					const bool bUsingGamepad = InputSubsystem->GetCurrentInputType() == ECommonInputType::Gamepad;
					Border->SetStyle(bUsingGamepad ? BorderStyleFocused_Gamepad
						                 : (bUseKeyboardAndMouseStyles ? BorderStyleFocused_MouseAndKeyboard : BorderStyleDefault_Gamepad));
					Border->SetPadding(bUsingGamepad ? BorderPaddingFocused_Gamepad
						                   : (bUseKeyboardAndMouseStyles ? BorderPaddingFocused_MouseAndKeyboard : BorderPaddingDefault_Gamepad));
				}
			}
		}
	}
	return Super::NativeOnFocusReceived(InGeometry, InFocusEvent);
}

void UNodecraftListItemBase::NativeOnFocusLost(const FFocusEvent& InFocusEvent)
{
	Super::NativeOnFocusLost(InFocusEvent);

	if (const ULocalPlayer* LocalPlayer = GetOwningLocalPlayer())
	{
		if (const UCommonInputSubsystem* InputSubsystem = UCommonInputSubsystem::Get(LocalPlayer))
		{
			ResetStyles(InputSubsystem->GetCurrentInputType());
		}
	}
}

void UNodecraftListItemBase::ResetStyles(ECommonInputType InputType)
{
	if (Border)
	{
		const bool bUsingGamepad = InputType == ECommonInputType::Gamepad;
		Border->SetStyle(bUsingGamepad ? BorderStyleDefault_Gamepad
			: (bUseKeyboardAndMouseStyles ? BorderStyleDefault_MouseAndKeyboard : BorderStyleDefault_Gamepad));
		Border->SetPadding(bUsingGamepad ? BorderPaddingDefault_Gamepad
			: (bUseKeyboardAndMouseStyles ? BorderPaddingDefault_MouseAndKeyboard : BorderPaddingDefault_Gamepad));
	}
}
