// Nodecraft, Inc. © 2012-2024, All Rights Reserved.


#include "UI/Foundation/NodecraftLoadingButton.h"

bool UNodecraftLoadingButton::ShouldDeactivateWhileLoading()
{
	return true;
}

bool UNodecraftLoadingButton::ShouldActivateWhenFinishedLoading()
{
	return true;
}

void UNodecraftLoadingButton::SetIsLoading(const bool bInIsLoading)
{
	bIsLoading = bInIsLoading;
	if (LoadGuard)
	{
		LoadGuard->SetIsLoading(bIsLoading);
	}
	
	if (bInIsLoading)
	{
		SetIsEnabled(!ShouldDeactivateWhileLoading());
	}
	else
	{
		SetIsEnabled(ShouldActivateWhenFinishedLoading());
	}
}

bool UNodecraftLoadingButton::GetIsLoading()
{
	return bIsLoading;
}
