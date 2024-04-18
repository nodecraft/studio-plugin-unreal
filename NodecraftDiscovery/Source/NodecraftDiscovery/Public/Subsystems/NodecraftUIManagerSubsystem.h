// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "NodecraftUIManagerSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FShowNodecraftUIDelegate, UUserWidget*, Widget, bool, Animate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHideNodecraftUIDelegate, bool, Animate);

UCLASS()
class NODECRAFTDISCOVERY_API UNodecraftUIManagerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	static UNodecraftUIManagerSubsystem* Get(UGameInstance* GI) { return UGameInstance::GetSubsystem<UNodecraftUIManagerSubsystem>(GI); }

	// UGameInstanceSubsystem interface
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	// End of UGameInstanceSubsystem interface
	
	// This is your interface for opening the Nodecraft UI
	UFUNCTION(BlueprintCallable, Category = "Nodecraft UI")
	void OpenNodecraftUI(bool bAnimate);

	// This is your interface for opening the Nodecraft UI
	UFUNCTION(BlueprintCallable, Category = "Nodecraft UI")
	void CloseNodecraftUI(bool bAnimate);

	// This delegate is used to pass the widget to be shown to your game
	UPROPERTY(BlueprintAssignable, Category = "Nodecraft UI")
	FShowNodecraftUIDelegate ShowNodecraftUI;

	// This delegate is used to tell your game to hide the Nodecraft UI
	UPROPERTY(BlueprintAssignable, Category = "Nodecraft UI")
	FHideNodecraftUIDelegate HideNodecraftUI;

private:
	bool bIsShowingUI;

	UFUNCTION()
	void OnNodecraftUIHidden_Internal(UUserWidget* Widget, bool bAnimate);

	UFUNCTION()
	void OnNodecraftUIShown_Internal(UUserWidget* Widget, bool bAnimate);

	UPROPERTY()
	UUserWidget* MainMenu;

	UPROPERTY()
	UUserWidget* LoginScreen;
};
