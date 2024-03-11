// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonLoadGuard.h"
#include "CommonTextBlock.h"
#include "CommonUserWidget.h"
#include "ServerDetailsSection.h"
#include "Models/ServerDataObject.h"
#include "UObject/Object.h"
#include "ServerDetailsOverviewSection.generated.h"

/**
 * 
 */
UCLASS()
class NODECRAFTDISCOVERY_API UServerDetailsOverviewSection : public UCommonUserWidget, public IServerDetailsSection
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UCommonLoadGuard* LoadGuard;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UCommonTextBlock* OverviewText;
	
public:
	virtual void SetServerData(UServerDataObject* InServerDataObject) override;
	void SetLoading(bool bLoading);

	virtual void NativeOnInitialized() override;
};
