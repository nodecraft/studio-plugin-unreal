// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonTextBlock.h"
#include "CommonUserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "Components/Image.h"
#include "PlayerSelectorSectionHeader.generated.h"

/**
 * 
 */
UCLASS()
class NODECRAFTDISCOVERY_API UPlayerSelectorSectionHeader : public UCommonUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Details|Moderation Console", meta = (BindWidget))
	UImage* SectionHeaderIcon;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Details|Moderation Console", meta = (BindWidget))
	UCommonTextBlock* SectionHeaderTextBlock;

	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
};
