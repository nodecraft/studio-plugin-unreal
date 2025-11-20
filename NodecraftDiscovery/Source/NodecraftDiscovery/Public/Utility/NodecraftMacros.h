// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#define ON_INPUT_METHOD_CHANGED(Func) \
	if (ULocalPlayer* LocalPlayer = GetOwningLocalPlayer()) \
	{ \
		Func(UCommonInputSubsystem::Get(LocalPlayer)->GetCurrentInputType()); \
		UCommonInputSubsystem::Get(LocalPlayer)->OnInputMethodChangedNative.AddWeakLambda( \
			this, [this](ECommonInputType InputType) \
		{ \
			Func(InputType); \
		}); \
	}
