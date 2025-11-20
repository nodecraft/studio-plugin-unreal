// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#include "UI/Common/NodecraftScrollBox.h"

#include "DeveloperSettings/NodecraftStudioInputSettings.h"
#include "Input/CommonUIActionRouterBase.h"
#include "Input/CommonUIInputTypes.h"
#include "Subsystems/NodecraftUIManagerSubsystem.h"

void UNodecraftScrollBox::SetListeningForInput(bool bShouldListen, TOptional<FText> InDisplayName)
{
	if (InDisplayName.IsSet())
	{
		DisplayName = InDisplayName.GetValue();
	}
	
	if (bShouldListen != bIsListeningForInput)
	{
		bIsListeningForInput = bShouldListen;
		UpdateBindings();
		
		if (UGameInstance* GameInstance = GetGameInstance())
		{
			UNodecraftUIManagerSubsystem* UIManagerSubsystem = GameInstance->GetSubsystem<UNodecraftUIManagerSubsystem>();
			if (UIManagerSubsystem)
			{
				switch (ScrollThumbstick)
				{
				case EScrollboxThumbstick::Left:
					{
						if (bIsListeningForInput)
						{
							UIManagerSubsystem->OnRequestScrollPrimaryMenu.AddUniqueDynamic(this, &UNodecraftScrollBox::ScrollMenu);
						}
						else
						{
							UIManagerSubsystem->OnRequestScrollPrimaryMenu.RemoveDynamic(this, &UNodecraftScrollBox::ScrollMenu);
						}
						break;
					}
				case EScrollboxThumbstick::Right:
					{
						if (bIsListeningForInput)
						{
							UIManagerSubsystem->OnRequestScrollSecondaryMenu.AddUniqueDynamic(this, &UNodecraftScrollBox::ScrollMenu);
						}
						else
						{
							UIManagerSubsystem->OnRequestScrollSecondaryMenu.RemoveDynamic(this, &UNodecraftScrollBox::ScrollMenu);
						}
						break;
					}
				default:
					break;
				}
			}
		}
	}
}

bool UNodecraftScrollBox::GetListeningForInput()
{
	return bIsListeningForInput;
}

void UNodecraftScrollBox::CheckIfScrolledToBottom(const float CurrentOffset)
{
	if (CurrentOffset >= GetScrollOffsetOfEnd())
	{
		OnScrolledToBottom.Broadcast();
	}
}

void UNodecraftScrollBox::OnWidgetRebuilt()
{
	OnUserScrolled.AddUniqueDynamic(this, &UNodecraftScrollBox::CheckIfScrolledToBottom);
}

void UNodecraftScrollBox::UpdateBindings()
{
	if (bIsListeningForInput)
	{
		FDataTableRowHandle InputActionData = ScrollInputActionDataOverride;

		if (InputActionData.IsNull())
		{
			const UNodecraftStudioInputSettings& InputSettings = UNodecraftStudioInputSettings::Get();
			InputActionData = ScrollThumbstick == EScrollboxThumbstick::Left ?
				InputSettings.GetDefaultLeftThumbstickMenuScrollInputActionData() :
				InputSettings.GetDefaultRightThumbstickMenuScrollInputActionData();
		}
		
		
		ScrollActionHandle = RegisterUIActionBinding(FBindUIActionArgs(InputActionData, bDisplayInActionBar, FSimpleDelegate::CreateWeakLambda(this, [this]()
		{
			// null op. We handle scrolling elsewhere. This is just to get the action to appear in the action bar.
		})));
	}
	else
	{
		UnregisterUIActionBinding(ScrollActionHandle);
	}
}

FUIActionBindingHandle UNodecraftScrollBox::RegisterUIActionBinding(const FBindUIActionArgs& BindActionArgs)
{
	if (UCommonUIActionRouterBase* ActionRouter = UCommonUIActionRouterBase::Get(*this))
	{
		FBindUIActionArgs FinalBindActionArgs = BindActionArgs;
		if (bDisplayInActionBar && !BindActionArgs.bDisplayInActionBar)
		{
			FinalBindActionArgs.bDisplayInActionBar = true;
		}
		
		if (DisplayName.IsEmpty() == false)
		{
			FinalBindActionArgs.OverrideDisplayName = DisplayName;
		}

		FUIActionBindingHandle BindingHandle = ActionRouter->RegisterUIActionBinding(*this, FinalBindActionArgs);
		ActionRouter->OnBoundActionsUpdated().Broadcast();

		return BindingHandle;
	}

	return FUIActionBindingHandle();
}

void UNodecraftScrollBox::UnregisterUIActionBinding(FUIActionBindingHandle& BindingHandle)
{
	BindingHandle.Unregister();
	if (UCommonUIActionRouterBase* ActionRouter = UCommonUIActionRouterBase::Get(*this))
	{
		ActionRouter->OnBoundActionsUpdated().Broadcast();
	}
}

void UNodecraftScrollBox::ScrollMenu(float ScrollAmount)
{
	if (FMath::Abs(ScrollAmount) > DeadZone)
	{
		const float CurrentOffset = GetScrollOffset();
		const float DesiredOffset = CurrentOffset + ScrollAmount * ScrollSpeed;
		const float NewOffset = FMath::Clamp(DesiredOffset, 0.0f, GetScrollOffsetOfEnd());
		SetScrollOffset(NewOffset);
		if (DesiredOffset >= GetScrollOffsetOfEnd() && CurrentOffset != NewOffset)
		{
			OnScrolledToBottom.Broadcast();
		}
	}
}
