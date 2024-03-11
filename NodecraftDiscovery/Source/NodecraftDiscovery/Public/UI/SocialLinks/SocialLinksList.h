// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
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

protected:
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UImage* NoLinksImage;
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UCommonTextBlock* NoLinksText;
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UCommonListView* ListView;
};
