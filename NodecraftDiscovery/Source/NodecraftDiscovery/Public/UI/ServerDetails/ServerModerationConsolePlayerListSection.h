// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

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
	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Details|Moderation Console", meta = (BindWidget))
	UImage* SectionHeaderIcon;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Details|Moderation Console", meta = (BindWidget))
	UCommonTextBlock* SectionHeaderTextBlock;

	// START Players list
	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Details|Moderation Console", meta = (BindWidget))
	UPanelWidget* PlayerListContainer;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Details|Moderation Console", meta = (BindWidget))
	UCommonListView* PlayerList;
	// END Players list

	// START No Players
	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Details|Moderation Console", meta = (BindWidget))
	UPanelWidget* NoPlayersContainer;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Details|Moderation Console", meta = (BindWidget))
	UCommonTextBlock* NoPlayersTextBlock;

	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "Nodecraft UI|Server Details")
	FText NoPlayersText = FText::FromString("No players to display");
	// END No Players

	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "Nodecraft UI|Server Details")
	bool bAreChildrenSelectable = true;

	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "Nodecraft UI|Server Details")
	TSoftObjectPtr<UTexture2D> HeaderIconTexture;
	
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "Nodecraft UI|Server Details")
	FText HeaderText = FText::FromString("OWNER");

	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "Nodecraft UI|Server Details")
	FLinearColor HeaderIconTint;

public:
	virtual void NativePreConstruct() override;

	void SetPlayers(const TArray<UPlayerServerDetailsDataObject*>& Players);
	
	void ListenForSelectedPlayersChange(FOnSelectedPlayersChanged& MulticastDelegate);
	void SelectAllPlayers();
	void ClearSelection();

	FOnPlayerSelectionChanged OnPlayerSelectionChanged;

	FOnPlayerSelectedExclusive OnPlayerSelectedExclusive;

	FOnPlayerItemReceivedFocus OnPlayerItemReceivedFocus;
};
