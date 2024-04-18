// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "ServerRegionRow.generated.h"

class UCommonTextStyle;
class UServerRegionDataObject;
DECLARE_DELEGATE_OneParam(FOnServerRegionButtonClicked, FString /* RegionID */);

class UImage;
class UNodecraftButtonBase;
class UCommonTextBlock;
class UCheckBox;

/**
 * 
 */
UCLASS()
class NODECRAFTDISCOVERY_API UServerRegionRow : public UCommonUserWidget
{
	GENERATED_BODY()

public:
	void Configure(UServerRegionDataObject* RegionDataObject, bool bIsDefaultRegion, bool bIsSelectedRegion);
	
	void SetIsCurrent(bool bIsCurrent, bool bShowCurrentText);
	void SetIsDefault(bool bIsDefault);
	void UpdateStyles(bool bIsCurrent);
	FString GetRegionID() const;
	
	FOnServerRegionButtonClicked OnServerRegionButtonClicked;

protected:
	UPROPERTY(EditDefaultsOnly)
	TSoftClassPtr<UCommonTextStyle> SelectedTextStyle;

	UPROPERTY(EditDefaultsOnly)
	TSoftClassPtr<UCommonTextStyle> NormalTextStyle;

	UPROPERTY(EditDefaultsOnly)
	FColor ButtonStateImageSelectedColor;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UNodecraftButtonBase* Button;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UImage* ButtonStateImage;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UImage* EmptyCircle;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UCommonTextBlock* RegionTitle;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UCommonTextBlock* CurrentRegion;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UCommonTextBlock* DefaultText;
	
	FString RegionID;
};
