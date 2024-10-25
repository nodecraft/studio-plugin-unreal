// Nodecraft, Inc. © 2012-2024, All Rights Reserved.


#include "UI/NodecraftMenuNavInputProcessor.h"
#include "InputCoreTypes.h"
#include "Input/Events.h"
#include "Widgets/SWidget.h"
#include "Layout/ArrangedChildren.h"
#include "Layout/WidgetPath.h"
#include "Framework/Application/SlateApplication.h"
#include "Framework/Application/SlateUser.h"

FNodecraftMenuNavInputProcessor::FNodecraftMenuNavInputProcessor()
	: DeadZone(0.1f)
{
	ClearAnalogValues();
}

void FNodecraftMenuNavInputProcessor::Tick(const float DeltaTime, FSlateApplication& SlateApp, TSharedRef<ICursor>)
{
}

bool FNodecraftMenuNavInputProcessor::IsRelevantInput(const FInputEvent& InputEvent) const
{
	return GetOwnerUserIndex() == InputEvent.GetUserIndex();
}

bool FNodecraftMenuNavInputProcessor::IsRelevantInput(const FKeyEvent& KeyEvent) const
{
	return false;
}

bool FNodecraftMenuNavInputProcessor::IsRelevantInput(const FAnalogInputEvent& AnalogInputEvent) const
{
	FKey Key = AnalogInputEvent.GetKey();
	return Key == EKeys::Gamepad_LeftX || Key == EKeys::Gamepad_LeftY || Key == EKeys::Gamepad_RightX || Key == EKeys::Gamepad_RightY;
}

bool FNodecraftMenuNavInputProcessor::IsRelevantInput(const FPointerEvent& MouseEvent) const
{
	return false;
}

bool FNodecraftMenuNavInputProcessor::HandleKeyDownEvent(FSlateApplication& SlateApp, const FKeyEvent& InKeyEvent)
{
	return IInputProcessor::HandleKeyDownEvent(SlateApp, InKeyEvent);
}

bool FNodecraftMenuNavInputProcessor::HandleKeyUpEvent(FSlateApplication& SlateApp, const FKeyEvent& InKeyEvent)
{
	return IInputProcessor::HandleKeyUpEvent(SlateApp, InKeyEvent);
}

bool FNodecraftMenuNavInputProcessor::HandleAnalogInputEvent(FSlateApplication& SlateApp, const FAnalogInputEvent& InAnalogInputEvent)
{
	if (IsRelevantInput(InAnalogInputEvent))
	{
		FKey Key = InAnalogInputEvent.GetKey();
		float AnalogValue = InAnalogInputEvent.GetAnalogValue();

		if (Key == EKeys::Gamepad_LeftX)
		{
			FVector2D& Value = GetAnalogValue(EAnalogStick::Left);
			Value.X = AnalogValue;
		}
		else if (Key == EKeys::Gamepad_LeftY)
		{
			FVector2D& Value = GetAnalogValue(EAnalogStick::Left);
			Value.Y = -AnalogValue;
		}
		else if (Key == EKeys::Gamepad_RightX)
		{
			FVector2D& Value = GetAnalogValue(EAnalogStick::Right);
			Value.X = AnalogValue;
		}
		else if (Key == EKeys::Gamepad_RightY)
		{
			FVector2D& Value = GetAnalogValue(EAnalogStick::Right);
			Value.Y = -AnalogValue;
		}
		else
		{
			return false;
		}
	}
	// Don't swallow input
	return false;
}

bool FNodecraftMenuNavInputProcessor::HandleMouseMoveEvent(FSlateApplication& SlateApp, const FPointerEvent& MouseEvent)
{
	return IInputProcessor::HandleMouseMoveEvent(SlateApp, MouseEvent);
}

void FNodecraftMenuNavInputProcessor::ClearAnalogValues()
{
	AnalogValues[static_cast<uint8>(EAnalogStick::Left)] = FVector2D::ZeroVector;
	AnalogValues[static_cast<uint8>(EAnalogStick::Right)] = FVector2D::ZeroVector;
}
