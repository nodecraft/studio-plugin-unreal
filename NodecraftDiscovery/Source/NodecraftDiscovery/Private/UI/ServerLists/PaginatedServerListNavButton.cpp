// Nodecraft, Inc. © 2012-2024, All Rights Reserved.


#include "UI/ServerLists/PaginatedServerListNavButton.h"

#include "CommonInputSubsystem.h"

void UPaginatedServerListNavButton::ConfigureForButtonState(EServerListButtonState InButtonState)
{
	// Should never be focusable when using gamepad
	switch (InButtonState)
	{
	case EServerListButtonState::Default:
		SetIsLoading(false);
		SetIsFocusable(InputType != ECommonInputType::Gamepad);
		break;
	case EServerListButtonState::Disabled:
		SetIsLoading(false);
		SetIsEnabled(false);
		SetIsFocusable(false);
		break;
	case EServerListButtonState::Loading:
		SetIsLoading(true);
		SetIsFocusable(false);
		break;
	}
}

void UPaginatedServerListNavButton::SetButtonState(const EServerListButtonState InButtonState)
{
	ButtonState = InButtonState;
	ConfigureForButtonState(ButtonState);
}

void UPaginatedServerListNavButton::OnInputMethodChanged(const ECommonInputType CurrentInputType)
{
	Super::OnInputMethodChanged(CurrentInputType);
	InputType = CurrentInputType;
	ConfigureForButtonState(ButtonState);
}
