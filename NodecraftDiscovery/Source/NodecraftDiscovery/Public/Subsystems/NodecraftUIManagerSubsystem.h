// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "Subsystems/GameInstanceSubsystem.h"
#include "Engine/GameInstance.h"
#include "Tickable.h"
#include "UI/NodecraftMenuNavInputProcessor.h"
#include "NodecraftUIManagerSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FShowNodecraftUIDelegate, UUserWidget*, Widget, bool, Animate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHideNodecraftUIDelegate, bool, Animate);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRequestScrollMenuDelegate, float, ScrollAmount);

UCLASS()
class NODECRAFTDISCOVERY_API UNodecraftUIManagerSubsystem : public UGameInstanceSubsystem, public FTickableGameObject
{
	GENERATED_BODY()

public:
	static UNodecraftUIManagerSubsystem* Get(UGameInstance* GI) { return UGameInstance::GetSubsystem<UNodecraftUIManagerSubsystem>(GI); }

	// FTickableGameObject interface
	virtual void Tick(float DeltaTime) override;
	virtual TStatId GetStatId() const override;
	// End of FTickableGameObject interface
	
	// UGameInstanceSubsystem interface
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	// End of UGameInstanceSubsystem interface
	
	// This is your interface for opening the Nodecraft UI
	UFUNCTION(BlueprintCallable, Category = "Nodecraft Subsystems|UI Manager")
	void OpenNodecraftUI(bool bAnimate);

	// This is your interface for opening the Nodecraft UI
	UFUNCTION(BlueprintCallable, Category = "Nodecraft Subsystems|UI Manager")
	void CloseNodecraftUI(bool bAnimate);

	// This delegate is used to pass the widget to be shown to your game
	UPROPERTY(BlueprintAssignable, Category = "Nodecraft Subsystems|UI Manager")
	FShowNodecraftUIDelegate ShowNodecraftUI;

	// This delegate is used to tell your game to hide the Nodecraft UI
	UPROPERTY(BlueprintAssignable, Category = "Nodecraft Subsystems|UI Manager")
	FHideNodecraftUIDelegate HideNodecraftUI;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Nodecraft Subsystems|UI Manager")
	FRequestScrollMenuDelegate OnRequestScrollPrimaryMenu;

	int32 GetNumTilesInServerList();
	

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Nodecraft Subsystems|UI Manager")
	FRequestScrollMenuDelegate OnRequestScrollSecondaryMenu;

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

	TSharedPtr<FNodecraftMenuNavInputProcessor> MenuNavInputProcessor = nullptr;
};
