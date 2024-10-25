// Nodecraft, Inc. © 2012-2024, All Rights Reserved.


#include "UI/Common/NodecraftRadioButton.h"

#include "CommonBorder.h"
#include "CommonInputSubsystem.h"
#include "CommonTextBlock.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/Image.h"
#include "Components/Overlay.h"
#include "Input/CommonUIInputTypes.h"
#include "Subsystems/AssetStreamerSubsystem.h"
#include "UI/Foundation/NodecraftButtonBase.h"
#include "Utility/NodecraftMacros.h"

void UNodecraftRadioButton::SetIsChecked(bool bIsChecked)
{
	Button->SetIsSelected(bIsChecked);
	ButtonStateBackground->SetVisibility(bIsChecked ? ESlateVisibility::Collapsed : ESlateVisibility::HitTestInvisible);
	ButtonStateImage->SetVisibility(bIsChecked ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
	ButtonStateImage->SetColorAndOpacity(bIsChecked ? Style.CheckedTint : Style.UncheckedTint);
	UpdateTextStyles(bIsChecked);
	UpdateBorderStyles(bIsChecked);
}

bool UNodecraftRadioButton::GetIsChecked() const
{
	return Button->GetSelected();
}

void UNodecraftRadioButton::SetPrimaryLabelText(const FText& InText)
{
	Descriptor.PrimaryLabel = InText;
	PrimaryLabel->SetText(InText);
	PrimaryLabel->SetVisibility(InText.IsEmpty() ? ESlateVisibility::Collapsed : ESlateVisibility::SelfHitTestInvisible);
}

void UNodecraftRadioButton::SetSecondaryLabelText(const FText& InText)
{
	Descriptor.SecondaryLabel = InText;
	SecondaryLabel->SetText(InText);
	SecondaryLabel->SetVisibility(InText.IsEmpty() ? ESlateVisibility::Collapsed : ESlateVisibility::SelfHitTestInvisible);
}

void UNodecraftRadioButton::SetTertiaryLabelText(const FText& InText)
{
	Descriptor.TertiaryLabel = InText;
	TertiaryLabel->SetText(InText);
	TertiaryLabel->SetVisibility(InText.IsEmpty() ? ESlateVisibility::Collapsed : ESlateVisibility::SelfHitTestInvisible);
}

void UNodecraftRadioButton::SetTextStyles(const FRadioButtonTextStyle& InPrimaryTextStyle,
	const FRadioButtonTextStyle& InSecondaryTextStyle, const FRadioButtonTextStyle& InTertiaryTextStyle)
{
	PrimaryTextStyle = InPrimaryTextStyle;
	SecondaryTextStyle = InSecondaryTextStyle;
	TertiaryTextStyle = InTertiaryTextStyle;
	UpdateTextStyles(GetIsChecked());
}

void UNodecraftRadioButton::SetBorderStyles(TSubclassOf<UCommonBorderStyle> InBorderStyleDefault,
	TSubclassOf<UCommonBorderStyle> InBorderStyleFocused)
{
	Style.ButtonBorderStyleDefault = InBorderStyleDefault;
	Style.ButtonBorderStyleFocused = InBorderStyleFocused;
	UpdateBorderStyles(HasUserFocus(GetOwningPlayer()));
}

void UNodecraftRadioButton::SetBorderPadding(const FMargin& InBorderPaddingDefault,
	const FMargin& InBorderPaddingFocused)
{
	Style.BorderPaddingDefault = InBorderPaddingDefault;
	Style.BorderPaddingFocused = InBorderPaddingFocused;
	UpdateBorderStyles(HasUserFocus(GetOwningPlayer()));
}

void UNodecraftRadioButton::SetTriggeringInputAction(const FDataTableRowHandle& InInputActionData, TOptional<FText> InInputActionNameOverride)
{
	ButtonTriggeringInputAction = InInputActionData;
	if (InInputActionNameOverride.IsSet())
	{
		TriggeringActionNameOverride = InInputActionNameOverride.GetValue();
	}
}

void UNodecraftRadioButton::SetupVisuals()
{
	ButtonStateImage->SetBrush(Style.CheckedImage.HasUObject() ? Style.CheckedImage : FSlateBrush());
	ButtonStateBackground->SetBrush(Style.BackgroundImage.HasUObject() ? Style.BackgroundImage : FSlateBrush());
	ButtonStateImage->SetColorAndOpacity(Style.UncheckedTint);
	if (UHorizontalBoxSlot* HBoxSlot = Cast<UHorizontalBoxSlot>(RadioContainer->Slot))
	{
		HBoxSlot->SetPadding(Style.RadioPadding);
	}
	if (Border)
	{
		Border->SetStyle(Style.ButtonBorderStyleDefault);
		Border->SetPadding(Style.BorderPaddingDefault);
	}
}

void UNodecraftRadioButton::UpdateTextStyles(bool bIsChecked)
{
	if (bIsChecked)
	{
		if (PrimaryLabel && PrimaryTextStyle.IsValid())
		{
			UAssetStreamerSubsystem::Get().LoadAssetAsync(PrimaryTextStyle.SelectedTextStyle.ToSoftObjectPath(),
				FStreamableDelegate::CreateWeakLambda(this, [this]
			{
				PrimaryLabel->SetStyle(PrimaryTextStyle.SelectedTextStyle.Get());
			}));
		}
		if (SecondaryLabel && SecondaryTextStyle.IsValid())
		{
			UAssetStreamerSubsystem::Get().LoadAssetAsync(SecondaryTextStyle.SelectedTextStyle.ToSoftObjectPath(),
				FStreamableDelegate::CreateWeakLambda(this, [this]
			{
				SecondaryLabel->SetStyle(SecondaryTextStyle.SelectedTextStyle.Get());
			}));
		}
		if (TertiaryLabel && TertiaryTextStyle.IsValid())
		{
			UAssetStreamerSubsystem::Get().LoadAssetAsync(TertiaryTextStyle.SelectedTextStyle.ToSoftObjectPath(),
				FStreamableDelegate::CreateWeakLambda(this, [this]
			{
				TertiaryLabel->SetStyle(TertiaryTextStyle.SelectedTextStyle.Get());
			}));
		}
	}
	else
	{
		if (PrimaryLabel && PrimaryTextStyle.IsValid())
		{
			UAssetStreamerSubsystem::Get().LoadAssetAsync(PrimaryTextStyle.TextStyle.ToSoftObjectPath(),
				FStreamableDelegate::CreateWeakLambda(this, [this]
			{
				PrimaryLabel->SetStyle(PrimaryTextStyle.TextStyle.Get());
			}));
		}
		if (SecondaryLabel && SecondaryTextStyle.IsValid())
		{
			UAssetStreamerSubsystem::Get().LoadAssetAsync(SecondaryTextStyle.TextStyle.ToSoftObjectPath(),
				FStreamableDelegate::CreateWeakLambda(this, [this]
			{
				SecondaryLabel->SetStyle(SecondaryTextStyle.TextStyle.Get());
			}));
		}
		if (TertiaryLabel && TertiaryTextStyle.IsValid())
		{
			UAssetStreamerSubsystem::Get().LoadAssetAsync(TertiaryTextStyle.TextStyle.ToSoftObjectPath(),
				FStreamableDelegate::CreateWeakLambda(this, [this]
			{
				TertiaryLabel->SetStyle(TertiaryTextStyle.TextStyle.Get());
			}));
		}
	}
}

void UNodecraftRadioButton::UpdateBorderStyles(bool bIsFocused)
{
	if (Border)
	{
		Border->SetStyle(bIsFocused ? Style.ButtonBorderStyleFocused : Style.ButtonBorderStyleDefault);
		Border->SetPadding(bIsFocused ? Style.BorderPaddingFocused : Style.BorderPaddingDefault);
	}
}

void UNodecraftRadioButton::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	SetupVisuals();

	if (bOverrideLabels)
	{
		SetPrimaryLabelText(Descriptor.PrimaryLabel);
		SetSecondaryLabelText(Descriptor.SecondaryLabel);
		SetTertiaryLabelText(Descriptor.TertiaryLabel);
	}
}

void UNodecraftRadioButton::Configure(const FNodecraftRadioButtonConfiguration& RadioButtonDescriptor, int32 InButtonIndex)
{
	ButtonIndex = InButtonIndex;
	Descriptor = RadioButtonDescriptor;
	
	if (RadioButtonDescriptor.PrimaryLabel.IsEmpty() == false)
	{
		PrimaryLabel->SetText(RadioButtonDescriptor.PrimaryLabel);
		PrimaryLabel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else
	{
		PrimaryLabel->SetVisibility(ESlateVisibility::Collapsed);
	}
	if (RadioButtonDescriptor.SecondaryLabel.IsEmpty() == false)
	{
		SecondaryLabel->SetText(RadioButtonDescriptor.SecondaryLabel);
		SecondaryLabel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else
	{
		SecondaryLabel->SetVisibility(ESlateVisibility::Collapsed);
	}
	if (RadioButtonDescriptor.TertiaryLabel.IsEmpty() == false)
	{
		TertiaryLabel->SetText(RadioButtonDescriptor.TertiaryLabel);
		TertiaryLabel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else
	{
		TertiaryLabel->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UNodecraftRadioButton::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	Button->SetIsSelectable(true);
	Button->SetIsToggleable(false);

	Button->OnClicked().Clear();
	Button->OnClicked().AddWeakLambda(this, [this]()
	{
		OnRadioButtonSelected.Broadcast(ButtonIndex);
	});	
	Button->OnFocusReceived().AddWeakLambda(this, [this]()
	{
		UpdateBorderStyles(true);
		if (ButtonTriggeringInputAction.IsNull() == false)
		{
			FBindUIActionArgs UIActionArgs = FBindUIActionArgs(ButtonTriggeringInputAction, true,
			                                                   FSimpleDelegate::CreateWeakLambda(this, [this]
			                                                   {
				                                                   Button->OnClicked().Broadcast();
			                                                   }));
			if (TriggeringActionNameOverride.IsSet())
			{
				UIActionArgs.OverrideDisplayName = TriggeringActionNameOverride.GetValue();
			}
			InputActionHandle = RegisterUIActionBinding(UIActionArgs);
		}
	});
	Button->OnFocusLost().AddWeakLambda(this, [this]()
	{
		UpdateBorderStyles(false);
		if (InputActionHandle.IsValid())
		{
			InputActionHandle.Unregister();
		}
	});

	ON_INPUT_METHOD_CHANGED([this](ECommonInputType InputType)
	{
		if (InputType == ECommonInputType::MouseAndKeyboard)
		{
			UpdateBorderStyles(false);
		}
	});
}

FNavigationReply UNodecraftRadioButton::NativeOnNavigation(const FGeometry& MyGeometry,
	const FNavigationEvent& InNavigationEvent, const FNavigationReply& InDefaultReply)
{
	if (OnNavigation.IsBound())
	{
		return OnNavigation.Execute(MyGeometry, InNavigationEvent, InDefaultReply);
	}
	return Super::NativeOnNavigation(MyGeometry, InNavigationEvent, InDefaultReply);
}

FReply UNodecraftRadioButton::NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent)
{
	if (InFocusEvent.GetCause() == EFocusCause::Navigation || InFocusEvent.GetCause() == EFocusCause::SetDirectly)
	{
		if (const ULocalPlayer* LocalPlayer = GetOwningLocalPlayer())
		{
			if (UCommonInputSubsystem::Get(LocalPlayer)->GetCurrentInputType() == ECommonInputType::Gamepad)
			{
				Button->SetFocus();
			}
		}
	}
	return Super::NativeOnFocusReceived(InGeometry, InFocusEvent);
}

void UNodecraftRadioButton::NativeOnFocusLost(const FFocusEvent& InFocusEvent)
{
	Super::NativeOnFocusLost(InFocusEvent);
	
	if (InFocusEvent.GetCause() == EFocusCause::Navigation || InFocusEvent.GetCause() == EFocusCause::SetDirectly)
	{
		UpdateBorderStyles(false);
	}
}
