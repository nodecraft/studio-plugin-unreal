// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "CommonTextBlock.h"
#include "PlayerPlatformIcon.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "Components/Border.h"
#include "Components/CheckBox.h"
#include "Components/Image.h"
#include "Models/PlayerServerDetails.h"
#include "UI/Foundation/NodecraftButtonBase.h"
#include "ServerModerationConsolePlayerListItem.generated.h"

class UAsyncImage;

UENUM()
enum class EPlayerServerStatus : uint8
{
	Online,
	Offline,
	Banned
};

USTRUCT()
struct FPlayerListItemStatusStyle
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	FLinearColor StatusTextColor = FLinearColor::Transparent;
	
	UPROPERTY(EditDefaultsOnly)
	FLinearColor DateTextColor = FLinearColor::Transparent;
	
	UPROPERTY(EditDefaultsOnly)
	FLinearColor OnlineIndicatorColor  = FLinearColor::Transparent;

	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UTexture2D> OnlineIndicatorTexture;
};

USTRUCT()
struct FPlayerListItemSelectionStyle
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	FLinearColor BackgroundColor = FLinearColor::Transparent;
	
	UPROPERTY(EditDefaultsOnly)
	FLinearColor HairlineColor = FLinearColor::Transparent;
};


/**
 * 
 */
UCLASS()
class NODECRAFTDISCOVERY_API UServerModerationConsolePlayerListItem : public UCommonActivatableWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

protected:
	UFUNCTION()
	void OnCheckboxStateChanged(bool bIsChecked);

	void StyleForCheckedStatus(bool bIsChecked);

	UPROPERTY(EditDefaultsOnly, Category = "Player List Item")
	FPlayerListItemSelectionStyle UnselectedStyle;

	UPROPERTY(EditDefaultsOnly, Category = "Player List Item")
	FPlayerListItemSelectionStyle SelectedStyle;

	UPROPERTY(EditDefaultsOnly, Category = "Player List Item")
	TMap<EPlayerServerStatus, FPlayerListItemStatusStyle> StatusStyles;

	void StyleOnlineStatus(EPlayerServerStatus Status);
	// IUserObjectListEntry
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
	// END IUserObjectListEntry

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UBorder* Border;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UImage* Hairline;

	// Player Section
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UAsyncImage* PlayerProfileImage;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UCommonTextBlock* PlayerNameTextBlock;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UCommonTextBlock* PlayerIdTextBlock;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UPlayerPlatformIcon* PlatformIcon;
	// End Player Section

	// Ban Reason Section
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UPanelWidget* BanReasonSection;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UCommonTextBlock* BanReasonTextBlock;
	// End Ban Reason Section

	// Status Section
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UPanelWidget* YouIndicator;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UImage* OnlineIndicator;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UCommonTextBlock* OnlineStatusTextBlock;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UCommonTextBlock* OnlineStatusDateTextBlock;
	// End Status Section

	// Playtime Section
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UCommonTextBlock* TotalPlaytimeTextBlock;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UCommonTextBlock* FirstJoinedTextBlock;
	// End Playtime Section

	// Controls
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UCheckBox* Checkbox;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UNodecraftButtonBase* SelectButton;
	// End Controls
	
	UPROPERTY()
	UPlayerServerDetailsViewModel* ViewModel;

public:
	void ClearSelection();
	void Select();
	void SetSelected(bool bSelected);

	void NativeDestruct() override;
};
