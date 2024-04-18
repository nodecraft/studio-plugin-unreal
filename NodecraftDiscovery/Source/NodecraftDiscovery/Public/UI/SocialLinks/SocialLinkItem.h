// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "SocialLinkItem.generated.h"

enum class ESocialLinkType : uint8;
class UNodecraftButtonBase;
class UCommonTextBlock;
class UImage;
/**
 * 
 */
UCLASS()
class NODECRAFTDISCOVERY_API USocialLinkItem : public UCommonActivatableWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

	// IUserObjectListEntry
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
	// IUserObjectListEntry

protected:
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UNodecraftButtonBase* Button;
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UImage* LinkIconLeft;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UCommonTextBlock* Label;
	
	// Set in blueprint and never changes
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UImage* LinkIconRight;
	
	UPROPERTY(EditDefaultsOnly)
	TMap<ESocialLinkType, TSoftObjectPtr<UTexture2D>> IconMap;
	
	FString URL;
};
