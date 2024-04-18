// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "Services/ServerQueueService.h"
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
	void Configure(UServerDataObject* ServerDataObject, FSimpleDelegate InOnClosed);

protected:
	virtual void NativeConstruct() override;

	virtual void NativeOnActivated() override;
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
	
	FDelegateHandle TransitionToJoiningServerQueueHandle;

private:
	UFUNCTION()
	void OnGetServerSession(const UServerSessionDataObject* Session);

	FSimpleDelegate OnClosed;
};
