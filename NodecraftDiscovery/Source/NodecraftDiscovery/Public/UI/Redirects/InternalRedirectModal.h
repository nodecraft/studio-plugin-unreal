// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "Components/EditableTextBox.h"
#include "DataTypes/PlayerConnectionSubjects.h"
#include "Models/PlayerConnectionDataObject.h"
#include "UI/Alerts/AlertMessage.h"
#include "UI/Common/NodecraftLoadGuard.h"
#include "UI/Foundation/NodecraftLoadingButton.h"
#include "InternalRedirectModal.generated.h"

class UCommonBorder;
enum class ELinkType : uint8;
class UNodecraftButtonBase;
class UAsyncImage;
class UCommonTextBlock;
/**
 * 
 */
UCLASS()
class NODECRAFTDISCOVERY_API UInternalRedirectModal : public UCommonActivatableWidget
{
	GENERATED_BODY()

public:
	void Configure(FSimpleDelegate OnClosed, EPlayerConnectionSubject Subject, TOptional<FString> ServerId);

	virtual void NativeOnActivated() override;
	virtual void NativeOnDeactivated() override;

protected:
	UPROPERTY(meta=(BindWidget))
	UCommonTextBlock* InternalLinkHeader;

	UPROPERTY(meta=(BindWidget))
	UAsyncImage* QRCode;

	UPROPERTY(meta=(BindWidget))
	UNodecraftLoadGuard* LoadGuard;
	
	UPROPERTY(meta=(BindWidget))
	UNodecraftLoadingButton* SubmitButton;

	UPROPERTY(meta=(BindWidget))
	UEditableTextBox* CodeChallengeInput;
	
	UPROPERTY(meta=(BindWidget))
	UNodecraftButtonBase* CloseButtonBottom;

	UPROPERTY(meta=(BindWidget))
	UNodecraftButtonBase* OpenInBrowserButton;
	
	UPROPERTY(meta=(BindWidget))
    UPanelWidget* CodeChallengeContainer;
    
    UPROPERTY(meta=(BindWidget))
    UPanelWidget* PendingContainer;
	
	UPROPERTY(meta=(BindWidget))
	UAlertMessage* AlertMessage;
	
	bool bHasLoadedQrCode;

private:
	void RefreshUI(EPlayerConnectionStatus Status);
	void ShowErrorUI(const FText& ErrorMsg);
};
