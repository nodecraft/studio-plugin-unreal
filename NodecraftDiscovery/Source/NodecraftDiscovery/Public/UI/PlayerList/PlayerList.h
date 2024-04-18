// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "CommonListView.h"
#include "CommonTextBlock.h"
#include "API/PlayerListType.h"
#include "Components/Image.h"
#include "Models/ServerDataObject.h"
#include "UObject/Object.h"
#include "PlayerList.generated.h"

class UCommonLoadGuard;
class UPlayerListDataObject;
/**
 * 
 */
UCLASS()
class NODECRAFTDISCOVERY_API UPlayerList : public UCommonActivatableWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UCommonLoadGuard* LoadGuard;
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UCommonTextBlock* TitleTextBlock;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UImage* HeaderHairline;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UCommonListView* PlayerListView;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UImage* NoPlayersImage;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UCommonTextBlock* NoPlayersTextBlock;

public:
	UFUNCTION(BlueprintCallable)
	void SetTitleText(const FText Text);

	UFUNCTION(BlueprintCallable)
	void LoadData(EPlayerListType PlayerListType, UServerDataObject* ServerDataObject);
	void SetServerData(EPlayerListType PlayerListType, UPlayerListDataObject* PlayerListDataObject) const;
};
