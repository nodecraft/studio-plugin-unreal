// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "Engine/Texture2D.h"
#include "CommonTextBlock.h"
#include "PlayerPlatformIcon.h"
#include "Common/NodecraftDelegates.h"
#include "Components/CheckBox.h"
#include "Components/Image.h"
#include "Models/PlayerServerDetails.h"
#include "UI/Common/NodecraftListItemBase.h"
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

	UPROPERTY(EditDefaultsOnly, Category = "Nodecraft UI|Styles")
	FLinearColor StatusTextColor = FLinearColor::Transparent;
	
	UPROPERTY(EditDefaultsOnly, Category = "Nodecraft UI|Styles")
	FLinearColor DateTextColor = FLinearColor::Transparent;
	
	UPROPERTY(EditDefaultsOnly, Category = "Nodecraft UI|Styles")
	FLinearColor OnlineIndicatorColor  = FLinearColor::Transparent;

	UPROPERTY(EditDefaultsOnly, Category = "Nodecraft UI|Styles")
	TSoftObjectPtr<UTexture2D> OnlineIndicatorTexture;
};

USTRUCT()
struct FPlayerListItemSelectionStyle
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "Nodecraft UI|Styles")
	TSubclassOf<UCommonBorderStyle> BorderStyle;
	
	UPROPERTY(EditDefaultsOnly, Category = "Nodecraft UI|Styles")
	FLinearColor HairlineColor = FLinearColor::Transparent;
};


/**
 * 
 */
UCLASS()
class NODECRAFTDISCOVERY_API UServerModerationConsolePlayerListItem : public UNodecraftListItemBase
{
	GENERATED_BODY()

protected:
	virtual void NativePreConstruct() override;
	
	UFUNCTION()
	void OnCheckboxStateChanged(bool bIsChecked);

	void StyleForCheckedStatus(bool bIsChecked);

	UPROPERTY(EditDefaultsOnly, Category = "Nodecraft UI|Server Details|Styles")
	FPlayerListItemSelectionStyle UnselectedStyle;

	UPROPERTY(EditDefaultsOnly, Category = "Nodecraft UI|Server Details|Styles")
	FPlayerListItemSelectionStyle SelectedStyle;

	UPROPERTY(EditDefaultsOnly, Category = "Nodecraft UI|Server Details|Styles")
	TMap<EPlayerServerStatus, FPlayerListItemStatusStyle> StatusStyles;

	void StyleOnlineStatus(EPlayerServerStatus Status);
	// IUserObjectListEntry
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
	// END IUserObjectListEntry
	
	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Details|Moderation Console", meta = (BindWidget))
	UImage* Hairline;

	// Player Section
	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Details|Moderation Console", meta = (BindWidget))
	UAsyncImage* PlayerProfileImage;
	
	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Details|Moderation Console", meta = (BindWidget))
	UCommonTextBlock* PlayerNameTextBlock;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Details|Moderation Console", meta = (BindWidget))
	UCommonTextBlock* PlayerIdTextBlock;
	
	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Details|Moderation Console", meta = (BindWidget))
	UPlayerPlatformIcon* PlatformIcon;
	// End Player Section

	// Ban Reason Section
	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Details|Moderation Console", meta = (BindWidget))
	UPanelWidget* BanReasonSection;
	
	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Details|Moderation Console", meta = (BindWidget))
	UCommonTextBlock* BanReasonTextBlock;
	// End Ban Reason Section

	// Status Section
	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Details|Moderation Console", meta = (BindWidget))
	UPanelWidget* YouIndicator;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Details|Moderation Console", meta = (BindWidget))
	UImage* OnlineIndicator;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Details|Moderation Console", meta = (BindWidget))
	UCommonTextBlock* OnlineStatusTextBlock;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Details|Moderation Console", meta = (BindWidget))
	UCommonTextBlock* OnlineStatusDateTextBlock;
	// End Status Section

	// Playtime Section
	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Details|Moderation Console", meta = (BindWidget))
	UCommonTextBlock* TotalPlaytimeTextBlock;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Details|Moderation Console", meta = (BindWidget))
	UCommonTextBlock* FirstJoinedTextBlock;
	// End Playtime Section

	// Controls
	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Details|Moderation Console", meta = (BindWidget))
	UCheckBox* Checkbox;
	// End Controls
	
	UPROPERTY()
	UPlayerServerDetailsViewModel* ViewModel;

public:
	FOnNavigationDelegate OnNavDelegate;

	void ClearSelection();
	void Deselect();
	void Select();
	bool IsSelected();
	void SetSelected(bool bSelected);

	virtual void NativeDestruct() override;
	
	virtual FReply NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent) override;

	virtual FNavigationReply NativeOnNavigation(const FGeometry& MyGeometry, const FNavigationEvent& InNavigationEvent, const FNavigationReply& InDefaultReply) override;
};
