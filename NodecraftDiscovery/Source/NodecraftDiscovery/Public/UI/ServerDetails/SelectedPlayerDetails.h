// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonTextBlock.h"
#include "CommonUserWidget.h"
#include "Components/PanelWidget.h"
#include "UI/Common/AsyncImage.h"
#include "SelectedPlayerDetails.generated.h"

class UPlayerServerDetailsDataObject;
/**
 * 
 */
UCLASS()
class NODECRAFTDISCOVERY_API USelectedPlayerDetails : public UCommonUserWidget
{
	GENERATED_BODY()

protected:
	// Begin Player Details
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UPanelWidget* AllTextDetails;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UAsyncImage* PlayerAvatarImage;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UCommonTextBlock* UsernameLabel;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UCommonTextBlock* PlatformLabel;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UCommonTextBlock* IdLabel;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UCommonTextBlock* FirstJoinedLabel;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UCommonTextBlock* OnlineSinceLabel;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UCommonTextBlock* LastSeenLabel;
	// End Player Details

	// Begin Ban Details
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UPanelWidget* BanDetailsSection;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UCommonTextBlock* BannedOnLabel;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UCommonTextBlock* BanExpiresLabel;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UCommonTextBlock* BanCategoryLabel;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UCommonTextBlock* BanReasonLabel;
	//End Ban Details

public:

	void ConfigureForSelectedPlayers(const TArray<UPlayerServerDetailsDataObject*> SelectedPlayers);
};
