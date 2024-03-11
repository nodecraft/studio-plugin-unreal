// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "ServerModerationConsolePlayerListSection.h"
#include "Components/CheckBox.h"
#include "ModerationConsolePlayerSelector.generated.h"

UENUM()
enum class EPlayerSelectorMode : uint8
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

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UServerModerationConsolePlayerListSection* OwnerSection;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UServerModerationConsolePlayerListSection* ModeratorsSection;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UServerModerationConsolePlayerListSection* OnlinePlayersSection;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UServerModerationConsolePlayerListSection* OfflinePlayersSection;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UServerModerationConsolePlayerListSection* BannedPlayersSection;

	// Header section
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UCheckBox* HeaderCheckbox;
	// END Header section
	
	UPROPERTY()
	TArray<UPlayerServerDetailsDataObject*> SelectedPlayers;

	EPlayerSelectorMode DisplayMode;


public:
	void SetOwner(UPlayerServerDetailsDataObject* Owner);
	void SetModerators(const TArray<UPlayerServerDetailsDataObject*>& Players);
	void SetOnlinePlayers(const TArray<UPlayerServerDetailsDataObject*>& Players);
	void SetOfflinePlayers(const TArray<UPlayerServerDetailsDataObject*>& Players);
	void SetBannedPlayers(const TArray<UPlayerServerDetailsDataObject*>& Players);
	void SetDisplayMode(EPlayerSelectorMode Mode);
	void SelectAllPlayers();

	UFUNCTION()
	void OnHeaderCheckboxStateChanged(bool bIsChecked);
	virtual void NativeOnInitialized() override;

	FOnSelectedPlayersChanged OnSelectedPlayersChanged;
};
