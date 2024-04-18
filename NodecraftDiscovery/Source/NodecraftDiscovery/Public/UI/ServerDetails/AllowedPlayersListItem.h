// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "AllowedPlayersListItem.generated.h"

class UHorizontalBox;
class UNodecraftLoadGuard;
enum class EAllowStatus : uint8;
class UCommonTextStyle;
class UNodecraftButtonBase;
class UImage;
class UCommonTextBlock;
class UAsyncImage;

USTRUCT()
struct FAllowStatusStyle
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TSubclassOf<UCommonTextStyle> TextStyle;

	UPROPERTY(EditAnywhere)
	FLinearColor TintColor = FLinearColor::Transparent;
};
/**
 * 
 */
UCLASS()
class NODECRAFTDISCOVERY_API UAllowedPlayersListItem : public UCommonActivatableWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

protected:
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UNodecraftLoadGuard* LoadGuard;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UAsyncImage* Avatar;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UCommonTextBlock* Username;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UImage* IdentImage;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UCommonTextBlock* IdentText;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UImage* StatusIcon;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UCommonTextBlock* StatusText;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UNodecraftButtonBase* RevokeButton;
	
	UPROPERTY(EditDefaultsOnly)
	FAllowStatusStyle PendingStyle;

	UPROPERTY(EditDefaultsOnly)
	FAllowStatusStyle AcceptedStyle;

	UPROPERTY(EditDefaultsOnly)
	FAllowStatusStyle DeclinedStyle;
	
	UPROPERTY(EditDefaultsOnly)
	FAllowStatusStyle RevokedStyle;

	UPROPERTY(EditDefaultsOnly)
	TMap<EAllowStatus, TSoftObjectPtr<UTexture2D>> StatusIcons;

	UPROPERTY(EditDefaultsOnly)
	TMap<EAllowStatus, FText> StatusMessage;
};
