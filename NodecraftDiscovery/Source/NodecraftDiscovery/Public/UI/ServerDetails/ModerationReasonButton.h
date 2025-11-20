// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonTextBlock.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "Models/ModerationReasonDataObject.h"
#include "UI/Foundation/NodecraftButtonBase.h"
#include "ModerationReasonButton.generated.h"

/**
 * 
 */
UCLASS()
class NODECRAFTDISCOVERY_API UModerationReasonButton : public UNodecraftButtonBase, public IUserObjectListEntry
{
	GENERATED_BODY()

public:
	// IUserObjectListEntry
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
	// IUserObjectListEntry

	void SetUseSelectedStyle(bool bUseSelectedStyle);
	
protected:
	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Details", meta = (BindWidget))
	UCommonTextBlock* Label;
	
	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Details", meta = (BindWidget))
	UCommonTextBlock* Description;

	UPROPERTY(EditDefaultsOnly, Category = "Nodecraft UI|Server Details|Styles")
	TSoftClassPtr<UCommonButtonStyle> NormalButtonStyle;
	
	UPROPERTY(EditDefaultsOnly, Category = "Nodecraft UI|Server Details|Styles")
	TSoftClassPtr<UCommonButtonStyle> SelectedButtonStyle;
};
