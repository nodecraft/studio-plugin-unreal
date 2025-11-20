// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ServerModerationConsolePlayerListSection.h"
#include "Common/NodecraftDelegates.h"
#include "Components/CheckBox.h"
#include "ModerationConsolePlayerList.generated.h"

class UModerationConsolePlayerSelector;

UENUM()
enum class EPlayerSelectorMode : uint8
{
	All,
	Players,
	Staff,
	Banned
};

UENUM()
enum class EModerationConsolePlayerListType : uint8
{
	None,
	Owner,
	Moderators,
	OnlinePlayers,
	OfflinePlayers,
	BannedPlayers
};

USTRUCT()
struct NODECRAFTDISCOVERY_API FModerationConsoleHeaderIcon
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category = "Nodecraft UI|Server Details|Moderation Console")
	TSoftObjectPtr<UTexture2D> Icon;

	UPROPERTY(EditAnywhere, Category = "Nodecraft UI|Server Details|Moderation Console")
	FLinearColor IconColor = FLinearColor::White;
};

UCLASS()
class NODECRAFTDISCOVERY_API UPlayerListSectionHeaderViewModel : public UObject
{
	GENERATED_BODY()

public:
	FText HeaderText;

	UPROPERTY()
	FModerationConsoleHeaderIcon IconData;
	// TSoftObjectPtr<UTexture2D> Icon;
	
};

UCLASS()
class NODECRAFTDISCOVERY_API UNoPlayersFoundItemData : public UObject
{
	GENERATED_BODY()

public:
	static UNoPlayersFoundItemData* Create(EModerationConsolePlayerListType InItemType)
	{
		UNoPlayersFoundItemData* NoPlayersFoundItemData = NewObject<UNoPlayersFoundItemData>();
		NoPlayersFoundItemData->ItemType = InItemType;
		return NoPlayersFoundItemData;
	}

	EModerationConsolePlayerListType GetListType() const { return ItemType; }
	
private:
	EModerationConsolePlayerListType ItemType = EModerationConsolePlayerListType::None;
};


UCLASS()
class NODECRAFTDISCOVERY_API UModerationConsolePlayerList : public UCommonListView
{
	GENERATED_BODY()

public:
	FSimpleBooleanDelegate AreAnyPlayersSelectedDelegate;

	FGetFocusDestination GetRightNavFocusDestination;

	virtual TSubclassOf<UUserWidget> GetDesiredEntryClassForItem(UObject* Item) const override;
	void SetFocusOnFirstPlayer();
	void SelectAllPlayers();

	void ClearSelectedPlayers();

	virtual FOnItemSelectionChanged& OnItemSelectionChanged() const override;

	void ListenForSelectedPlayersChange(FOnSelectedPlayersChanged& OnSelectedPlayersChanged);
	void SetupNavigation(const FGetFocusDestination& Delegate);

	

protected:

	virtual bool OnIsSelectableOrNavigableInternal(UObject* FirstSelectedItem) override;

	// Header section
	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Details", meta = (BindWidget))
	UCheckBox* HeaderCheckbox;
	// END Header section

	UPROPERTY(EditAnywhere, Category = "Nodecraft UI|Server Details|Moderation Console Player List")
	TSubclassOf<UUserWidget> PlayerWidgetClass;

	UPROPERTY(EditAnywhere, Category = "Nodecraft UI|Server Details|Moderation Console Player List")
	TSubclassOf<UUserWidget> NoPlayersFoundWidgetClass;

	UPROPERTY(EditAnywhere, Category = "Nodecraft UI|Server Details|Moderation Console Player List")
	TSubclassOf<UUserWidget> SectionHeaderWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Nodecraft UI|Server Details|Moderation Console Player List", meta = (RowType = "/Script/CommonUI.CommonInputActionDataBase"))
	FDataTableRowHandle SelectOrDeselectPlayerInputActionData;

	UPROPERTY(EditAnywhere, Category = "Nodecraft UI|Server Details|Moderation Console Player List")
	TMap<FString, FModerationConsoleHeaderIcon> PlayerRoleIcons;
	
	UPROPERTY()
	TArray<UPlayerServerDetailsDataObject*> SelectedPlayers;

	EPlayerSelectorMode DisplayMode;
	
	FOnNavigationDelegate HandleNavForEntryWidget;

	DECLARE_DELEGATE_OneParam(FOnFocusedPlayerChanged, UWidget* /*PlayerWidget*/);

	void UpdateActions() const;

public:
	void SetDisplayMode(EPlayerSelectorMode Mode);

	UWidget* GetFirstFocusableItem() const;
	
	FOnPlayerSelectionChanged OnPlayerSelectionChanged;

	FOnPlayerSelectedExclusive OnPlayerSelectedExclusive;

	FOnPlayerItemReceivedFocus OnPlayerItemReceivedFocus;

	void SetBannedPlayers(const TArray<UPlayerServerDetailsDataObject*>& InBannedPlayers);
	void RefreshList();
	void SetOnlinePlayers(const TArray<UPlayerServerDetailsDataObject*>& InOnlinePlayers);
	void SetOfflinePlayers(const TArray<UPlayerServerDetailsDataObject*>& InOfflinePlayers);
	void SetModerators(const TArray<UPlayerServerDetailsDataObject*>& InModerators);
	void SetOwner(UPlayerServerDetailsDataObject* InOwner);

	virtual UUserWidget& OnGenerateEntryWidgetInternal(UObject* Item, TSubclassOf<UUserWidget> DesiredEntryClass, const TSharedRef<STableViewBase>& OwnerTable) override;

private:
	void AddSectionHeader(const FText& HeaderText, const FModerationConsoleHeaderIcon& IconData);
	void AddPlayersToList(const TArray<UPlayerServerDetailsDataObject*>& Players, bool bAreChildrenSelectable, const TOptional<EModerationConsolePlayerListType>& PlayerListType = TOptional<EModerationConsolePlayerListType>());
	
	UPROPERTY()
	UPlayerServerDetailsDataObject* Owner;
	
	UPROPERTY()
	TArray<UPlayerServerDetailsDataObject*> Moderators;

	UPROPERTY()
	TArray<UPlayerServerDetailsDataObject*> OnlinePlayers;

	UPROPERTY()
	TArray<UPlayerServerDetailsDataObject*> OfflinePlayers;

	UPROPERTY()
	TArray<UPlayerServerDetailsDataObject*> BannedPlayers;
	
	mutable FUIActionBindingHandle SelectOrDeselectSelectedItemActionHandle;

};
