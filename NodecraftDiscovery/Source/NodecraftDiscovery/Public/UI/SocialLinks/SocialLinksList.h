// Nodecraft, Inc. © 2012-2024, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CommonListView.h"
#include "SocialLinksList.generated.h"

class USocialLinkDataObject;
class UCommonTextBlock;
class UImage;
class UCommonListView;
/**
 * 
 */
UCLASS()
class NODECRAFTDISCOVERY_API USocialLinksList : public UUserWidget
{
	GENERATED_BODY()

public:
	void ShowLinksVisuals(bool bShowLinks);
	void SetListItems(const TArray<USocialLinkDataObject*>& ListItems);
	UListViewBase::FOnListEntryGenerated& OnEntryWidgetGenerated();

	virtual void NativeConstruct() override;

	UWidget* GetFirstWidget() const;

	FSimpleDelegate OnControllerMovedUpFromFirstItem;
	FSimpleDelegate OnControllerMovedDownFromLastItem;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Social", meta=(BindWidget))
	UImage* NoLinksImage;
	
	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Social", meta=(BindWidget))
	UCommonTextBlock* NoLinksText;
	
	UPROPERTY(BlueprintReadOnly, Category = "Nodecraft UI|Social", meta=(BindWidget))
	UCommonListView* ListView;
};
