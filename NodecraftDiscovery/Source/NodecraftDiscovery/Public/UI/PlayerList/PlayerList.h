// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "CommonListView.h"
#include "CommonTextBlock.h"
#include "API/PlayerListType.h"
#include "Components/Image.h"
#include "Models/ServerDataObject.h"
#include "UI/Common/NodecraftScrollBox.h"
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
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Player List", meta=(BindWidget))
	UCommonLoadGuard* LoadGuard;
	
	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Player List", meta=(BindWidget))
	UCommonTextBlock* TitleTextBlock;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Player List", meta=(BindWidget))
	UImage* HeaderHairline;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Player List", meta=(BindWidget))
	UCommonListView* PlayerListView;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Nodecraft UI|Player List", meta=(BindWidget))
	FText ActionDisplayName;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Player List", meta=(BindWidget))
	UImage* NoPlayersImage;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Player List", meta=(BindWidget))
	UCommonTextBlock* NoPlayersTextBlock;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Player List", meta=(BindWidget))
	UNodecraftScrollBox* ScrollBox;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Player List", meta=(BindWidgetOptional))
	UCommonTextBlock* NoPlayersSubTextBlock;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Nodecraft UI|Player List")
	UTexture2D* NoPlayersImageAsset;

public:
	UFUNCTION(BlueprintCallable,  Category = "Nodecraft UI|Player List")
	void SetTitleText(const FText Text);

	void SetListeningForScrollInput(bool bIsListeningForInput);

	bool IsListeningForScrollInput() const;

	void LoadData(EPlayerListType PlayerListType, UServerDataObject* ServerDataObject, FSimpleDelegate OnComplete);
	void SetServerData(EPlayerListType PlayerListType, UPlayerListDataObject* PlayerListDataObject) const;
	bool HasItems();
};
