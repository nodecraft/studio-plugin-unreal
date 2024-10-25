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
	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Details", meta = (BindWidget))
	UPanelWidget* AllTextDetails;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Details", meta = (BindWidget))
	UAsyncImage* PlayerAvatarImage_Bottom;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Details", meta = (BindWidget))
	UAsyncImage* PlayerAvatarImage_Middle;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Details", meta = (BindWidget))
	UAsyncImage* PlayerAvatarImage_Top;
	
	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Details", meta = (BindWidget))
	UCommonTextBlock* UsernameLabel;
	
	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Details", meta = (BindWidget))
	UCommonTextBlock* PlatformLabel;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Details", meta = (BindWidget))
	UCommonTextBlock* IdLabel;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Details", meta = (BindWidget))
	UCommonTextBlock* FirstJoinedLabel;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Details", meta = (BindWidget))
	UCommonTextBlock* OnlineSinceLabel;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Details", meta = (BindWidget))
	UCommonTextBlock* LastSeenLabel;
	// End Player Details

	// Begin Ban Details
	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Details", meta = (BindWidget))
	UPanelWidget* BanDetailsSection;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Details", meta = (BindWidget))
	UCommonTextBlock* BannedOnLabel;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Details", meta = (BindWidget))
	UCommonTextBlock* BanExpiresLabel;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Details", meta = (BindWidget))
	UCommonTextBlock* BanCategoryLabel;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Details", meta = (BindWidget))
	UCommonTextBlock* BanReasonLabel;
	//End Ban Details

public:

	void ConfigureForSelectedPlayers(const TArray<UPlayerServerDetailsDataObject*> SelectedPlayers);
};
