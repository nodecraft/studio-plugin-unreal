// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "CommonInputTypeEnum.h"
#include "CommonTextBlock.h"
#include "ServerCardBase.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "Common/NodecraftDelegates.h"
#include "Models/ServerDataObject.h"
#include "UI/Common/AsyncImage.h"
#include "ServerCard.generated.h"

class UNodecraftButtonBase;
class UNodecraftLoadingButton;
class UBorder;

/**
 * 
 */
UCLASS()
class NODECRAFTDISCOVERY_API UServerCard : public UServerCardBase, public IUserObjectListEntry
{
	GENERATED_BODY()

public:
	void SetServerData(UServerDataObject* InServerData);

protected:
	// IUserObjectListEntry
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
	// IUserObjectListEntry
	
	virtual FReply NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent) override;
	virtual void NativeOnFocusLost(const FFocusEvent& InFocusEvent) override;
	virtual void NativeOnInitialized() override;
	
	// The main contents of the card. Mutually exclusive from showing the create server button
	UPROPERTY(meta=(BindWidget))
	UPanelWidget* Card;

	// The create server button. Mututally exclusive from showing the Card contents.
	// Will be shown if we have an empty server entry (which is how we cheaply show the create server button) in
	// the private server list
	UPROPERTY(meta=(BindWidget))
	UUserWidget* CreateServerButton;
	
	UPROPERTY(meta=(BindWidget))
	UCommonTextBlock* ServerNameTextBlock;

	UPROPERTY(meta=(BindWidget))
	UCommonTextBlock* ServerPlayersTextBlock;

	UPROPERTY(meta=(BindWidget))
	UCommonTextBlock* ServerDescriptionTextBlock;	

	UPROPERTY(meta=(BindWidget))
	UAsyncImage* ServerImage;

	UPROPERTY(meta=(BindWidget))
	UNodecraftLoadingButton* JoinButton;

	UPROPERTY(meta=(BindWidget))
	UNodecraftButtonBase* ShowServerDetailsButton;

	UPROPERTY(meta=(BindWidget))
	UUserWidget* FavoriteTag;

	UPROPERTY(meta=(BindWidget))
	UUserWidget* PrivateTag;

	UPROPERTY(meta=(BindWidget))
	UUserWidget* OwnerTag;

	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Server Card", meta=(BindWidget))
	UBorder* Border;

	UPROPERTY()
	UServerDataObject* ServerDataObject = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Nodecraft UI|Styles")
	FLinearColor BorderColorDefault = FLinearColor::Transparent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Nodecraft UI|Styles")
	FLinearColor BorderColorFocus = FLinearColor::Transparent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Nodecraft UI|Styles")
	FMargin BorderPaddingDefault = FMargin(0);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Nodecraft UI|Styles")
	FMargin BorderPaddingFocus = FMargin(2);

	UPROPERTY(EditAnywhere, Category = "Nodecraft UI|Input", meta=(RowType="/Script/CommonUI.CommonInputActionDataBase"))
	FDataTableRowHandle ShowServerDetailsInputActionData;
	
	UPROPERTY(EditAnywhere, Category = "Nodecraft UI|Input", meta=(RowType="/Script/CommonUI.CommonInputActionDataBase"))
	FDataTableRowHandle JoinServerInputActionData;
	
	UFUNCTION(BlueprintCallable, Category = "Nodecraft UI|Server Card")
	void OpenServerDetails();

	UFUNCTION(BlueprintCallable, Category = "Nodecraft UI|Server Card")
	void ResetBorderStyle();

private:
	void ResetJoinButtonLoadingState() const;
	void RefreshActions(ECommonInputType InputType);
	
	FUIActionBindingHandle JoinServerUIActionHandle;
	FUIActionBindingHandle ShowServerDetailsUIActionHandle;
};
