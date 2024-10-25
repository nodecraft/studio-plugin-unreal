// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/Texture2D.h"
#include "CommonBorder.h"
#include "CommonButtonBase.h"
#include "CommonRichTextBlock.h"
#include "CommonTextBlock.h"
#include "Components/SizeBox.h"
#include "Models/NotificationDataObject.h"
#include "UI/Common/AsyncImage.h"
#include "UI/Common/NodecraftListItemBase.h"
#include "UI/Common/NodecraftLoadGuard.h"

#include "NotificationListItem.generated.h"

USTRUCT(BlueprintType)
struct FNotificationListItemConfig
{
	GENERATED_BODY()

	FNotificationListItemConfig() = default;

	UPROPERTY(EditAnywhere, Category = "Nodecraft UI|Notification List Item Config")
	FLinearColor ContentAreaBackgroundColor = FLinearColor::Transparent;

	UPROPERTY(EditAnywhere, Category = "Nodecraft UI|Notification List Item Config")
	TSubclassOf<UCommonBorderStyle> BorderStyleDefault;

	UPROPERTY(EditAnywhere, Category = "Nodecraft UI|Notification List Item Config")
	TSubclassOf<UCommonBorderStyle> BorderStyleFocused;

	UPROPERTY(EditAnywhere, Category = "Nodecraft UI|Notification List Item Config")
	TSoftObjectPtr<UTexture2D> NotifIcon;

	UPROPERTY(EditAnywhere, Category = "Nodecraft UI|Notification List Item Config")
	TSoftObjectPtr<UTexture2D> EventTextIcon;

	UPROPERTY(EditAnywhere, Category = "Nodecraft UI|Notification List Item Config")
	FLinearColor EventTextIconColor = FLinearColor::Transparent;

	UPROPERTY(EditAnywhere, Category = "Nodecraft UI|Notification List Item Config")
	FLinearColor EventTextColor = FLinearColor::Transparent;

	UPROPERTY(EditAnywhere, Category = "Nodecraft UI|Notification List Item Config")
	TSoftClassPtr<UCommonButtonStyle> PrimaryButtonStyle;

	UPROPERTY(EditAnywhere, Category = "Nodecraft UI|Notification List Item Config")
	FLinearColor PrimaryButtonBorderColor = FLinearColor::Transparent;

	UPROPERTY(EditAnywhere, Category = "Nodecraft UI|Notification List Item Config")
	FLinearColor PrimaryButtonColorLeft = FLinearColor::Transparent;

	// dlohnes 01/29/24: Currently unused as we are just applying a flat color, but we want to use a material that applies a gradient
	UPROPERTY(EditAnywhere, Category = "Nodecraft UI|Notification List Item Config")
	FLinearColor PrimaryButtonColorRight = FLinearColor::Transparent;
};


/**
 * 
 */
UCLASS()
class NODECRAFTDISCOVERY_API UNotificationListItem : public UNodecraftListItemBase
{
	GENERATED_BODY()

	enum ENotifListItemSize
	{
		Small,
		Large
	};

private:
	void ConfigForKickNotif(const UNotificationDataObject* NotificationDataObject);
	void ConfigForBanNotif(const UNotificationDataObject* NotificationDataObject);
	void ConfigForAllowNotif(const UNotificationDataObject* NotificationDataObject);
	void ConfigForSessionNotif(const UNotificationDataObject* NotificationDataObject);
	void ConfigForInviteNotif(const UNotificationDataObject* NotificationDataObject);
	void ConfigForModerationNotif(const UNotificationDataObject* NotificationDataObject);
	void UpdateTimestamp(const FDateTime& Time);

	void UpdateItemSize(ENotifListItemSize Size);

protected:
	// IUserObjectListEntry
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
	// IUserObjectListEntry

	virtual FReply NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent) override;
	virtual void NativeOnFocusLost(const FFocusEvent& InFocusEvent) override;
	
protected:
	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Notification List Item", meta=(BindWidget))
	UBorder* ContentBorder;

	// Controls the height of the entire notif with height override
	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Notification List Item", meta=(BindWidget))
	USizeBox* NotifSizeBox;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Notification List Item", meta=(BindWidget))
	USizeBox* NotifImageSizeBox;
	
	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Notification List Item", meta=(BindWidget))
	UAsyncImage* NotifImage;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Notification List Item", meta=(BindWidget))
	UPanelWidget* PlayerInfoContainer;
	
	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Notification List Item", meta=(BindWidget))
	UImage* PlayerPlatformIcon;
	
	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Notification List Item", meta=(BindWidget))
	UCommonTextBlock* PlayerNameTextBlock;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Notification List Item", meta=(BindWidget))
	UPanelWidget* EventInfoContainer;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Notification List Item", meta=(BindWidget))
	UImage* EventIcon;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Notification List Item", meta=(BindWidget))
	UCommonRichTextBlock* EventTextBlock;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Notification List Item", meta=(BindWidget))
	UCommonRichTextBlock* EventTextDetailBlock;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Notification List Item", meta=(BindWidget))
	UCommonTextBlock* TimestampTextBlock;

	// Map of configs for each notification type
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Nodecraft UI|Notification List Item Config")
	TMap<ENotificationType, FNotificationListItemConfig> NotificationTypeConfigs;

	void SetIsLoading(bool bLoading);
private:
	UPROPERTY(meta=(BindWidget))
	UNodecraftLoadGuard* LoadGuard;
	
	void UpdateBorderStyles();
};
