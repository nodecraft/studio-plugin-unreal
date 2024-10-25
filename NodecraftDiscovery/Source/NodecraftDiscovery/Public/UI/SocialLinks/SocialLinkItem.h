// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Common/NodecraftDelegates.h"
#include "UI/Common/NodecraftListItemBase.h"
#include "SocialLinkItem.generated.h"

enum class ESocialLinkType : uint8;
class UCommonTextBlock;
class UImage;


UCLASS()
class NODECRAFTDISCOVERY_API USocialLinkItem : public UNodecraftListItemBase
{
	GENERATED_BODY()

	// IUserObjectListEntry
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
	// IUserObjectListEntry

	virtual FNavigationReply NativeOnNavigation(const FGeometry& MyGeometry, const FNavigationEvent& InNavigationEvent, const FNavigationReply& InDefaultReply) override;
	virtual FReply NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent) override;
	virtual void NativeOnFocusLost(const FFocusEvent& InFocusEvent) override;

public:
	FOnNavigationDelegate OnNavDelegate;
	
protected:	
	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Social", meta=(BindWidget))
	UImage* LinkIconLeft;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Social", meta=(BindWidget))
	UCommonTextBlock* Label;
	
	// Set in blueprint and never changes
	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Social", meta=(BindWidget))
	UImage* LinkIconRight;
	
	UPROPERTY(EditDefaultsOnly, Category = "Nodecraft UI|Social")
	TMap<ESocialLinkType, TSoftObjectPtr<UTexture2D>> IconMap;
	
	FString URL;
};
