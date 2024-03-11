// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "UMG/Public/Components/NamedSlot.h"
#include "NodecraftLoadGuard.generated.h"

/**
 * 
 */
UCLASS()
class NODECRAFTDISCOVERY_API UNodecraftLoadGuard : public UCommonUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UNamedSlot* ContentSlot;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UNamedSlot* LoadingSlot;

public:
	void SetIsLoading(bool bIsLoading);

	virtual void NativeConstruct() override;
};
