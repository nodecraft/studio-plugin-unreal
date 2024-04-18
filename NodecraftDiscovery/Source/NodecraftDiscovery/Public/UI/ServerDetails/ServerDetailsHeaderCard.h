// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "UI/Common/NodecraftLoadGuard.h"
#include "UObject/Object.h"
#include "ServerDetailsHeaderCard.generated.h"




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

	UPROPERTY(EditAnywhere)
	TSoftClassPtr<UCommonBorderStyle> BorderStyle;

	UPROPERTY(EditAnywhere)
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
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UNodecraftLoadGuard* LoadGuard;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UAsyncImage* ServerImage;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UCommonBorder* ServerTypeBorder;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UCommonTextBlock* ServerTypeTextBlock;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UCommonTextBlock* ServerNameTextBlock;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UCommonTextBlock* ServerOwnerInfoTextBlock;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UCommonTextBlock* ServerRegionTextBlock;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UCommonTextBlock* GameVersionTextBlock;

	// TODO: Instead of this being one single block of text, this should be a list of tags that can be clicked on to filter the server list`
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UCommonTextBlock* TagsTextBlock;

	UPROPERTY(EditDefaultsOnly)
	TMap<EServerType, FServerTypeStyle> ServerTypeStyle;
	
public:
	void SetServerData(UServerDataObject* ServerDataObject, bool bIsAwaitingMoreCompleteData);

protected:
	virtual void NativeConstruct() override;
};
