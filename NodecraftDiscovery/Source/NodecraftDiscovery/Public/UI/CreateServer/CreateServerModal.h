// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "CreateServerModal.generated.h"

class UImage;
class UNodecraftRadioButton;
class UNodecraftButtonBase;
/**
 * 
 */
UCLASS()
class NODECRAFTDISCOVERY_API UCreateServerModal : public UCommonActivatableWidget
{
	GENERATED_BODY()

public:
	void Configure(FSimpleDelegate OnModalClosed);

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	
	UPROPERTY(meta=(BindWidget))
	UNodecraftButtonBase* PersonalServerButton;

	UPROPERTY(meta=(BindWidget))
	UNodecraftRadioButton* PersonalServerRadioButton;

	UPROPERTY(meta=(BindWidget))
	UImage* PersonalServerImage;

	UPROPERTY(meta=(BindWidget))
	UNodecraftButtonBase* GetPersonalServerButton;

	UPROPERTY(meta=(BindWidget))
	UNodecraftButtonBase* NodecraftProServerButton;

	UPROPERTY(meta=(BindWidget))
	UNodecraftRadioButton* NodecraftProRadioButton;

	UPROPERTY(meta=(BindWidget))
	UImage* NodecraftProImage;

	UPROPERTY(meta=(BindWidget))
	UNodecraftButtonBase* GetNodecraftProButton;

	UPROPERTY(meta=(BindWidget))
	UNodecraftButtonBase* CloseButton;

	UPROPERTY(meta=(BindWidget))
	UNodecraftButtonBase* CancelButton;

	UPROPERTY(meta=(BindWidget))
	UNodecraftButtonBase* NextButton;

	UPROPERTY(EditDefaultsOnly)
	FLinearColor SelectedImageTint;

	UPROPERTY(EditDefaultsOnly)
	FLinearColor UnselectedImageTint;

private:
	void SelectPersonalServer();
	void SelectNodecraftProServer();
};
