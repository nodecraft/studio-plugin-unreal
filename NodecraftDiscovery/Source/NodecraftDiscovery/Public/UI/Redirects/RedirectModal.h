// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "RedirectModal.generated.h"

class UCommonBorder;
enum class ELinkType : uint8;
class UNodecraftButtonBase;
class UAsyncImage;
class UCommonTextBlock;
/**
 * 
 */
UCLASS()
class NODECRAFTDISCOVERY_API URedirectModal : public UCommonActivatableWidget
{
	GENERATED_BODY()

public:
	void Configure(FSimpleDelegate OnClosed, FString URL, ELinkType LinkType);

protected:
	virtual void NativeOnInitialized() override;
	virtual bool NativeOnHandleBackAction() override;
	
	UPROPERTY(meta=(BindWidget))
	UCommonBorder* ExternalLinkHeader;

	UPROPERTY(meta=(BindWidget))
	UCommonTextBlock* InternalLinkHeader;
	
	UPROPERTY(meta=(BindWidget))
	UCommonTextBlock* UrlText;

	UPROPERTY(meta=(BindWidget))
	UAsyncImage* QRCode;

	UPROPERTY(meta=(BindWidget))
	UNodecraftButtonBase* CloseButtonTop;

	UPROPERTY(meta=(BindWidget))
	UNodecraftButtonBase* CloseButtonBottom;

	UPROPERTY(meta=(BindWidget))
	UNodecraftButtonBase* OpenInBrowserButton;

	UPROPERTY(EditDefaultsOnly, Category = "Nodecraft UI|Input", meta=(RowType="/Script/CommonUI.CommonInputActionDataBase"))
	FDataTableRowHandle OpenInBrowserActionData;

private:
	void UpdateActionBindings(ECommonInputType CurrentInputType);

	FUIActionBindingHandle OpenInBrowserActionHandle;
};
