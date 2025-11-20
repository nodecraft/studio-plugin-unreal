// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "Components/NamedSlot.h"
#include "NodecraftLoadGuard.generated.h"

/**
 * 
 */
UCLASS()
class NODECRAFTDISCOVERY_API UNodecraftLoadGuard : public UCommonUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI", meta = (BindWidget))
	UNamedSlot* ContentSlot;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI", meta = (BindWidget))
	UNamedSlot* LoadingSlot;

public:
	void SetIsLoading(bool bIsLoading);

	virtual void NativeConstruct() override;
};
