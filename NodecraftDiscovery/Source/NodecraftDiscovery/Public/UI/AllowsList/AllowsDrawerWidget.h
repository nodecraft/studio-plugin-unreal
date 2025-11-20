// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "AllowsDrawerWidget.generated.h"

class UNodecraftLoadGuard;
class UCommonListView;
/**
 * 
 */
UCLASS()
class NODECRAFTDISCOVERY_API UAllowsDrawerWidget : public UCommonActivatableWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnActivated() override;
	virtual UWidget* NativeGetDesiredFocusTarget() const override;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Allows", meta=(BindWidget))
	UNodecraftLoadGuard* LoadGuard;
	
	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Allows", meta=(BindWidget))
	UCommonListView* ListView;
};
