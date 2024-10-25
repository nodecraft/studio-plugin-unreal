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
	ResetBorderStyle();
}

void UNodecraftListItemBase::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	ON_INPUT_METHOD_CHANGED([this](ECommonInputType InputType)
	{
		if (InputType == ECommonInputType::MouseAndKeyboard)
		{
			ResetBorderStyle();
		}
	});
}

FReply UNodecraftListItemBase::NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent)
{
	if (Border)
	{
		if (InFocusEvent.GetCause() == EFocusCause::Navigation || InFocusEvent.GetCause() == EFocusCause::SetDirectly)
		{
			if (const ULocalPlayer* LocalPlayer = GetOwningLocalPlayer())
			{
				const bool bUsingGamepad = UCommonInputSubsystem::Get(LocalPlayer)->GetCurrentInputType() == ECommonInputType::Gamepad;
				Border->SetStyle(bUsingGamepad ? BorderStyleFocused : BorderStyleDefault);
				Border->SetPadding(bUsingGamepad ? BorderPaddingFocused : BorderPaddingDefault);
			}
		}
	}
	return Super::NativeOnFocusReceived(InGeometry, InFocusEvent);
}

void UNodecraftListItemBase::NativeOnFocusLost(const FFocusEvent& InFocusEvent)
{
	Super::NativeOnFocusLost(InFocusEvent);
    ResetBorderStyle();
}

void UNodecraftListItemBase::ResetBorderStyle()
{
	if (Border)
	{
		Border->SetStyle(BorderStyleDefault);
		Border->SetPadding(BorderPaddingDefault);
	}
}
