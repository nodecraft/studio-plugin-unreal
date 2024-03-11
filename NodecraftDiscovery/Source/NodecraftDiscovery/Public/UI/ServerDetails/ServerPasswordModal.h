// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "ServerPasswordModal.generated.h"

class UServerSessionDataObject;
class UAlertMessage;
class UServerDataObject;
class UNodecraftButtonBase;
class UEditableTextBox;
/**
 * 
 */
UCLASS()
class NODECRAFTDISCOVERY_API UServerPasswordModal : public UCommonActivatableWidget
{
	GENERATED_BODY()

public:
	void Configure(const UServerDataObject* ServerDataObject, FSimpleDelegate InOnClosed);

protected:
	virtual void NativeConstruct() override;

	virtual void NativeOnDeactivated() override;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UEditableTextBox* InputBox;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UNodecraftButtonBase* ShowHidePasswordButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UNodecraftButtonBase* CancelButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UNodecraftButtonBase* JoinButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UAlertMessage* AlertMessage;

private:
	UFUNCTION()
	void OnGetServerSession(const UServerSessionDataObject* Session);

	FSimpleDelegate OnClosed;
};
