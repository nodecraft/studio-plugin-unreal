// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "UI/Common/NodecraftLoadGuard.h"
#include "UObject/Object.h"
#include "ServerDetailsHeaderCard.generated.h"


class UCommonButtonStyle;
class UTagButton;
class UHorizontalBox;
class UCommonBorder;
class UServerDataObject;
class UCommonTextStyle;
enum class EServerType : uint8;
class UCommonTextBlock;
class UAsyncImage;
class UCommonLoadGuard;
class UCommonBorderStyle;


USTRUCT()
struct FServerTypeStyle
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Nodecraft UI|Styles")
	TSoftClassPtr<UCommonBorderStyle> BorderStyle;

	UPROPERTY(EditAnywhere, Category = "Nodecraft UI|Styles")
	TSoftClassPtr<UCommonTextStyle> TextStyle;
};

/**
 * 
 */
UCLASS()
class NODECRAFTDISCOVERY_API UServerDetailsHeaderCard : public UCommonUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Details", meta = (BindWidget))
	UNodecraftLoadGuard* LoadGuard;
	
	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Details", meta = (BindWidget))
	UAsyncImage* ServerImage;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Details", meta = (BindWidget))
	UCommonBorder* ServerTypeBorder;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Details", meta = (BindWidget))
	UCommonTextBlock* ServerTypeTextBlock;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Details", meta = (BindWidget))
	UCommonTextBlock* ServerNameTextBlock;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Details", meta = (BindWidget))
	UCommonTextBlock* ServerOwnerInfoTextBlock;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Details", meta = (BindWidget))
	UCommonTextBlock* ServerRegionTextBlock;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Details", meta = (BindWidget))
	UCommonTextBlock* GameVersionTextBlock;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Details", meta = (BindWidget))
	UHorizontalBox* TagsBox;

	UPROPERTY(EditDefaultsOnly, Category = "Nodecraft UI|Server Details|Moderation Console")
	TSoftClassPtr<UTagButton> ServerTagButtonClass;

	UPROPERTY(EditDefaultsOnly, Category = "Nodecraft UI|Server Details|Moderation Console")
	TMap<EServerType, FServerTypeStyle> ServerTypeStyle;
	
public:
	void SetServerData(UServerDataObject* ServerDataObject, bool bIsAwaitingMoreCompleteData);

protected:
	virtual void NativeConstruct() override;
};
