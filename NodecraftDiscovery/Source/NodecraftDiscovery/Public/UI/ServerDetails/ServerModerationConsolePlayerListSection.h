// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonListView.h"
#include "CommonTextBlock.h"
#include "CommonUserWidget.h"
#include "Components/Image.h"
#include "Models/PlayerServerDetails.h"
#include "ServerModerationConsolePlayerListSection.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnSelectedPlayersChanged, const TArray<UPlayerServerDetailsDataObject*>& /*SelectedPlayers*/);

UCLASS()
class NODECRAFTDISCOVERY_API UServerModerationConsolePlayerListSection : public UCommonUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UImage* SectionHeaderIcon;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UCommonTextBlock* SectionHeaderTextBlock;

	// START Players list
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UPanelWidget* PlayerListContainer;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UCommonListView* PlayerList;
	// END Players list

	// START No Players
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UPanelWidget* NoPlayersContainer;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UCommonTextBlock* NoPlayersTextBlock;

	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "Player List Section")
	FText NoPlayersText = FText::FromString("No players to display");
	// END No Players

	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "Player List Section")
	bool bAreChildrenSelectable = true;

	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "Player List Section")
	TSoftObjectPtr<UTexture2D> HeaderIconTexture;
	
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "Player List Section")
	FText HeaderText = FText::FromString("OWNER");

	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "Player List Section")
	FLinearColor HeaderIconTint;

public:
	virtual void NativePreConstruct() override;

	void SetPlayers(const TArray<UPlayerServerDetailsDataObject*>& Players);
	
	void ListenForSelectedPlayersChange(FOnSelectedPlayersChanged& MulticastDelegate);
	void SelectAllPlayers();

	FOnPlayerSelectionChanged OnPlayerSelectionChanged;

	FOnPlayerSelectedExclusive OnPlayerSelectedExclusive;
};
