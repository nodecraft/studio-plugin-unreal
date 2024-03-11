// Fill out your copyright notice in the Description page of Project Settings.

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

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UNodecraftLoadGuard* LoadGuard;
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UCommonListView* ListView;
};
