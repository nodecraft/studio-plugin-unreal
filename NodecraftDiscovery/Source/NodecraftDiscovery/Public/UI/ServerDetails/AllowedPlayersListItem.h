// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Common/NodecraftDelegates.h"
#include "UI/Common/NodecraftListItemBase.h"
#include "AllowedPlayersListItem.generated.h"

class UHorizontalBox;
class UNodecraftLoadGuard;
enum class EAllowStatus : uint8;
class UCommonTextStyle;
class UImage;
class UCommonTextBlock;
class UAsyncImage;

USTRUCT()
struct FAllowStatusStyle
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Nodecraft UI|Styles")
	TSubclassOf<UCommonTextStyle> TextStyle;

	UPROPERTY(EditAnywhere, Category = "Nodecraft UI|Styles")
	FLinearColor TintColor = FLinearColor::Transparent;
};
/**
 * 
 */
UCLASS()
class NODECRAFTDISCOVERY_API UAllowedPlayersListItem : public UNodecraftListItemBase
{
	GENERATED_BODY()

public:
	FOnNavigationDelegate OnNavDelegate;

protected:
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
	virtual FReply NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent) override;
	virtual void NativeOnFocusLost(const FFocusEvent& InFocusEvent) override;
	virtual FNavigationReply NativeOnNavigation(const FGeometry& MyGeometry, const FNavigationEvent& InNavigationEvent, const FNavigationReply& InDefaultReply) override;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Details", meta = (BindWidget))
	UNodecraftLoadGuard* LoadGuard;
	
	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Details", meta = (BindWidget))
	UAsyncImage* Avatar;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Details", meta = (BindWidget))
	UCommonTextBlock* Username;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Details", meta = (BindWidget))
	UImage* IdentImage;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Details", meta = (BindWidget))
	UCommonTextBlock* IdentText;
	
	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Details", meta = (BindWidget))
	UImage* StatusIcon;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Details", meta = (BindWidget))
	UCommonTextBlock* StatusText;
	
	UPROPERTY(EditDefaultsOnly, Category = "Nodecraft UI|Server Details")
	FAllowStatusStyle PendingStyle;

	UPROPERTY(EditDefaultsOnly, Category = "Nodecraft UI|Server Details")
	FAllowStatusStyle AcceptedStyle;

	UPROPERTY(EditDefaultsOnly, Category = "Nodecraft UI|Server Details")
	FAllowStatusStyle DeclinedStyle;
	
	UPROPERTY(EditDefaultsOnly, Category = "Nodecraft UI|Server Details")
	FAllowStatusStyle RevokedStyle;

	UPROPERTY(EditDefaultsOnly, Category = "Nodecraft UI|Server Details")
	TMap<EAllowStatus, TSoftObjectPtr<UTexture2D>> StatusIcons;

	UPROPERTY(EditDefaultsOnly, Category = "Nodecraft UI|Server Details")
	TMap<EAllowStatus, FText> StatusMessage;

private:
	void UpdateActionBar();
};
