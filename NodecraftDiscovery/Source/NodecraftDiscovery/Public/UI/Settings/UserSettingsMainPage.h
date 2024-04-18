// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "UserSettingsMainPage.generated.h"

class UNodecraftButtonBase;
class UCommonTextBlock;
class UImage;
class UAsyncImage;

DECLARE_DELEGATE_OneParam(FOnButtonClicked, int32 /* TabIndex */);

/**
 * 
 */
UCLASS()
class NODECRAFTDISCOVERY_API UUserSettingsMainPage : public UCommonActivatableWidget
{
	GENERATED_BODY()

public:
	FOnButtonClicked OnButtonClicked;
	
protected:
	virtual void NativeConstruct() override;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UAsyncImage* PlayerImage;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UImage* IdentTypeImage;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UCommonTextBlock* Username;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UNodecraftButtonBase* ChangeServerRegionButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UNodecraftButtonBase* PlayerReputationButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UNodecraftButtonBase* DataPrivacyButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UNodecraftButtonBase* LegalStuffButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UNodecraftButtonBase* ExitMultiplayerButton;
};
