// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "ModerationConsolePlayerList.h"
#include "ServerModerationConsolePlayerListSection.h"
#include "Components/CheckBox.h"
#include "ModerationConsolePlayerSelector.generated.h"

UENUM()
enum class EPlayerSelectorMode_DEP : uint8
{
	All,
	Players,
	Staff,
	Banned
};

/**
 * 
 */
UCLASS()
class NODECRAFTDISCOVERY_API UModerationConsolePlayerSelector : public UCommonActivatableWidget
{
	GENERATED_BODY()

	virtual UWidget* NativeGetDesiredFocusTarget() const override;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Details", meta = (BindWidget))
	UModerationConsolePlayerList* PlayersList;

	// Header section
	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Details", meta = (BindWidget))
	UCheckBox* HeaderCheckbox;
	// END Header section
	
	UPROPERTY()
	TArray<UPlayerServerDetailsDataObject*> SelectedPlayers;

	EPlayerSelectorMode DisplayMode;

	DECLARE_DELEGATE_OneParam(FOnFocusedPlayerChanged, UWidget* /*PlayerWidget*/);

public:
	void SetOwner(UPlayerServerDetailsDataObject* Owner);
	void SetModerators(const TArray<UPlayerServerDetailsDataObject*>& Players);
	void SetOnlinePlayers(const TArray<UPlayerServerDetailsDataObject*>& Players);
	void SetOfflinePlayers(const TArray<UPlayerServerDetailsDataObject*>& Players);
	void SetBannedPlayers(const TArray<UPlayerServerDetailsDataObject*>& Players);
	void SetDisplayMode(EPlayerSelectorMode Mode);
	void SelectAllPlayers();
	void ClearSelection();

	virtual FReply NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent) override;

	virtual void NativeOnActivated() override;

	virtual void NativeOnDeactivated() override;

	UFUNCTION()
	void OnHeaderCheckboxStateChanged(bool bIsChecked);
	virtual void NativeOnInitialized() override;
	void SetFocusOnFirstPlayer();
	bool HasSelectedPlayers();
	void SetupNavigation(const FGetFocusDestination& Delegate);

	UWidget* GetFirstFocusablePlayerWidget();

	FOnSelectedPlayersChanged OnSelectedPlayersChanged;
	FOnFocusedPlayerChanged OnFocusSetToPlayer;

};
