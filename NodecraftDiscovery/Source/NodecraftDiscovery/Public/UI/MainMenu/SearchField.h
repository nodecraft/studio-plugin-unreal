// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "SearchField.generated.h"

class UNodecraftButtonBase;
class UEditableTextBox;
/**
 * 
 */
UCLASS()
class NODECRAFTDISCOVERY_API USearchField : public UCommonUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;
	virtual FReply NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent) override;
	
	UPROPERTY(meta=(BindWidget))
	UEditableTextBox* SearchTextBox;

	UPROPERTY(meta=(BindWidget))
	UNodecraftButtonBase* ClearSearchButton;

	UFUNCTION()
	void OnSearchTextChanged(const FText& Text);
	
	UFUNCTION()
	void OnSearchTextCommited(const FText& Text, ETextCommit::Type CommitMethod);

private:
	FText LastSearchText;
	FDelegateHandle OnRequestClearSearch;
};
