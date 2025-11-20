// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "UserSettingsAccountReputation.generated.h"

class UVerticalBox;
class UCommonBorder;
class UCommonListView;
class UNodecraftLoadGuard;
/**
 * 
 */
UCLASS()
class NODECRAFTDISCOVERY_API UUserSettingsAccountReputation : public UCommonActivatableWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnActivated() override;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Account Reputation", meta = (BindWidget))
	UNodecraftLoadGuard* LoadGuard;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Account Reputation", meta = (BindWidget))
	UCommonListView* ListView;
	
	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Account Reputation", meta = (BindWidget))
	UVerticalBox* BansContent;
	
	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Account Reputation", meta = (BindWidget))
	UCommonBorder* NoBansContent;
};
