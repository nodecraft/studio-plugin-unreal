// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "Blueprint/UserWidget.h"
#include "NodecraftStudioWidgetSettings.generated.h"

class UCommonActivatableWidget;
/**
 * 
 */
UCLASS(Config=Nodecraft, DefaultConfig)
class NODECRAFTDISCOVERY_API UNodecraftStudioWidgetSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	static const UNodecraftStudioWidgetSettings& Get() {
		return *GetDefault<UNodecraftStudioWidgetSettings>();
	}

	virtual FName GetCategoryName() const override;

	TSoftClassPtr<UCommonActivatableWidget> GetJoiningServerQueueModal() const;

	TSoftClassPtr<UCommonActivatableWidget> GetServerPasswordModal() const;
	TSoftClassPtr<UUserWidget> GetMainMenuWidget() const;
	TSoftClassPtr<UUserWidget> GetLoginScreenWidget() const;
	TSoftClassPtr<UUserWidget> GetInternalRedirectModal() const;
	TSoftClassPtr<UUserWidget> GetExternalRedirectModal() const;

protected:
	// TODO: move queue and password modals to the main menu
	UPROPERTY(EditAnywhere, Category = "Nodecraft Settings", Config)
	TSoftClassPtr<UCommonActivatableWidget> JoiningServerQueueModal;

	UPROPERTY(EditAnywhere, Category = "Nodecraft Settings", Config)
	TSoftClassPtr<UCommonActivatableWidget> ServerPasswordModal;

	UPROPERTY(EditAnywhere, Category = "Nodecraft Settings", Config)
	TSoftClassPtr<UUserWidget> MainMenuWidget;

	UPROPERTY(EditAnywhere, Category = "Nodecraft Settings", Config)
	TSoftClassPtr<UUserWidget> LoginScreenWidget;

	UPROPERTY(EditAnywhere, Category = "Nodecraft Settings", Config)
	TSoftClassPtr<UUserWidget> InternalRedirectModal;

	UPROPERTY(EditAnywhere, Category = "Nodecraft Settings", Config)
	TSoftClassPtr<UUserWidget> ExternalRedirectModal;
};
