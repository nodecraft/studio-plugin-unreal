// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "ServerInvitesModal.generated.h"

class UAlertMessage;
class UFriendDataObject;
class UCommonActivatableWidgetStack;
class UServerDataObject;
class UNodecraftButtonBase;
class UCommonTextBlock;
class UCommonListView;
class UNodecraftLoadGuard;
/**
 * 
 */
UCLASS()
class NODECRAFTDISCOVERY_API UServerInvitesModal : public UCommonActivatableWidget
{
	GENERATED_BODY()

public:
	void Configure(const TArray<UServerDataObject*>& Servers, const UFriendDataObject* InInvitee, FSimpleDelegate OnPopupClosed);

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeOnActivated() override;
	virtual UWidget* NativeGetDesiredFocusTarget() const override;
	virtual bool NativeOnHandleBackAction() override;
	
	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Invites", meta=(BindWidget))
	UNodecraftLoadGuard* LoadGuard;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Invites", meta=(BindWidget))
	UCommonListView* ListView;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Invites", meta=(BindWidget))
	UCommonTextBlock* Title;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Invites", meta=(BindWidget))
	UNodecraftButtonBase* CloseButtonTop;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Invites", meta=(BindWidget))
	UNodecraftButtonBase* CloseButtonBottom;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Invites", meta=(BindWidget))
	UAlertMessage* Alert;
};
