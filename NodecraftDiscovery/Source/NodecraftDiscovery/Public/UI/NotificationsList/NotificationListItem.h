// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "CommonBorder.h"
#include "CommonButtonBase.h"
#include "CommonRichTextBlock.h"
#include "CommonTextBlock.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "Components/SizeBox.h"
#include "Models/NotificationDataObject.h"
#include "UI/Common/AsyncImage.h"
#include "UI/Common/NodecraftLoadGuard.h"
#include "UI/Foundation/NodecraftButtonBase.h"
#include "NotificationListItem.generated.h"

USTRUCT(BlueprintType)
struct FNotificationListItemConfig
{
	GENERATED_BODY()

	FNotificationListItemConfig() = default;

	UPROPERTY(EditAnywhere)
	FLinearColor BorderColor = FLinearColor::Transparent;

	UPROPERTY(EditAnywhere)
	FLinearColor BackgroundColor = FLinearColor::Transparent;

	UPROPERTY(EditAnywhere)
	FLinearColor ContentAreaBackgroundColor = FLinearColor::Transparent;

	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UTexture2D> NotifIcon;

	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UTexture2D> EventTextIcon;

	UPROPERTY(EditAnywhere)
	FLinearColor EventTextIconColor = FLinearColor::Transparent;

	UPROPERTY(EditAnywhere)
	FLinearColor EventTextColor = FLinearColor::Transparent;

	UPROPERTY(EditAnywhere)
	TSoftClassPtr<UCommonButtonStyle> PrimaryButtonStyle;

	UPROPERTY(EditAnywhere)
	FLinearColor PrimaryButtonBorderColor = FLinearColor::Transparent;

	UPROPERTY(EditAnywhere)
	FLinearColor PrimaryButtonColorLeft = FLinearColor::Transparent;

	// dlohnes 01/29/24: Currently unused as we are just applying a flat color, but we want to use a material that applies a gradient
	UPROPERTY(EditAnywhere)
	FLinearColor PrimaryButtonColorRight = FLinearColor::Transparent;
};


/**
 * 
 */
UCLASS()
class NODECRAFTDISCOVERY_API UNotificationListItem : public UCommonActivatableWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

	enum ENotifListItemSize
	{
		Small,
		Large
	};


	void ConfigForKickNotif(const UNotificationDataObject* NotificationDataObject);
	void ConfigForBanNotif(const UNotificationDataObject* NotificationDataObject);
	void ConfigForAllowNotif(const UNotificationDataObject* NotificationDataObject);
	void ConfigForSessionNotif(const UNotificationDataObject* NotificationDataObject);
	void ConfigForInviteNotif(const UNotificationDataObject* NotificationDataObject);
	void ConfigForModerationNotif(const UNotificationDataObject* NotificationDataObject);
	void UpdateTimestamp(const FDateTime& Time);

	void UpdateItemSize(ENotifListItemSize Size);
	// IUserObjectListEntry
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
	// IUserObjectListEntry

protected:
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UCommonBorder* NotifBorder;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UBorder* ContentBorder;

	// Controls the height of the entire notif with height override
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	USizeBox* NotifSizeBox;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	USizeBox* NotifImageSizeBox;
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UAsyncImage* NotifImage;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UPanelWidget* PlayerInfoContainer;
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UImage* PlayerPlatformIcon;
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UCommonTextBlock* PlayerNameTextBlock;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UPanelWidget* EventInfoContainer;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UImage* EventIcon;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UCommonRichTextBlock* EventTextBlock;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UCommonRichTextBlock* EventTextDetailBlock;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UCommonTextBlock* TimestampTextBlock;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UCommonButtonBase* PrimaryActionButton;
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UNodecraftButtonBase* DismissActionButton;

	// Dynamic material instance for the border and background
	UPROPERTY()
	UMaterialInstanceDynamic* BorderMaterialInstance;

	// Map of configs for each notification type
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TMap<ENotificationType, FNotificationListItemConfig> NotificationTypeConfigs;

	void SetIsLoading(bool bLoading);
private:
	UPROPERTY(meta=(BindWidget))
	UNodecraftLoadGuard* LoadGuard;

public:
	virtual void NativeConstruct() override;
};
