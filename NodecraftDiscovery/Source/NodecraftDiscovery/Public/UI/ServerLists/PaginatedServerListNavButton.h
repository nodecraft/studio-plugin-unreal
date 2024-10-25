// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ServerListViewModel.h"
#include "UI/Common/IconTextLoadingButton.h"
#include "UI/Foundation/NodecraftLoadingButton.h"
#include "PaginatedServerListNavButton.generated.h"

/**
 * 
 */
UCLASS()
class NODECRAFTDISCOVERY_API UPaginatedServerListNavButton : public UIconTextLoadingButton
{
	GENERATED_BODY()

public:
	void ConfigureForButtonState(EServerListButtonState InButtonState);
	void SetButtonState(EServerListButtonState InButtonState);

	virtual void OnInputMethodChanged(ECommonInputType CurrentInputType) override;

protected:
	EServerListButtonState ButtonState = EServerListButtonState::Default;

	ECommonInputType InputType = ECommonInputType::Gamepad;
};
