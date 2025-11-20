// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "NodecraftButtonBase.h"
#include "UI/Common/NodecraftLoadGuard.h"
#include "NodecraftLoadingButton.generated.h"

/**
 * By default loading buttons will use a load guard, but they needn't in all cases.
 * If you want to make a loading button that uses a material animation for example
 * instead, you can subclass this and override SetIsLoading to do whatever you want.
 */
UCLASS()
class NODECRAFTDISCOVERY_API UNodecraftLoadingButton : public UNodecraftButtonBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI", meta=(BindWidgetOptional))
	UNodecraftLoadGuard* LoadGuard;

	// If you ever need to do some kind of calculation to determine whether a button should in fact be active
	// once it is done loading (or while it is loading, perhaps) rather than taking it for granted, you can override these.
	UFUNCTION()
	virtual bool ShouldActivateWhenFinishedLoading();

	UFUNCTION()
	virtual bool ShouldDeactivateWhileLoading();

public:
	virtual void SetIsLoading(bool bInIsLoading);
	
	bool GetIsLoading();

private:
	bool bIsLoading;
};
